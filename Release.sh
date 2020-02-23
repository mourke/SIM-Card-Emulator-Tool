rm -rf x64/Release/
rm -rf ScET/x64/Release/
rm -rf ScET/GeneratedFiles/

~/Qt/5.14.1/clang_64/bin/qmake -makefile -Wnone ScET/ScET.pro -o ScET/GeneratedFiles/Release/Makefile -spec macx-clang CONFIG+=x86_64 CONFIG+=release

cd ScET/GeneratedFiles/Release/

make

cd ..

cd ..

cd ..

cd x64/Release

~/Qt/5.14.1/clang_64/bin/macdeployqt ScET.app -dmg -codesign="Apple Development"

cd ..

cd ..

mv x64/Release/ScET.dmg .

sign_update ScET.dmg
