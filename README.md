# SIM-Card-Emulator-Tool
ScET is a tool that records the live behaviour of a SIM card when connected to a network using any end user device. All SIM card communication, in the form of APDU commands are tracked and logged by this tool.


### Release new version

1. Update Installer/packages/com.cardcentric.scet/meta/package.xml
2. Update ScET/Resources/Info.plist
3. Update ScET/Version.h
4. Run Release.bat on Windows or Release.sh on macOS

## Release new installer

1. Update Installer/config/platform.xml
2. `E:\Qt\QtIFW-3.1.1\bin\binarycreator.exe -n -c Installer\config\Windows.xml -p Installer\packages "ScET Installer.exe"` on Windows or `~/Qt/Tools/QtInstallerFramework/3.2/bin/binarycreator -n -c Installer/config/macOS.xml -p Installer/packages "ScET Installer.app"`
