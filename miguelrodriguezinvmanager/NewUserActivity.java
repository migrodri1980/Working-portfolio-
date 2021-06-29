package com.zybooks.miguelrodriguezinvmanager;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Button;
import android.widget.Toast;

public class NewUserActivity extends AppCompatActivity {
    EditText userName;
    EditText userPass;
    Button saveButton;
    LoginDatabase1 Logindb;
    Button backButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_new_user);
        Logindb = new LoginDatabase1(this);

        userName = (EditText) findViewById(R.id.newUserInput);
        userPass = (EditText) findViewById(R.id.newUserPass);
        saveButton = (Button) findViewById(R.id.saveButton);
        backButton = (Button) findViewById(R.id.backButton);
        addUser();


    }

    public void Back_Sent(View view){
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
    }

    public void addUser() {
        saveButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        boolean isInserted = Logindb.insertData(userName.getText().toString(),
                                userPass.getText().toString());
                        if(isInserted = true)
                            Toast.makeText(NewUserActivity.this, "Data Inserted",Toast.LENGTH_LONG).show();
                            else
                                Toast.makeText(NewUserActivity.this,"Data not Inserted", Toast.LENGTH_LONG). show();
                    }
                }
        );
    }
}
