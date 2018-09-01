package com.example.perec.robokot;

import android.annotation.SuppressLint;
import android.net.Uri;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.common.api.GoogleApiClient;

import com.jmedeisis.bugstick.Joystick;
import com.jmedeisis.bugstick.JoystickListener;

import android.net.wifi.WifiManager;
import android.text.format.Formatter;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import android.os.Build;
import java.net.InetAddress;

public class MainActivity extends AppCompatActivity {


    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;
    UDP_Client udpClient = new UDP_Client();
    TextView view;

    String getCommand(float degrees, float offset) {
        String res = "";
        if (degrees < 135 && degrees > 45) {
            res = "f";
        }
        if (degrees > 135 || degrees < -135) {
            res = "l";
        }
        if (degrees < 45 && degrees > -45) {
            res = "r";
        }
        if (degrees < -45 && degrees > -135) {
            res = "b";
        }
        if (offset < 0.1) {
            res = "s";
        }

        int speed = (int) (offset*10);
        if (speed > 9) {speed = 9;}
        res += speed;
        return res;
    }

    String getText(float degrees, float offset) {
        String res = "Degrees: " + degrees + ";    Offset: " + offset;
        res += "\n " + getCommand(degrees, offset);
        return res;
    }
    void updateData(float degrees, float offset, int count)
    {
        view.setText(getText(degrees, offset));
        for (int i=0; i<count;++i) {
            sendRequest(degrees, offset);
        }
    }
    void sendRequest(float degrees, float offset) {
        String val = getCommand(degrees, offset);

        //Set message
        udpClient.Message = val;
        //Send message
        udpClient.NachrichtSenden();


    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView view3 = (TextView) findViewById(R.id.textView3);
        view = (TextView) findViewById(R.id.textView2);

        WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
        String ip = Formatter.formatIpAddress(wm.getConnectionInfo().getIpAddress());
        view3.setText("IP: " + ip);


        Joystick joystick = (Joystick) findViewById(R.id.joystick);
        joystick.setJoystickListener(new JoystickListener() {



            @Override
            public void onDown() {
                // ..
                updateData(0,0,5);
            }

            @Override
            public void onDrag(float degrees, float offset) {
                // ..
                updateData(degrees,offset,2);
            }

            @Override
            public void onUp() {
                // ..
                updateData(0,0,5);
            }
        });
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Main Page", // TODO: Define a title for the content shown.
                // TODO: If you have web page content that matches this app activity's content,
                // make sure this auto-generated web page URL is correct.
                // Otherwise, set the URL to null.
                Uri.parse("http://host/path"),
                // TODO: Make sure this auto-generated app URL is correct.
                Uri.parse("android-app://com.example.perec.robokot/http/host/path")
        );
        AppIndex.AppIndexApi.start(client, viewAction);
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Main Page", // TODO: Define a title for the content shown.
                // TODO: If you have web page content that matches this app activity's content,
                // make sure this auto-generated web page URL is correct.
                // Otherwise, set the URL to null.
                Uri.parse("http://host/path"),
                // TODO: Make sure this auto-generated app URL is correct.
                Uri.parse("android-app://com.example.perec.robokot/http/host/path")
        );
        AppIndex.AppIndexApi.end(client, viewAction);
        client.disconnect();
    }
}

