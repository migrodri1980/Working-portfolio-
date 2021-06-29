package com.zybooks.miguelrodriguezinvmanager;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Button;
import android.widget.Toast;
public class MainActivity extends AppCompatActivity {

    Button buttonLogin;
    EditText userName;
    EditText userPassword;
    LoginDatabase1 Logindb;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Logindb = new LoginDatabase1(this);


        buttonLogin = (Button) findViewById(R.id.buttonLogin);
        userName = (EditText) findViewById(R.id.editTextTextPersonName);
        userPassword = (EditText) findViewById(R.id.editTextTextPassword);


        }



    public void newlogin_Sent(View view){
        Intent intent = new Intent(this, NewUserActivity.class);
        startActivity(intent);
    }

    public void userlogin_Sent(View view){
        Intent intent= new Intent(this, MainScreenActivity.class);
        startActivity(intent);
    }

   public void verifyData(){
        buttonLogin.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                       String name;
                       String password;

                        boolean isMatchData = Logindb.matchData(userName.getText().toString(),
                                userPassword.getText().toString());

                        name = userName.getText().toString();
                        password = userPassword.getText().toString();

                        if (isMatchData = true) {
                            Toast.makeText(MainActivity.this, "Access Granted", Toast.LENGTH_LONG).show();

                        }else {
                            Toast.makeText(MainActivity.this, "Access Denied", Toast.LENGTH_LONG).show();
                        }

                        LoginDatabase1 loginDatabase1 = new LoginDatabase1(MainActivity.this);
                        loginDatabase1.matchData(name,password);






                    }
                }
        );
    }

}