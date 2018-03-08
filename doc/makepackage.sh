PACKAGE_NAME="saintstools"

rm -rf package/$PACKAGE_NAME
mkdir -p package/$PACKAGE_NAME
cp crosstex/build/*.dll package/$PACKAGE_NAME
cp libsaints/build/*.dll package/$PACKAGE_NAME
cp saintstools/cli/build/*.exe package/$PACKAGE_NAME
cp saintstools/gui/build/*.exe package/$PACKAGE_NAME
cp -r saintstools/doc/dist-licenses package/$PACKAGE_NAME/licenses

cd package/$PACKAGE_NAME
# Use saintspackgui because it contains all the dependencies
windeployqt --release saintspackgui.exe
libs=`ldd saintspackgui.exe | grep -o -P '/mingw64/.*(?= \(0x)'`
while read -r lib; do
    cp "$lib" .
done <<< "$libs"
cd ..
zip -r "$PACKAGE_NAME.zip" "$PACKAGE_NAME"
