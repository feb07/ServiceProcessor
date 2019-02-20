package com.feb.serviceprocessor;

/**
 * Created by lilichun on 2019/2/20.
 */
public class Watcher {
    static {
        System.loadLibrary("native-lib");
    }
    public native void createWatcher(String userId);
    public native void connectMonitor();
    public native void forkTest();
}
