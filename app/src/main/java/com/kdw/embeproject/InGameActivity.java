package com.kdw.embeproject;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Debug;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.ToggleButton;

public class InGameActivity extends AppCompatActivity {
    static{
        System.loadLibrary("fpga-button");
    }
    public native int GetPressedButtons();
    public native void SetScore(int score);
    private String song;
    private int delay = 1500;
    @SuppressLint("HandlerLeak")
    Handler random_handler =new Handler(){
        public void handleMessage(Message msg){
            int index = msg.arg1;
            for (int i = 0;i<9;i++)
            {
                buttons[i].setChecked(false);
            }
            buttons[index].setChecked(true);
        }
    };

    @SuppressLint("HandlerLeak")
    Handler input_handler =new Handler(){
        public void handleMessage(Message msg){
            int index = msg.arg1 - 1;
            if(index == -1 || index>9)
                return;

            if(buttons[index].isChecked())
            {
                buttons[index].setChecked(false);
                score += 100;
            }
            else {
                score -= 20;
            }
            SetScore(score);
        }
    };

    class BackThread extends Thread{
        Handler handler;

        BackThread(Handler handler){
            this.handler = handler;
        }
        public void run(){
            try{Thread.sleep(4000);}catch(InterruptedException e){;}
            while(true){
                int random_number = (int)(Math.random()*9);
                Message msg=Message.obtain();
                msg.what = 0;
                msg.arg1 = random_number;
                handler.sendMessage(msg);
                try{Thread.sleep(delay);}catch(InterruptedException e){;}
            }
        }
    }

    class GetInputThread extends Thread{
        Handler handler;

        GetInputThread(Handler handler){
            this.handler = handler;
        }
        public void run(){
            try{Thread.sleep(3000);}catch(InterruptedException e){;}
            while(true){

                int button_index = GetPressedButtons();
                Message msg=Message.obtain();
                msg.what = 1;
                msg.arg1 = button_index;
                handler.sendMessage(msg);

                try{Thread.sleep(300);}catch(InterruptedException e){;}
            }
        }
    }
    GetInputThread inputThread;
    BackThread backThread;
    ToggleButton[] buttons = new ToggleButton[9];
    int score = 1000;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_in_game);

        Intent intent = getIntent();
        this.song = intent.getExtras().getString("song");

        if (song.equals("bee"))
        {
            delay = 300;
        }
        else if(song.equals("march"))
        {
            delay = 600;
        }

        buttons[0] = findViewById(R.id.toggleButton1);
        buttons[1] = findViewById(R.id.toggleButton2);
        buttons[2] = findViewById(R.id.toggleButton3);
        buttons[3] = findViewById(R.id.toggleButton4);
        buttons[4] = findViewById(R.id.toggleButton5);
        buttons[5] = findViewById(R.id.toggleButton6);
        buttons[6] = findViewById(R.id.toggleButton7);
        buttons[7] = findViewById(R.id.toggleButton8);
        buttons[8] = findViewById(R.id.toggleButton9);

        for (int i=0;i<9;i++)
            buttons[i].setEnabled(false);

        SetScore(score);
        backThread =new BackThread(random_handler);
        backThread.setDaemon(true);
        backThread.start();

        inputThread = new GetInputThread(input_handler);
        inputThread.setDaemon(true);
        inputThread.start();
        intent = new Intent(getApplicationContext(), MusicService.class);
        intent.putExtra("song",song);
        startService(intent);

    }
}
