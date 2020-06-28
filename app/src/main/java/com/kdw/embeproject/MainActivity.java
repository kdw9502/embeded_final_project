package com.kdw.embeproject;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void ChangeActivity(View v)
    {
        Intent intent = new Intent(MainActivity.this, InGameActivity.class);
        String song = v.getTag().toString();
        intent.putExtra("song",song);

        startActivity(intent);
    }
}
