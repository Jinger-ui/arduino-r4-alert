/**
 * @file main.ino
 * @brief UNO R4 WiFi：D3 绿灯常亮；有声时灯带红常亮，无声约 1 s 后熄灭
 */

#include "config.h"
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

enum class SystemState : uint8_t {
    IDLE,
    STRIP_RED_ON
};

static SystemState state = SystemState::IDLE;
static uint32_t lastSampleMs = 0;
static uint32_t lastSoundDetectedMs = 0;

static uint16_t audioSamples[SAMPLES_FOR_PEAK];
static uint8_t sampleIndex = 0;
static bool samplesReady = false;
static bool soundDetected = false;

// 上电校准后的触发/释放阈值
static uint16_t acTriggerHigh = AUDIO_DEVIATION_HIGH;
static uint16_t acTriggerLow = AUDIO_DEVIATION_LOW;

static uint16_t calcAcFromBuffer(const uint16_t *buf, uint8_t n);
static void calibrateNoiseFloor(void);
static void greenLedOn(void);
static void stripInit(void);
static void stripAllOff(void);
static void stripSetAllRed(void);
static void sampleAudio(void);
static bool detectSound(uint16_t ac);
static void processAudioBatch(uint32_t nowMs);
static void updateState(uint32_t nowMs);
static void printDebug(uint16_t pk, uint16_t avg, uint16_t ac);

#if STRIP_ONLY_TEST && LED_HW_TEST_MODE
void setup() {
    strip.begin();
    strip.setBrightness(STRIP_BRIGHTNESS);
    for (uint16_t i = 0; i < NUM_PIXELS; i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0));
    }
    strip.show();
}
void loop() {
    delay(1000);
}
#else

void setup() {
    DEBUG_INIT();
    DEBUG_PRINTLN(F("D3 green ON | D6 strip RED on sound"));
    DEBUG_PRINTLN(F("UNO R4 WiFi"));

    pinMode(PIN_AUDIO_IN, INPUT);
    greenLedOn();
    stripInit();
    stripAllOff();

    DEBUG_PRINTLN(F("Calibrating... please keep quiet ~1s"));
    calibrateNoiseFloor();

    lastSampleMs = millis();
    lastSoundDetectedMs = millis();
    state = SystemState::IDLE;
    soundDetected = false;

    DEBUG_PRINTF("pixels=%d trigger>=%u release<%u\n",
                 NUM_PIXELS, acTriggerHigh, acTriggerLow);
}

void loop() {
    uint32_t now = millis();

    greenLedOn();

    if (now - lastSampleMs >= SAMPLE_INTERVAL_MS) {
        lastSampleMs = now;
        sampleAudio();
    }
    if (samplesReady) {
        processAudioBatch(now);
        samplesReady = false;
    }

    updateState(now);
}

#endif

static uint16_t calcAcFromBuffer(const uint16_t *buf, uint8_t n) {
    uint32_t sum = 0;
    for (uint8_t i = 0; i < n; i++) {
        sum += buf[i];
    }
    uint16_t avg = sum / n;
    uint16_t ac = 0;
    for (uint8_t i = 0; i < n; i++) {
        uint16_t d = (buf[i] > avg) ? (buf[i] - avg) : (avg - buf[i]);
        ac = max(ac, d);
    }
    return ac;
}

static void calibrateNoiseFloor(void) {
    uint16_t maxQuietAc = 0;

    for (uint8_t round = 0; round < 40; round++) {
        uint16_t snap[5];
        for (uint8_t i = 0; i < 5; i++) {
            snap[i] = analogRead(PIN_AUDIO_IN);
            delay(8);
        }
        maxQuietAc = max(maxQuietAc, calcAcFromBuffer(snap, 5));
    }

    acTriggerLow = maxQuietAc + AUDIO_MARGIN_LOW;
    acTriggerHigh = maxQuietAc + AUDIO_MARGIN_HIGH;
    if (acTriggerHigh <= acTriggerLow + AUDIO_HYSTERESIS_MIN) {
        acTriggerHigh = acTriggerLow + AUDIO_HYSTERESIS_MIN;
    }

    DEBUG_PRINTF("quiet ac max=%u -> high=%u low=%u\n",
                 maxQuietAc, acTriggerHigh, acTriggerLow);
}

static void greenLedOn(void) {
    pinMode(PIN_LED_GREEN, OUTPUT);
    digitalWrite(PIN_LED_GREEN, HIGH);
}

static void stripInit(void) {
    strip.begin();
    strip.clear();
    strip.setBrightness(STRIP_BRIGHTNESS);
    strip.show();
}

static void stripAllOff(void) {
    strip.clear();
    strip.show();
}

static void stripSetAllRed(void) {
    for (uint16_t i = 0; i < NUM_PIXELS; i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0));
    }
    strip.show();
}

static void sampleAudio(void) {
    audioSamples[sampleIndex] = analogRead(PIN_AUDIO_IN);
    sampleIndex = (sampleIndex + 1) % SAMPLES_FOR_PEAK;
    if (sampleIndex == 0) {
        samplesReady = true;
    }
}

static bool detectSound(uint16_t ac) {
    static bool latched = false;
    if (!latched && ac >= acTriggerHigh) {
        latched = true;
    } else if (latched && ac < acTriggerLow) {
        latched = false;
    }
    return latched;
}

static void processAudioBatch(uint32_t nowMs) {
    uint32_t sum = 0;
    uint16_t pk = 0;
    for (uint8_t i = 0; i < SAMPLES_FOR_PEAK; i++) {
        uint16_t s = audioSamples[i];
        sum += s;
        pk = max(pk, s);
    }
    uint16_t avg = sum / SAMPLES_FOR_PEAK;
    uint16_t ac = 0;
    for (uint8_t i = 0; i < SAMPLES_FOR_PEAK; i++) {
        uint16_t s = audioSamples[i];
        uint16_t d = (s > avg) ? (s - avg) : (avg - s);
        ac = max(ac, d);
    }
    soundDetected = detectSound(ac);
    if (soundDetected) {
        lastSoundDetectedMs = nowMs;
    }
#if DEBUG_ENABLED
    static uint32_t lastDbg = 0;
    if (nowMs - lastDbg > 500) {
        lastDbg = nowMs;
        printDebug(pk, avg, ac);
    }
#endif
}

static void updateState(uint32_t nowMs) {
    if (soundDetected) {
        if (state == SystemState::IDLE) {
            DEBUG_PRINTLN(F(">>> sound -> red ON"));
            state = SystemState::STRIP_RED_ON;
        }
        stripSetAllRed();
        return;
    }

    if (state == SystemState::STRIP_RED_ON) {
        if ((nowMs - lastSoundDetectedMs) >= SILENCE_RECOVERY_MS) {
            DEBUG_PRINTLN(F("<<< quiet -> off"));
            state = SystemState::IDLE;
            stripAllOff();
        }
        return;
    }

    stripAllOff();
}

static void printDebug(uint16_t pk, uint16_t avg, uint16_t ac) {
    DEBUG_PRINTF("pk:%u avg:%u ac:%u th:%u/%u snd:%s st:",
                 pk, avg, ac, acTriggerHigh, acTriggerLow,
                 soundDetected ? "Y" : "n");
    DEBUG_PRINTLN(state == SystemState::STRIP_RED_ON ? "red" : "idle");
}
