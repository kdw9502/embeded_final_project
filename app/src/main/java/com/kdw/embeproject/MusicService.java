package com.kdw.embeproject;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.media.MediaPlayer;

public class MusicService extends Service {
    MediaPlayer mp;

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        return null;
    }
    @Override
    public void onCreate() {
        super.onCreate();
    }
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        String song = intent.getExtras().getString("song");

        if (song.equals("cannon"))
            mp = MediaPlayer.create(this, R.raw.canon);
        else if (song.equals("bee"))
            mp = MediaPlayer.create(this, R.raw.bee);
        else if (song.equals("march"))
            mp = MediaPlayer.create(this, R.raw.march);

        mp.setLooping(false);

        mp.start();
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        mp.stop();
    }
}
