@echo off
title LexiTrie Launcher
cd /d "%~dp0"

set "PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%"

if not exist "LexiTrieBackend.exe" (
    echo LOI: Khong tim thay backend!
    pause
    exit /b 1
)

echo Dang khoi dong LexiTrie...

start "LexiTrie Backend" cmd /k "cd /d ""%~dp0"" && LexiTrieBackend.exe"

timeout /t 4 /nobreak >nul

start "" "http://127.0.0.1:18765"
