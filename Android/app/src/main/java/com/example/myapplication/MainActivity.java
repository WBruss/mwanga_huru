package com.example.myapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

    EditText editIPAddress;
    EditText editPort;
    TextView textIPAddress;
    TextView textPort;
    TextView textResponse;
    Button buttonSet;
    Button buttonOn;
    Button buttonOff;
    String TAG = "MainActivity";
    String response = "NULL";
    String ipAddress = "NULL";
    int port = 0;
    String status;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editIPAddress = findViewById(R.id.etIPAddress);
        editPort = findViewById(R.id.etPort);
        textIPAddress = findViewById(R.id.tvIPAddress);
        textPort = findViewById(R.id.tvPort);
        textResponse = findViewById(R.id.tvResponse);
        buttonSet = findViewById(R.id.btnSet);
        buttonOn = findViewById(R.id.btnOn);
        buttonOff = findViewById(R.id.btnOff);

        buttonSet.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ipAddress = editIPAddress.getText().toString();
                port = Integer.parseInt(editPort.getText().toString());
                textIPAddress.setText(ipAddress);
                textPort.setText(String.valueOf(port));
            }
        });

        buttonOn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                status = "on.";
                setStatus(status);
            }
        });

        buttonOff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                status = "off.";
                setStatus(status);
            }
        });
    }


    public void setStatus(final String sts){
        Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Socket s = new Socket((editIPAddress.getText().toString()),(Integer.parseInt(editPort.getText().toString())));

                    PrintWriter pw = new PrintWriter(s.getOutputStream());
                    pw.write(sts);
                    pw.flush();

                    InputStreamReader in = new InputStreamReader(s.getInputStream());
                    BufferedReader bf = new BufferedReader(in);
                    response = bf.readLine();
                    Log.i(TAG, response);


                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            textResponse.setText(response);
                        }
                    });

                    s.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        });

        t.start();

    }


}
