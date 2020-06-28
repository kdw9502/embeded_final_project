//
// Created by kdw on 2020-06-28.
//

#include <jni.h>
#include <fcntl.h>
#include <stdio.h>
#include <android/log.h>

JNIEXPORT jint JNICALL Java_com_kdw_embeproject_InGameActivity_GetPressedButtons(JNIEnv *env, jobject job)
{
    int dev;
    unsigned char pressed_button;

    dev =  open("/dev/fpga_control", O_RDWR);
    if (dev<0) {
        __android_log_print(ANDROID_LOG_DEBUG, "tag", "Device open error \n");
        return 0;
	}
    read(dev, &pressed_button, 1);
    __android_log_print(ANDROID_LOG_DEBUG, "tag", "%d", pressed_button);
    close(dev);

    return (jint)pressed_button;
}

JNIEXPORT void JNICALL Java_com_kdw_embeproject_InGameActivity_SetScore(JNIEnv *env, jobject job, jint score)
{
    int dev;
    int _score;
    unsigned char numbers[4];
    _score = (int)score;
    numbers[0] = _score%10000/1000;
    numbers[1] = _score%1000/100;
    numbers[2] = _score%100/10;
    numbers[3] = _score%10;

    dev =  open("/dev/fpga_control", O_WRONLY);
    if (dev<0) {
        __android_log_print(ANDROID_LOG_DEBUG, "tag", "Device open error \n");
        return;
    }
    write(dev, &numbers, 4);
    close(dev);

}
