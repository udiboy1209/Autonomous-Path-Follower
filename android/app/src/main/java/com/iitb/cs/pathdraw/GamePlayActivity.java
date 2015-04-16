package com.iitb.cs.pathdraw;

import android.app.Activity;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager.LayoutParams;

public class GamePlayActivity extends Activity {
    DisplayPanel display;

    @Override
    public void onCreate(Bundle icicle){
        super.onCreate(icicle);
        display=new DisplayPanel(this, null);

        setContentView(display);
    }
}