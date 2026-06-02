@echo off
chcp 65001 >nul
cd /d "%~dp0"

echo ========================================
echo  UNO R4 WiFi 上传（灯带测试固件）
echo ========================================
echo.
echo 1. 用 USB 连接开发板
echo 2. 在「设备管理器 - 端口」查看 COM 号
echo.

D:\anaconda\python.exe -m platformio device list
echo.

set /p PORT=请输入 COM 口（例如 COM4，不要猜 COM3）: 
if "%PORT%"=="" (
    echo 未输入端口，已取消。
    pause
    exit /b 1
)

echo.
echo 正在上传到 %PORT% ...
D:\anaconda\python.exe -m platformio run -t upload --upload-port %PORT%

if errorlevel 1 (
    echo.
    echo 上传失败。请确认：板子已插入、驱动已安装、COM 号正确。
) else (
    echo.
    echo 上传成功。
)
pause
