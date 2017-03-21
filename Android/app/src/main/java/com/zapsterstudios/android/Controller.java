package com.zapsterstudios.android;

import android.content.pm.ActivityInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.WindowManager;
import android.widget.Toast;

public class Controller extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // Create view
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_controller);

        // Force landscape rotation
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Disconnect menu
        MenuInflater mi = getMenuInflater();
        mi.inflate(R.menu.menu_disconnect, menu);

        // Return default
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Variables
        int selectID = item.getItemId();

        // Disconnect
        if(selectID == R.id.action_disconnect) {
            Toast.makeText(getApplicationContext(), "Disconnect Clicked", Toast.LENGTH_SHORT).show();
        }

        // Return default
        return super.onOptionsItemSelected(item);
    }
}
