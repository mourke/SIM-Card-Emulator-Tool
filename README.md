# SIM-Card-Emulator-Tool
ScET is a tool that records the live behaviour of a SIM card when connected to a network using any end user device. All SIM card communication, in the form of APDU commands are tracked and logged by this tool.


## Release new version

### Windows:

1. Update Installer/packages/com.cardcentric.scet/meta/package.xml
2. Update ScET/Version.h
3. Run Release.bat

### macOS:

1. Update ScET/Resources/Info.plist
2. Update ScET/Version.h
3. Run Release.sh
4. Copy the last line of the output (sparkle:edSignature)
5. Edit the appcast.xml file with new release information

## Release new installer (Windows only)

1. Update Installer/config/platform.xml
2. `E:\Qt\QtIFW-3.1.1\bin\binarycreator.exe -n -c Installer\config\config.xml -p Installer\packages "ScET Installer.exe"`
