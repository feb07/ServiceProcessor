//
// Created by 李利椿 on 2019/2/20.
//

#ifndef SERVICEPROCESSOR_NATIVE_LIB_H
#define SERVICEPROCESSOR_NATIVE_LIB_H

#endif //SERVICEPROCESSOR_NATIVE_LIB_H

#include <zconf.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <android/log.h>

#include <jni.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <android/log.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/signal.h>
#include <android/log.h>
#include <stdio.h>


#define LOG_TAG "feb"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

void child_do_work();

void child_listen_msg();

int child_create_channel();