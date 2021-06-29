package com.zybooks.miguelrodriguezinvmanager;


import androidx.appcompat.app.AppCompatActivity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.service.autofill.OnClickAction;
import android.view.View;
import android.widget.ImageButton;
import android.widget.Toast;


public class ItemActivity extends AppCompatActivity {

    ImageButton SMSbutton;
    AlertDialog.Builder builder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_item);

        SMSbutton = (ImageButton)findViewById(R.id.SMSbutton);
        builder = new AlertDialog.Builder(this);
        SMSbutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                builder.setTitle("SMS Permission Required for Feature");
                builder.setMessage("Do you want to allow permission")
                        .setCancelable(false)
                        .setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {

                                Toast.makeText(getApplicationContext(), "Alerts enabled",
                                        Toast.LENGTH_SHORT).show();
                            }
                        })
                        .setNegativeButton("No", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                dialog.cancel();
                                Toast.makeText(getApplicationContext(), "Alerts not enabled",
                                        Toast.LENGTH_SHORT).show();
                            }


                        });
                AlertDialog alert = builder.create();
                alert.show();

            }
        });
    }
}





