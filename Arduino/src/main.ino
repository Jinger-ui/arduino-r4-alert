/**
 * @file main.ino
 * @brief UNO R4 WiFi：D3 绿灯常亮；A1 有声时 D6 灯带彩虹渐变流动
 */

#include "config.h"
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

enum class SystemState : uint8_t {
    IDLE,
    GRADIENT_ACTIVE
};

static SystemState state = SystemState::IDLE;
static uint32_t lastSampleMs = 0;
static uint32_t lastSoundDetectedMs = 0;
static uint32_t lastGradientMs = 0;
static uint16_t gradientHueOffset = 0;

static uint16_t audioSamples[SAMPLES_FOR_PEAK];
static uint8_t sampleIndex = 0;
static bool samplesReady = false;
static bool soundDetected = false;
static uint16_t lastAcDeviation = 0;

static void greenLedOn(void);
static void stripInit(void);
static void stripAllOff(void);
static void stripShowGradient(uint16_t hueOffset);
static void sampleAudio(void);
static bool detectSound(uint16_t acPeakDeviation);
static void processAudioBatch(uint32_t nowMs);
static void updateState(uint32_t nowMs);
static void printDebug(uint16_t pk, uint16_t avg, uint16_t ac, bool snd);

#if STRIP_ONLY_TEST && LED_HW_TEST_MODE
void setup() {
    strip.begin();
    strip.setBrightness(STRIP_BRIGHTNESS);
    strip.show();
}
void loop() {
    stripShowGradient(gradientHueOffset);
    gradientHueOffset += GRADIENT_HUE_STEP;
    delay(GRADIENT_FRAME_MS);
}
#else

void setup() {
    DEBUG_INIT();
    DEBUG_PRINTLN(F("D3 green ON | D6 strip gradient on sound"));
    DEBUG_PRINTLN(F("UNO R4 WiFi"));

    pinMode(PIN_AUDIO_IN, INPUT);
    greenLedOn();
    stripInit();
    stripAllOff();

    lastSampleMs = millis();
    lastSoundDetectedMs = millis();
    state = SystemState::IDLE;

    DEBUG_PRINTF("pixels=%d acHigh=%d acLow=%d\n",
                 NUM_PIXELS, AUDIO_DEVIATION_HIGH, AUDIO_DEVIATION_LOW);
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

    if (state == SystemState::GRADIENT_ACTIVE) {
        if (now - lastGradientMs >= GRADIENT_FRAME_MS) {
            lastGradientMs = now;
            uint16_t speedBoost = min<uint16_t>(lastAcDeviation * 8U, 2000U);
            gradientHueOffset += GRADIENT_HUE_STEP + speedBoost;
            stripShowGradient(gradientHueOffset);
        }
    }
}

#endif  // strip-only test

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

static void stripShowGradient(uint16_t hueOffset) {
    for (uint16_t i = 0; i < NUM_PIXELS; i++) {
        uint16_t hue = hueOffset + (uint32_t)i * 65535U / NUM_PIXELS;
        strip.setPixelColor(i, strip.ColorHSV(hue, 255, 200));
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
    lastAcDeviation = ac;
    soundDetected = detectSound(ac);
    if (soundDetected) {
        lastSoundDetectedMs = nowMs;
    }
#if DEBUG_ENABLED
    static uint32_t lastDbg = 0;
    if (nowMs - lastDbg > 500) {
        lastDbg = nowMs;
        printDebug(pk, avg, ac, soundDetected);
    }
#endif
}

static bool detectSound(uint16_t acPeakDeviation) {
    static bool wasAbove = false;
    if (!wasAbove && acPeakDeviation > AUDIO_DEVIATION_HIGH) {
        wasAbove = true;
        return true;
    }
    if (wasAbove && acPeakDeviation < AUDIO_DEVIATION_LOW) {
        wasAbove = false;
        return false;
    }
    return wasAbove;
}

static void updateState(uint32_t nowMs) {
    if (state == SystemState::IDLE) {
        stripAllOff();
        if (soundDetected) {
            DEBUG_PRINTLN(F(">>> sound -> gradient"));
            state = SystemState::GRADIENT_ACTIVE;
            lastGradientMs = nowMs;
            gradientHueOffset = 0;
            stripShowGradient(gradientHueOffset);
        }
        return;
    }

    if (!soundDetected && (nowMs - lastSoundDetectedMs) > SILENCE_RECOVERY_MS) {
        DEBUG_PRINTLN(F("<<< silence -> idle"));
        state = SystemState::IDLE;
        stripAllOff();
    }
}

static void printDebug(uint16_t pk, uint16_t avg, uint16_t ac, bool snd) {
    DEBUG_PRINTF("pk:%u avg:%u ac:%u snd:%s st:",
                 pk, avg, ac, snd ? "Y" : "n");
    DEBUG_PRINTLN(state == SystemState::GRADIENT_ACTIVE ? "grad" : "idle");
}
