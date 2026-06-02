#pragma once

/**
 * @file config.h
 * @brief D3 绿 LED 常亮 + D6 WS2812 灯带（有声时彩虹渐变流动）
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

// 0 = 正常运行（绿常亮 + 声控渐变）；测灯带时改为 1
#define LED_HW_TEST_MODE     0
#define STRIP_ONLY_TEST      0

#define PIN_LED_GREEN        3
#define PIN_NEOPIXEL         6
#define NUM_PIXELS           10

#define STRIP_BRIGHTNESS     60    // 灯带整体亮度 0–255
#define GRADIENT_HUE_STEP    450   // 每帧色相偏移，越大流动越快
#define GRADIENT_FRAME_MS    20    // 渐变刷新间隔 ms

#define PIN_AUDIO_IN         A1
#define AUDIO_DEVIATION_HIGH   28
#define AUDIO_DEVIATION_LOW    16
#define SILENCE_RECOVERY_MS    500

#define SAMPLE_INTERVAL_MS     10
#define SAMPLES_FOR_PEAK       12
