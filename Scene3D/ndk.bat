cls
call ndk-build -j
cp libs/armeabi-v7a/libScene3D.so ../../HiARSDK/HiSceneARCamera/libs/armeabi-v7a/
echo "Please rebuild and install APK"
adb logcat -c
adb logcat | grep Scene3D