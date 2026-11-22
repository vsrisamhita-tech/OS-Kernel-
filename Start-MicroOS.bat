@echo off
TITLE CYBERLAB OS - Dual Architecture Platform Launcher
COLOR 0B

echo =========================================================================
echo       CYBERLAB OS :: 1-CLICK VIRTUALBOX ENGINEERING LAUNCHER
echo   Safe-State Analysis: How Conservative Is Deadlock Avoidance?
echo   Researcher: Vuppalapati Sri Samhita (USN: 25BTRCL285, CSE-AIML-E)
echo =========================================================================
echo.

SET "VBOX_PATH=C:\Program Files\Oracle\VirtualBox\VBoxManage.exe"
IF NOT EXIST "%VBOX_PATH%" (
    WHERE VBoxManage >nul 2>&1
    IF %ERRORLEVEL% EQU 0 (
        SET "VBOX_PATH=VBoxManage"
    ) ELSE (
        COLOR 0C
        echo [ERROR] Oracle VirtualBox was not found at standard paths!
        echo Please ensure Oracle VirtualBox is installed.
        pause
        exit /b 1
    )
)

SET "VM_NAME=CYBERLAB-MicroOS"
SET "ISO_PATH=%~dp0micro-os\build\cyberlab_microos.iso"

IF NOT EXIST "%ISO_PATH%" (
    COLOR 0C
    echo [ERROR] Bootable ISO not found at:
    echo "%ISO_PATH%"
    echo Please rebuild the ISO before launching.
    pause
    exit /b 1
)

echo [INFO] Verifying VirtualBox VM registration...
"%VBOX_PATH%" list vms | findstr /i "\"%VM_NAME%\"" >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
    echo [INFO] VM "%VM_NAME%" not found. Creating and provisioning VM...
    "%VBOX_PATH%" createvm --name "%VM_NAME%" --ostype "Linux_64" --register
    "%VBOX_PATH%" modifyvm "%VM_NAME%" --memory 2048 --cpus 2 --vram 128 --graphicscontroller vboxsvga --boot1 dvd --boot2 disk --boot3 none --nic1 nat
    "%VBOX_PATH%" storagectl "%VM_NAME%" --name "IDE Controller" --add ide
    "%VBOX_PATH%" storageattach "%VM_NAME%" --storagectl "IDE Controller" --port 0 --device 0 --type dvddrive --medium "%ISO_PATH%"
    echo [INFO] VM provisioned successfully.
) ELSE (
    echo [INFO] VirtualBox VM "%VM_NAME%" is already registered and configured.
)

echo.
echo [INFO] Starting "%VM_NAME%" in VirtualBox GUI mode...
"%VBOX_PATH%" startvm "%VM_NAME%" --type gui

IF %ERRORLEVEL% EQU 0 (
    COLOR 0A
    echo.
    echo =========================================================================
    echo [SUCCESS] CYBERLAB MicroOS has launched in VirtualBox!
    echo Look at your VirtualBox window: the desktop is booting up!
    echo =========================================================================
) ELSE (
    COLOR 0C
    echo [WARNING] VirtualBox reported code %ERRORLEVEL%. Check VirtualBox Manager.
)

echo.
pause
