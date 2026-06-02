# Arduino UNO R4 WiFi — 声控警示灯

基于 **Arduino UNO R4 WiFi** 的声音检测警示项目：绿灯常亮，检测到声音时 WS2812 灯带显示彩虹渐变流动。

## 硬件

| 部件 | 连接 |
|------|------|
| 绿色 LED | **D3**（经限流电阻到 GND） |
| WS2812 灯带（10 颗） | **5V**、**GND**、**DIN → D6**（建议 DI 串 330Ω） |
| 音频输入 | **A1**（TRRS 合路 + 1µF + 10kΩ 偏置，见 `Arduino/src/config.h`） |

## 行为

- **安静**：D3 绿灯常亮，灯带熄灭  
- **有声**：绿灯仍亮，灯带彩虹渐变沿灯带流动（音量越大流动略快）  
- **恢复安静约 0.5 s**：灯带熄灭  

## 编译与上传

需要 [PlatformIO](https://platformio.org/)。

```bash
cd Arduino
pio run -t upload --upload-port COMx
```

Windows 也可双击 `Arduino/upload.bat`，按提示输入 COM 口。

板型：`uno_r4_wifi`（见 `Arduino/platformio.ini`）。

## 配置

编辑 `Arduino/src/config.h`：

- `NUM_PIXELS` — 灯带 LED 数量  
- `STRIP_BRIGHTNESS` / `GRADIENT_HUE_STEP` — 灯带效果  
- `AUDIO_DEVIATION_HIGH` / `LOW` — 声音检测灵敏度  

## 目录结构

```
Arduino/
  platformio.ini
  src/
    main.ino
    config.h
  upload.bat
  writing/          # 布线参考数据
```

## 许可证

个人学习/实验用途；第三方库遵循各自许可证（如 Adafruit NeoPixel）。
