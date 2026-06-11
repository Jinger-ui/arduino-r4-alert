#pragma once

/**
 * @file config.h
 * @brief D3 绿 LED 常亮 + D6 WS2812 灯带（有声时红色常亮）
 *
 * 灯带：红线 5V、黑线 GND、黄线 DIN → D6
 * 绿灯：D3（经电阻到 GND）
 * 电脑音频：3.5mm 耳机孔信号 → 1µF → A5， Sleeve → GND（必须共地）
 *           10k 偏置到 2.5V；PC 音量建议 50% 以上
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

#define PIN_AUDIO_IN         A5

// 1 = 电脑线路音频（峰峰值检测，更灵敏）；0 = 麦克风/环境声
#define AUDIO_LINE_IN_MODE     1

// 兜底默认值；上电校准时请保持电脑静音
#define AUDIO_DEVIATION_HIGH   35
#define AUDIO_DEVIATION_LOW    20
#define AUDIO_MARGIN_HIGH      12   // 线路输入：安静基线 + 12 即触发
#define AUDIO_MARGIN_LOW       5    // 释放余量
#define AUDIO_HYSTERESIS_MIN   6

#define SILENCE_RECOVERY_MS    2000  // 电脑停播后约 2 s 熄灯

#define SAMPLE_INTERVAL_MS     5
#define SAMPLES_FOR_PEAK       16
