package com.tomahawkrobotics.kinesis.plugins.salesdemo;

import android.content.Intent;
import android.graphics.Typeface;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TableLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.util.HashMap;
import java.util.Objects;
public class KinesisH4HActivity extends AppCompatActivity {
    /**
     * Log Tag
     */
    public static final String TAG = KinesisH4HActivity.class.getSimpleName();

    public static KinesisDemoService mDemoService = null;
    /**
     * Handle to the kinesis test service
     */
    private boolean mServiceStarted = false;

    int numUxv = 5;

    public static class uxv {
        public int platformID;
        public String platformType;
        public String platformName;
        public String platformIP;
        public String platformControlPort;
        public boolean platformStarted;
        public TextView status;
        //        public String isHost;
        public String platformStatusPort;
        public String camURL;

        uxv(int ID)  {
            platformID = ID;
        }
    }

    uxv p1 = null;
    uxv p2 = null;
    uxv p3 = null;
    uxv p4 = null;
    uxv p5 = null;
    uxv p6 = null;
    uxv p7 = null;
    uxv p8 = null;

    /**
     * Start the plugin if it's not already started
     */
//    public synchronized void startPlugin() {
//        if (!mServiceStarted) {
//            Intent serviceStartIntent = new Intent(getApplicationContext(), KinesisDemoService.class);
//            serviceStartIntent.setAction(KinesisDemoService.ACTION_H4H);
//            getApplicationContext().startService(serviceStartIntent);
//            mServiceStarted = true;
//        }
//    }

    public void startTest(uxv str) {
        toast("node starting: " + str.platformName);
//        mDemoService.startH4HVehicle(str.platformName);
    }

    public void startUxV(uxv platform) {
        if (!platform.platformStarted) {
            updateParameters();
            Log.d(TAG, "platformName: " +platform.platformName);
            if (Objects.equals(platform.platformName, "")) {
                Log.d(TAG, "platformName == empty");
                switch (platform.platformID) {
                    case 1:
                        findViewById(R.id.platform1Name).requestFocus();
                        break;
                    case 2:
                        findViewById(R.id.platform2Name).requestFocus();
                        break;
                    case 3:
                        findViewById(R.id.platform3Name).requestFocus();
                        break;
                    case 4:
                        findViewById(R.id.platform4Name).requestFocus();
                        break;
                    case 5:
                        findViewById(R.id.platform5Name).requestFocus();
                        break;
                    case 6:
                        findViewById(R.id.platform6Name).requestFocus();
                        break;
                    case 7:
                        findViewById(R.id.platform7Name).requestFocus();
                        break;
                    case 8:
                        findViewById(R.id.platform8Name).requestFocus();
                        break;
                }
                return;
            }

            if (mDemoService != null) {
                mDemoService.startH4HVehicle(platform.platformName, platform.platformType, platform.platformControlPort, platform.platformStatusPort, platform.camURL);

                toast("platform started");
                platform.platformStarted = true;
                platform.status.setText(R.string.started);
                platform.status.setTypeface(Typeface.DEFAULT_BOLD);
            }

            else {
                toast("Kinesis Service not running");
            }

        }
    }

    /**
     * Stop the plugin if it's already started
     */
//    public synchronized void stopPlugin() {
//        if (mServiceStarted) {
//            Intent serviceStartIntent = new Intent(getApplicationContext(), KinesisDemoService.class);
//            serviceStartIntent.setAction(KinesisDemoService.ACTION_STOP);
//            getApplicationContext().startService(serviceStartIntent);
//            mServiceStarted = false;
//        }
//    }

    public synchronized void stopUxv(uxv platform) {
        if (platform.platformStarted) {
            Log.d(TAG, "this will stop this node. right...?");
//            TODO: fix this
//            Intent serviceStopIntent = new Intent(getApplicationContext(), KinesisDemoService.class);
//            serviceStopIntent.setAction(KinesisDemoService.ACTION_STOP_NODES);
//            getApplicationContext().startService(serviceStopIntent);
//            platform.platformStarted = false;
//            platform.status.setText(R.string.waiting);
        }
    }

    private void updateParameters(){
        for (int i=1; i <9; i++) {
            String status_port;
            switch (i) {
                case 1:
                    p1.platformName = ((EditText) findViewById(R.id.platform1Name)).getText().toString();
                    p1.platformIP = ((EditText) findViewById(R.id.platform1IP)).getText().toString();
                    p1.platformControlPort = "5551";
                    status_port = ((EditText) findViewById(R.id.platform1StatusPort)).getText().toString();
                    p1.platformStatusPort = p1.platformIP + ":" + status_port;
                    p1.status = findViewById(R.id.platform1Status);
                    p1.platformType = (String) ((Spinner) findViewById(R.id.platform1Spinner)).getSelectedItem();
                    p1.camURL = ((EditText) findViewById(R.id.platform1camURL)).getText().toString();
                    Log.d(TAG, "platform type: " + p1.platformType);
                    break;
                case 2:
                    p2.platformName = ((EditText) findViewById(R.id.platform2Name)).getText().toString();
                    p2.platformIP = ((EditText) findViewById(R.id.platform2IP)).getText().toString();
                    p2.platformControlPort = "5552";
                    status_port = ((EditText) findViewById(R.id.platform2StatusPort)).getText().toString();
                    p2.platformStatusPort = p2.platformIP + ":" + status_port;
                    p2.status = findViewById(R.id.platform2Status);
                    p2.platformType = (String) ((Spinner) findViewById(R.id.platform2Spinner)).getSelectedItem();
                    p2.camURL = ((EditText) findViewById(R.id.platform2camURL)).getText().toString();
                    break;
                case 3:
                    p3.platformName = ((EditText) findViewById(R.id.platform3Name)).getText().toString();
                    p3.platformIP = ((EditText) findViewById(R.id.platform3IP)).getText().toString();
                    p3.platformControlPort = "5553";
                    status_port = ((EditText) findViewById(R.id.platform3StatusPort)).getText().toString();
                    p3.platformStatusPort = p3.platformIP + ":" + status_port;
                    p3.status = findViewById(R.id.platform3Status);
                    p3.platformType = (String) ((Spinner) findViewById(R.id.platform3Spinner)).getSelectedItem();
                    p3.camURL = ((EditText) findViewById(R.id.platform3camURL)).getText().toString();
                    break;
                case 4:
                    p4.platformName = ((EditText) findViewById(R.id.platform4Name)).getText().toString();
                    p4.platformIP = ((EditText) findViewById(R.id.platform4IP)).getText().toString();
                    p4.platformControlPort = "5554";
                    status_port = ((EditText) findViewById(R.id.platform4StatusPort)).getText().toString();
                    p4.platformStatusPort = p4.platformIP + ":" + status_port;
                    p4.status = findViewById(R.id.platform4Status);
                    p4.platformType = (String) ((Spinner) findViewById(R.id.platform4Spinner)).getSelectedItem();
                    p4.camURL = ((EditText) findViewById(R.id.platform4camURL)).getText().toString();
                    break;
                case 5:
                    p5.platformName = ((EditText) findViewById(R.id.platform5Name)).getText().toString();
                    p5.platformIP = ((EditText) findViewById(R.id.platform5IP)).getText().toString();
                    p5.platformControlPort = "5555";
                    status_port = ((EditText) findViewById(R.id.platform5StatusPort)).getText().toString();
                    p5.platformStatusPort = p5.platformIP + ":" + status_port;
                    p5.status = findViewById(R.id.platform5Status);
                    p5.platformType = (String) ((Spinner) findViewById(R.id.platform5Spinner)).getSelectedItem();
                    p5.camURL = ((EditText) findViewById(R.id.platform5camURL)).getText().toString();
                    break;
                case 6:
                    p6.platformName = ((EditText) findViewById(R.id.platform6Name)).getText().toString();
                    p6.platformIP = ((EditText) findViewById(R.id.platform6IP)).getText().toString();
                    p6.platformControlPort = "5556";
                    status_port = ((EditText) findViewById(R.id.platform6StatusPort)).getText().toString();
                    p6.platformStatusPort = p6.platformIP + ":" + status_port;
                    p6.status = findViewById(R.id.platform6Status);
                    p6.platformType = (String) ((Spinner) findViewById(R.id.platform6Spinner)).getSelectedItem();
                    p6.camURL = ((EditText) findViewById(R.id.platform6camURL)).getText().toString();
                    break;
                case 7:
                    p7.platformName = ((EditText) findViewById(R.id.platform7Name)).getText().toString();
                    p7.platformIP = ((EditText) findViewById(R.id.platform7IP)).getText().toString();
                    p7.platformControlPort = "5557";
                    status_port = ((EditText) findViewById(R.id.platform7StatusPort)).getText().toString();
                    p7.platformStatusPort = p7.platformIP + ":" + status_port;
                    p7.status = findViewById(R.id.platform7Status);
                    p7.platformType = (String) ((Spinner) findViewById(R.id.platform7Spinner)).getSelectedItem();
                    p7.camURL = ((EditText) findViewById(R.id.platform7camURL)).getText().toString();
                    break;
                case 8:
                    p8.platformName = ((EditText) findViewById(R.id.platform8Name)).getText().toString();
                    p8.platformIP = ((EditText) findViewById(R.id.platform8IP)).getText().toString();
                    p8.platformControlPort = "5558";
                    status_port = ((EditText) findViewById(R.id.platform8StatusPort)).getText().toString();
                    p8.platformStatusPort = p8.platformIP + ":" + status_port;
                    p8.status = findViewById(R.id.platform8Status);
                    p8.platformType = (String) ((Spinner) findViewById(R.id.platform8Spinner)).getSelectedItem();
                    p8.camURL = ((EditText) findViewById(R.id.platform8camURL)).getText().toString();
                    break;

            }
        }
//        return params;
    }

//    private int getNumRunning() {
//        int count = 0;
//        if (p1.status.getText() == getString(R.string.started)){
//            count++;
//        }
//        if (p2.status.getText() == getString(R.string.started)){
//            count++;
//        }
//        if (p3.status.getText() == getString(R.string.started)){
//            count++;
//        }
//        return count;
//    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.v(TAG, "Lifecycle:onCreate called");

        // Setup the View
        setContentView(R.layout.h4h_activity);

        p1 = new uxv(1);
        p2 = new uxv(2);
        p3 = new uxv(3);
        p4 = new uxv(4);
        p5 = new uxv(5);
        p6 = new uxv(6);
        p7 = new uxv(7);
        p8 = new uxv(8);

        Button p1Start = findViewById(R.id.platform1StartButton);
        Button p2Start = findViewById(R.id.platform2StartButton);
        Button p3Start = findViewById(R.id.platform3StartButton);
        Button p4Start = findViewById(R.id.platform4StartButton);
        Button p5Start = findViewById(R.id.platform5StartButton);
        Button p6Start = findViewById(R.id.platform6StartButton);
        Button p7Start = findViewById(R.id.platform7StartButton);
        Button p8Start = findViewById(R.id.platform8StartButton);
        Button addUxv = findViewById(R.id.addUxvButton);

        Spinner spinner1 = findViewById(R.id.platform1Spinner);
        ArrayAdapter<CharSequence> adapter1 = ArrayAdapter.createFromResource(this, R.array.uxv_types, android.R.layout.simple_spinner_item);
        spinner1.setAdapter(adapter1);
        spinner1.setSelection(adapter1.getPosition("UAV"));

        Spinner spinner2 = findViewById(R.id.platform2Spinner);
        ArrayAdapter<CharSequence> adapter2 = ArrayAdapter.createFromResource(this, R.array.uxv_types, android.R.layout.simple_spinner_item);
        spinner2.setAdapter(adapter2);
        spinner2.setSelection(adapter2.getPosition("UGV"));

        Spinner spinner3 = findViewById(R.id.platform3Spinner);
        ArrayAdapter<CharSequence> adapter3 = ArrayAdapter.createFromResource(this, R.array.uxv_types, android.R.layout.simple_spinner_item);
        spinner3.setAdapter(adapter3);
        spinner3.setSelection(adapter3.getPosition("Manipulator"));

        Spinner spinner4 = findViewById(R.id.platform4Spinner);
        ArrayAdapter<CharSequence> adapter4 = ArrayAdapter.createFromResource(this, R.array.uxv_types, android.R.layout.simple_spinner_item);
        spinner4.setAdapter(adapter4);
        spinner4.setSelection(adapter4.getPosition("Quadruped"));

        Spinner spinner5 = findViewById(R.id.platform5Spinner);
        ArrayAdapter<CharSequence> adapter5 = ArrayAdapter.createFromResource(this, R.array.uxv_types, android.R.layout.simple_spinner_item);
        spinner5.setAdapter(adapter5);
        spinner1.setSelection(adapter1.getPosition("UGV"));

        Spinner spinner6 = findViewById(R.id.platform6Spinner);
        ArrayAdapter<CharSequence> adapter6 = ArrayAdapter.createFromResource(this, R.array.uxv_types, android.R.layout.simple_spinner_item);
        spinner6.setAdapter(adapter6);

        Spinner spinner7 = findViewById(R.id.platform7Spinner);
        ArrayAdapter<CharSequence> adapter7 = ArrayAdapter.createFromResource(this, R.array.uxv_types, android.R.layout.simple_spinner_item);
        spinner7.setAdapter(adapter7);

        Spinner spinner8 = findViewById(R.id.platform8Spinner);
        ArrayAdapter<CharSequence> adapter8 = ArrayAdapter.createFromResource(this, R.array.uxv_types, android.R.layout.simple_spinner_item);
        spinner8.setAdapter(adapter8);

        Button p1Stop = findViewById(R.id.platform1StopButton);
        Button p2Stop= findViewById(R.id.platform2StopButton);
        Button p3Stop = findViewById(R.id.platform3StopButton);
        Button p4Stop = findViewById(R.id.platform4StopButton);
        Button p5Stop = findViewById(R.id.platform5StopButton);
        Button p6Stop = findViewById(R.id.platform6StopButton);
        Button p7Stop = findViewById(R.id.platform7StopButton);
        Button p8Stop = findViewById(R.id.platform8StopButton);

        HashMap<Integer, TableLayout> list_tables = new HashMap<>();

        list_tables.put(1, findViewById(R.id.platform1Table));
        list_tables.put(2, findViewById(R.id.platform2Table));
        list_tables.put(3, findViewById(R.id.platform3Table));
        list_tables.put(4, findViewById(R.id.platform4Table));
        list_tables.put(5, findViewById(R.id.platform5Table));
        list_tables.put(6, findViewById(R.id.platform6Table));
        list_tables.put(7, findViewById(R.id.platform7Table));
        list_tables.put(8, findViewById(R.id.platform8Table));

        addUxv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                TableLayout new_uxv = list_tables.get(numUxv+1);
                new_uxv.setVisibility(View.VISIBLE);
                numUxv++;
            }
        });


        p1Start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateParameters();
                startUxV(p1);
            }
        });

        p2Start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startUxV(p2);
            }
        });

        p3Start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startUxV(p3);
            }
        });

        p4Start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startUxV(p4);
            }
        });

        p5Start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startUxV(p5);
            }
        });

        p6Start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startUxV(p6);
            }
        });

        p7Start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startUxV(p7);
            }
        });

        p8Start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startUxV(p8);
            }
        });

        p1Stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                updateParameters();
                toast("PLATFORM: " + p1.platformName);
//                stopUxv(p1);
            }
        });

        p2Stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopUxv(p2);
            }
        });

        p3Stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopUxv(p3);
            }
        });

        p4Stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopUxv(p4);
            }
        });

        p5Stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopUxv(p5);
            }
        });

        p6Stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopUxv(p6);
            }
        });

        p7Stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopUxv(p7);
            }
        });

        p8Stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopUxv(p8);
            }
        });

    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.v(TAG, "Lifecycle:onStart called");
        // Rather than buttons you could tie it to the lifecycle
        // onStart matches with onStop
        // startPlugin();
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.v(TAG, "Lifecycle:onResume called");
        // Rather than buttons you could tie it to the lifecycle
        // onPause matches with onResume
        // startPlugin();
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.v(TAG, "Lifecycle:onPause called");
        // Rather than buttons you could tie it to the lifecycle
        // onPause matches with onResume
        // stopPlugin();
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.v(TAG, "Lifecycle:onStop called");
        // Rather than buttons you could tie it to the lifecycle
        // onStart matches with onStop
        // stopPlugin();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.v(TAG, "Lifecycle:onDestroy called");
    }

    private void toast(String str) {
        Toast.makeText(getApplicationContext(), str, Toast.LENGTH_LONG).show();
    }

    public static void setService(KinesisDemoService service) {
        mDemoService = service;
    }

}
