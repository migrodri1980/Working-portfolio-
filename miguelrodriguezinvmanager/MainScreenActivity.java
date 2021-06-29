package com.zybooks.miguelrodriguezinvmanager;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ClipData;
import android.os.Bundle;
import android.text.Layout;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.Toast;

import com.google.android.material.floatingactionbutton.FloatingActionButton;


public class MainScreenActivity extends AppCompatActivity {

    Button AddtoGrid, Deletebtn;
    EditText editTextTextPersonName5, editTextNumber;
    Layout layout;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_screen);

        //button calls
        AddtoGrid= findViewById(R.id.button6);
        Deletebtn= findViewById(R.id.button7);
    }


    public void addData(){
        AddtoGrid.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        ItemModel itemModel;
                        try{
                            itemModel = new ItemModel(-1, editTextTextPersonName5.getText().toString(), Integer.parseInt(editTextNumber.getText().toString()));
                            Toast.makeText(MainScreenActivity.this, itemModel.toString(), Toast.LENGTH_SHORT);
                        }
                        catch (Exception e){
                            Toast.makeText(MainScreenActivity.this, "Error adding item", Toast.LENGTH_SHORT).show();
                            itemModel = new ItemModel(-1, "error", 0 );
                        }
                        ItemDatabase itemDatabase = new ItemDatabase(MainScreenActivity.this);
                        itemDatabase.insertItem(itemModel);


                    }

                }

        );

    }

    public void deleteData(){
        Deletebtn.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        ItemModel itemModel;
                        try{
                            itemModel = new ItemModel(-1, editTextTextPersonName5.getText().toString(), Integer.parseInt(editTextNumber.getText().toString()));
                            Toast.makeText(MainScreenActivity.this, itemModel.toString(), Toast.LENGTH_SHORT);
                        }
                        catch (Exception e){
                            Toast.makeText(MainScreenActivity.this, "Error deleting item", Toast.LENGTH_SHORT).show();
                            itemModel = new ItemModel(-1, "error", 0 );
                        }
                        ItemDatabase itemDatabase = new ItemDatabase(MainScreenActivity.this);
                        itemDatabase.deleteItem(itemModel);


                    }

                }

        );

    }




}