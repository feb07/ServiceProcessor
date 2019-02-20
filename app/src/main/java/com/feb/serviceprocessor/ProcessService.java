package com.feb.serviceprocessor;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Process;
import android.support.annotation.Nullable;
import android.util.Log;

import java.util.Timer;
import java.util.TimerTask;


/**
 * Created by lilichun on 2019/2/20.
 */
public class ProcessService extends Service {
    private int i;

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Watcher watcher = new Watcher();
        watcher.createWatcher(String.valueOf(Process.myUid()));
        watcher.connectMonitor();
        Timer timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                Log.e("ProcessService", "服务开启中" + i);
                i++;
            }
        }, 0, 1000 * 3);
    }
}
