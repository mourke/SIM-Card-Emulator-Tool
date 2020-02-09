~/Qt/5.14.1/clang_64/bin/qmake -makefile -Wnone ScET/ScET.pro -o ScET/GeneratedFiles/Release/Makefile -spec macx-clang CONFIG+=x86_64 CONFIG+=qtquickcompiler CONFIG+=release

cd ScET/GeneratedFiles/Release/

make

cd ..

cd ..

cd ..

~/Qt/5.14.1/clang_64/bin/macdeployqt x64/Release/ScET.app

~/Qt/Tools/QtInstallerFramework/3.2/bin/archivegen Installer/packages/com.cardcentric.scet/data/ScET.7z ./x64/Release/ScET.app

~/Qt/Tools/QtInstallerFramework/3.2/bin/repogen -v --update -p Installer/packages ~/Documents/mourke.github.io/macOS
