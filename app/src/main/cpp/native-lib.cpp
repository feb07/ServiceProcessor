#include <jni.h>
#include <string>
#include "native_lib.h"

const char *PATH = "data/data/com.feb.serviceprocessor/my.socket";
int m_child;
const char *userId;

/**
 * java socket示例
 * 一、搭建服务端
 * a)创建ServerSocket对象绑定监听端口
 * b)通过accept()方法监听客户端请求
 * c)建立连接后，通过输入输出流读取客户端发送的请求信息
 * d)通过输出流想客户端发送请求信息
 * e)关闭相关资源
 *try {
            ServerSocket serverSocket = new ServerSocket(5209);
            Socket socket = serverSocket.accept();
            String line = null;
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            do {
                line = in.readLine();
            } while (in.read() != -1);
            socket.close();
            serverSocket.close();

        } catch (IOException e) {
            e.printStackTrace();
        }

    二、客户端
    try {
            Socket socketClient = new Socket("10.10.2.246",5209);
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(socketClient.getInputStream()));
            socketClient.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
 */

/**
 * LINUX socket
 * 一、开启socket 等待连接
 * 二、读取消息
 */
void child_do_work() {
//开启socket
    int result = child_create_channel();
    if (result) {
        child_listen_msg();
    }

}

/**
 * 服务端读取信息
 */
void child_listen_msg() {
    fd_set rfds;
    struct timeval timeout = {3, 0};
    while (1) {
        //清空内容
        FD_ZERO(&rfds);
        //设值需要读取客户端
        FD_SET(m_child, &rfds);
        int r = select(m_child + 1, &rfds, NULL, NULL, &timeout);
        //读取消息前
        if (r > 0) {
            //缓冲区
            char pkg[256] = {0};
            //保证所读到的信息是指定客户端的
            if (FD_ISSET(m_child, &rfds)) {
                //read 阻塞式函数
                int result = read(m_child, pkg, sizeof(pkg));
                //开启服务
                execlp("am", "am", "startservice", "--user", userId,
                       "com.feb.serviceprocessor/com.feb.serviceprocessor.ProcessService",
                       (char *) NULL);
                break;
            }
        }

    }

}

/**
 * 创建服务端socket
 * 跨进程：基于文件读写
 * @return
 */
int child_create_channel() {
    LOGE("开始创建服务端socket");
    //文件
    int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    unlink(PATH);
    //内存区域
    struct sockaddr_un addr;
    //清空内存
    memset(&addr, 0, sizeof(sockaddr_un));
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, PATH);
    if ((bind(listenfd, reinterpret_cast<const sockaddr *>(&addr), sizeof(sockaddr_un))) < 0) {
        //绑定错误
        LOGE("服务端绑定socket错误");
        return 0;
    }
    listen(listenfd, 5);
    int connfd = 0;
    //保证宿主进程连接成功
    while (1) {
        //返回值  accept： 阻塞式函数
        if ((connfd = accept(listenfd, NULL, NULL)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                //读取错误
                LOGE("服务端读取错误");
                return 0;
            }
        }
        m_child = connfd;
        //连接成功
        LOGE("服务端连接成功");
        break;
    }
    return 1;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_feb_serviceprocessor_Watcher_createWatcher(JNIEnv *env, jobject instance,
                                                    jstring userId_) {
    userId = env->GetStringUTFChars(userId_, 0);

    //开双进程
    pid_t pid = fork();
    if (pid < 0) {
        LOGE("无效进程");
    } else if (pid == 0) {
        //子进程 守护进程
        LOGE("子进程开启");
        child_do_work();
    } else if (pid > 0) {
        //父进程
        LOGE("父进程开启");
    }

    env->ReleaseStringUTFChars(userId_, userId);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_feb_serviceprocessor_Watcher_connectMonitor(JNIEnv *env, jobject instance) {
    int socketd;
    struct sockaddr_un addr;
    while (1) {
        //客户端开始连接
        LOGE("客户端 父进程开始连接");
        socketd = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (socketd < 0) {
            //创建失败
            LOGE("客户端创建失败");
            return;
        }
        //内存区域

        //清空内存
        memset(&addr, 0, sizeof(sockaddr_un));
        addr.sun_family = AF_LOCAL;
        strcpy(addr.sun_path, PATH);
        if (connect(socketd, reinterpret_cast<const sockaddr *>(&addr), sizeof(sockaddr_un)) < 0) {
            //连接失败
            LOGE("客户端连接失败");
            close(socketd);
            sleep(1);
            //再来下一次尝试连接
            continue;
        }
        //连接成功
        LOGE("客户端连接成功");
        break;

    }

}


extern "C"
JNIEXPORT void JNICALL
Java_com_feb_serviceprocessor_Watcher_forkTest(JNIEnv *env, jobject instance) {

    //开双进程
    pid_t pid = fork();
    if (pid < 0) {
        LOGE("无效进程");
    } else if (pid == 0) {
        //子进程 守护进程
        LOGE("子进程开启");
        child_do_work();
    } else if (pid > 0) {
        //父进程
        LOGE("父进程开启");
    }

}