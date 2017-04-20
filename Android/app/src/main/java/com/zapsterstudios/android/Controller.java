package com.zapsterstudios.android;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Vibrator;
import android.util.Log;
import android.view.View;
import android.view.MotionEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.Toast;
import android.widget.Button;
import android.widget.SeekBar;
import android.content.Intent;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.bluetooth.BluetoothSocket;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.os.AsyncTask;
import java.io.IOException;
import java.util.UUID;

public class Controller extends AppCompatActivity {

    // Bluetooth
    String address = null;
    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;

    // Bluetooth SPP UUID
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    // Elements
    private SeekBar movementSpeed;

    private Button btnForward;
    private Button btnBackwards;
    private Button btnTurnLeft;
    private Button btnTurnRight;

    private Button btnUp;
    private Button btnDown;
    private Button btnRotateLeft;
    private Button btnRotateRight;

    final Context context = this;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // On created
        super.onCreate(savedInstanceState);

        // Retrieve address
        Intent newint = getIntent();
        address = newint.getStringExtra(DeviceList.EXTRA_ADDRESS);

        // Create view
        setContentView(R.layout.activity_controller);

        // Connect to Bluetooth
        new ConnectBT().execute();

        // Alive
        final Handler h = new Handler();
        h.postDelayed(new Runnable()
        {
            private long time = 0;

            @Override
            public void run()
            {
                time += 1000;
                sendBluetoothSignal("X");
                h.postDelayed(this, 1000);
            }
        }, 1000);

        // Seekbar - Movement Speed
        movementSpeed = (SeekBar) findViewById(R.id.movementSpeed);
        movementSpeed.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                sendBluetoothSignal(Integer.toString(progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        // Button - Forward
        btnForward = (Button) findViewById(R.id.btnForward);
        btnForward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                handleToggleButton(event, "W", "w");
                return false;
            }
        });

        // Button - Backwards
        btnBackwards = (Button) findViewById(R.id.btnBackwards);
        btnBackwards.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                handleToggleButton(event, "S", "s");
                return false;
            }
        });

        // Button - Turn Left
        btnTurnLeft = (Button) findViewById(R.id.btnTurnLeft);
        btnTurnLeft.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                handleToggleButton(event, "A", "a");
                return false;
            }
        });

        // Button - Turn Right
        btnTurnRight = (Button) findViewById(R.id.btnTurnRight);
        btnTurnRight.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                handleToggleButton(event, "D", "d");
                return false;
            }
        });

        // Button - Up
        btnUp = (Button) findViewById(R.id.btnUp);
        btnUp.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                handleToggleButton(event, "T", "t");
                return false;
            }
        });

        // Button - Down
        btnDown = (Button) findViewById(R.id.btnDown);
        btnDown.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                handleToggleButton(event, "G", "g");
                return false;
            }
        });

        // Button - Rotate Left
        btnRotateLeft = (Button) findViewById(R.id.btnRotateLeft);
        btnRotateLeft.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                handleToggleButton(event, "F", "f");
                return false;
            }
        });

        // Button - Rotate Right
        btnRotateRight = (Button) findViewById(R.id.btnRotateRight);
        btnRotateRight.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                handleToggleButton(event, "H", "h");
                return false;
            }
        });
    }

    private void handleToggleButton(MotionEvent event, String signalDown, String signalUp) {
        // Detect action
        if(event.getAction() == MotionEvent.ACTION_DOWN) {
            // Send down signal
            sendBluetoothSignal(signalDown);

            // Vibrate
            Vibrator v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
            v.vibrate(60);
        } else if(event.getAction() == MotionEvent.ACTION_UP) {
            // Send up signal
            sendBluetoothSignal(signalUp);

            // Vibrate
            Vibrator v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
            v.vibrate(30);
        }
    }

    private void sendBluetoothSignal(String signal) {
        if(btSocket != null)
        {
            try
            {
                btSocket.getOutputStream().write(signal.toString().getBytes());
            } catch (IOException e) {
                //Toast.makeText(getApplicationContext(), "Couldn't send signal.", Toast.LENGTH_LONG).show();
            }
        }
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
            if(btSocket != null) {
                try {
                    btSocket.close();
                } catch(IOException e) {
                    Toast.makeText(getApplicationContext(), "Couldn't disconnect...", Toast.LENGTH_LONG).show();
                }
            }

            finish();
        }

        // Kill
        if(selectID == R.id.action_kill) {
            // Open dialog
            AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(context);
            alertDialogBuilder
                .setCancelable(false)
                .setTitle("Kill Switch")
                .setMessage("Are you sure you want the drone to drop dead?")
                .setPositiveButton("Yes, kill it", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        sendBluetoothSignal("Q");
                    }
                })
                .setNegativeButton("No", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                }).show();

            AlertDialog alertDialog = alertDialogBuilder.create();
            alertDialog.show();
        }

        // Return default
        return super.onOptionsItemSelected(item);
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute()
        {
            progress = ProgressDialog.show(Controller.this, "Connecting...", "Please wait!!!");  //show a progress dialog
        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try
            {
                if (btSocket == null || !isBtConnected)
                {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);//connects to the device's address and checks if it's available
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                Toast.makeText(getApplicationContext(), "Connection Failed. Attempt to repair!", Toast.LENGTH_LONG).show();
                finish();
            }
            else
            {
                Toast.makeText(getApplicationContext(), "Connected!", Toast.LENGTH_LONG).show();
                isBtConnected = true;
            }

            progress.dismiss();
        }
    }
}
