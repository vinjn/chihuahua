cls
call ndk-build -j
cp -r ./libs/armeabi-v7a ../examples/AndroidTemplate/libs/
echo "Please rebuild and install APK"
adb logcat -c
adb logcat | grep Scene3D