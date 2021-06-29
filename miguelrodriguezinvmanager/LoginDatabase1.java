package com.zybooks.miguelrodriguezinvmanager;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import java.util.jar.Attributes;

public class LoginDatabase1 extends SQLiteOpenHelper {
    public static final String DATABASE_NAME = "Newlogin.db";
    public static final String TABLE_NAME = "Newlogin_table";
    public static final String COL_1 = "ID";
    public static final String COL_2 = "NAME";
    public static final String COL_3 = "PASSWORD";

    public LoginDatabase1(Context context){
        super(context, DATABASE_NAME, null, 1);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL("create table "+ TABLE_NAME +" (ID INTEGER PRIMARY KEY AUTOINCREMENT,NAME TEXT, PASSWORD TEXT)");

    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS "+TABLE_NAME);
        onCreate(db);

    }

    public boolean insertData(String name, String password){
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues contentValues = new ContentValues();
        contentValues.put(COL_2, name);
        contentValues.put(COL_3, password);
        long result = db.insert(TABLE_NAME,null,contentValues);
        if (result == -1)  //if -1 returns error
            return false;
        else
            return true;
    }

    public boolean matchData(String name, String password){
        String[] columns = {COL_1};
        SQLiteDatabase db = this.getReadableDatabase();


        String selection = COL_2 + " = ?" + " AND " + COL_3 + " = ?";

        String[] selectionArgs = {name, password};

        Cursor cursor = db.query(TABLE_NAME,columns,selection,selectionArgs,null, null,null);
        int cursorCount = cursor.getCount();

        if(cursorCount>0){
            return true;

        }else{
            return false;
        }

    }

}
