call android update project --path . --target android-21
call ant debug
adb logcat -c
adb logcat | grep Scene3D