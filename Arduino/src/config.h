#pragma once

/**
 * @file config.h
 * @brief D3 绿 LED 常亮 + D6 WS2812 灯带（有声时红色常亮）
 *
 * 灯带：红线 5V、黑线 GND、黄线 DIN → D6
 * 绿灯：D3（经电阻到 GND）
 * 音频：A1（TRRS 合路 + 1µF + 10k 偏置）
 */

#define DEBUG_ENABLED        1
#define DEBUG_BAUD           115200

#if DEBUG_ENABLED
    #include <cstdio>
    #define DEBUG_INIT()       Serial.begin(DEBUG_BAUD)
    #define DEBUG_PRINT(x)     Serial.print(x)
    #define DEBUG_PRINTLN(x)   Serial.println(x)
    #define DEBUG_PRINTF(fmt, ...)                                  \
        do {                                                        \
            char _b[160];                                           \
            std::snprintf(_b, sizeof(_b), fmt, ##__VA_ARGS__);       \
            Serial.print(_b);                                       \
        } while (0)
#else
    #define DEBUG_INIT()
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTF(fmt, ...)
#endif

#define LED_HW_TEST_MODE     0
#define STRIP_ONLY_TEST      0

#define PIN_LED_GREEN        3
#define PIN_NEOPIXEL         6
#define NUM_PIXELS           10

#define STRIP_BRIGHTNESS     80    // 红色亮度 0–255

#define PIN_AUDIO_IN         A1

// 兜底默认值；上电校准后会按环境噪声重算
#define AUDIO_DEVIATION_HIGH   35
#define AUDIO_DEVIATION_LOW    20
#define AUDIO_MARGIN_HIGH      24   // 触发 = quiet_max + 24
#define AUDIO_MARGIN_LOW       10   // 释放 = quiet_max + 10
#define AUDIO_HYSTERESIS_MIN   12

#define SILENCE_RECOVERY_MS    1000  // 声音结束后约 1 s 再熄红灯

#define SAMPLE_INTERVAL_MS     10
#define SAMPLES_FOR_PEAK       12
