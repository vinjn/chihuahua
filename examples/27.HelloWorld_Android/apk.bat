call android update project --path . --target android-21
call ant debug
adb install -r bin/NativeActivity-debug.apk
adb logcat -c
adb logcat | grep Scene3D