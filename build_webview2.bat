@echo off
setlocal
cd /d "%~dp0"
set "PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%"
if not exist "C:\msys64\ucrt64\bin\g++.exe" (
 echo ERROR: MSYS2 UCRT64 g++ not found.
 pause
 exit /b 1
)
echo [1/3] Building C++ Trie backend...
"C:\msys64\ucrt64\bin\g++.exe" -std=c++17 -O2 -Iinclude src\WebServer.cpp src\Trie.cpp src\FileManager.cpp src\SmartReview.cpp -o LexiTrieBackend.exe -lws2_32 -lbcrypt -static-libgcc -static-libstdc++
if errorlevel 1 (
 echo ERROR: C++ backend build failed.
 pause
 exit /b 1
)
echo [2/3] Starting C++ backend...
start "LexiTrie C++ Backend" /min "%~dp0LexiTrieBackend.exe"
C:\Windows\System32\timeout.exe /t 2 /nobreak >nul
where dotnet >nul 2>nul
if errorlevel 1 goto browser
 echo [3/3] Building WebView2 desktop window (requires .NET 8 SDK + internet on first run)...
 dotnet build WebViewHost\WebViewHost.csproj -c Release -o WebViewHost\publish --nologo
 if errorlevel 1 goto browser
 start "" "%~dp0WebViewHost\publish\WebViewHost.exe"
 goto done
:browser
 echo WebView2 host requires .NET 8 SDK and Microsoft.Web.WebView2 NuGet package.
 echo Opening browser fallback with the SAME C++ Trie backend...
 start "" "http://127.0.0.1:18765/"
:done
 echo If the app does not open, visit http://127.0.0.1:18765/
 echo To stop the backend: taskkill /IM LexiTrieBackend.exe /F
