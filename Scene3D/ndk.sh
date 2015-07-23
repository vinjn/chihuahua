clear
ndk-build -j
cp libs/armeabi-v7a/libIrrlicht.so ../../HiARSDK/HiSceneARCamera/libs/armeabi-v7a/
echo "Please rebuild and install APK"
adb logcat -c
adb logcat | grep Irrlicht