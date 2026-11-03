@echo off
setlocal
cd /d "%~dp0"
set "CXX=C:\msys64\ucrt64\bin\g++.exe"
if not exist "%CXX%" set "CXX=g++"
echo [1/3] Building C++ backend...
"%CXX%" -std=c++17 -O2 -Iinclude src\WebServer.cpp src\Trie.cpp src\FileManager.cpp src\SmartReview.cpp -o LexiTrieBackend.exe -lws2_32 -lbcrypt
if errorlevel 1 (echo C++ build failed. Check MSYS2 UCRT64. & pause & exit /b 1)
echo [2/3] Building WebView2 host...
dotnet build WebViewHost\WebViewHost.csproj -c Release
if errorlevel 1 (echo WebView2 build failed. Check .NET SDK. & pause & exit /b 1)
echo [3/3] Starting backend and WebView2...
start "LexiTrie Backend" cmd /k "cd /d ""%~dp0"" && LexiTrieBackend.exe"
timeout /t 3 /nobreak >nul
start "" "WebViewHost\bin\Release\net8.0-windows\WebViewHost.exe"
echo Open http://127.0.0.1:18765 if the window does not appear.
endlocal
