@ECHO OFF

TITLE Update ScET Release

ECHO ============================
ECHO Compiling...
ECHO ============================
"%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe" /t:Clean,Build /p:OutDir="%HOMEPATH%\Documents\GitHub\SIM-Card-Emulator-Tool\x64\Release" /p:Configuration=Release "%HOMEPATH%\Documents\GitHub\SIM-Card-Emulator-Tool\ScET.sln"

ECHO ============================
ECHO Combining resources...
ECHO ============================
E:\Qt\5.12.0\msvc2017_64\bin\windeployqt.exe --release "%HOMEPATH%\Documents\GitHub\SIM-Card-Emulator-Tool\x64\Release\ScET.exe"
copy "%HOMEPATH%\Documents\GitHub\SIM-Card-Emulator-Tool\ScET\Resources\FileIcon.ico" "%HOMEPATH%\Documents\GitHub\SIM-Card-Emulator-Tool\x64\Release"

ECHO ============================
ECHO Packaging...
ECHO ============================
"%ProgramFiles%\7-Zip\7z.exe" a "%HOMEPATH%\Documents\GitHub\SIM-Card-Emulator-Tool\Installer\packages\com.cardcentric.scet\data\ScET.7z" "%HOMEPATH%\Documents\GitHub\SIM-Card-Emulator-Tool\x64\Release\*"
E:\Qt\QtIFW-3.1.1\bin\repogen.exe -v --update -p "%HOMEPATH%\Documents\GitHub\SIM-Card-Emulator-Tool\Installer\packages" "%HOMEPATH%\Documents\GitHub\mourke.github.io\Windows"

ECHO ============================
ECHO Done!
ECHO ============================
PAUSE
