package com.zybooks.miguelrodriguezinvmanager;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import java.util.ArrayList;
import java.util.List;

public class ItemDatabase extends SQLiteOpenHelper {

    public static final String DATABASE_NAME = "Items.db";
    public static final String TABLE_NAME = "Items_Table";
    public static final String COL_1 = "ID";
    public static final String COL_2 = "NAME";
    public static final String COL_3 = "QTY";

    public ItemDatabase(Context context){
        super(context, DATABASE_NAME, null, 1);
    }


    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL("create table "+ TABLE_NAME +" (ID INTEGER PRIMARY KEY AUTOINCREMENT,NAME TEXT, QTY INTEGER)");

    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS "+TABLE_NAME);
        onCreate(db);

    }

    public boolean insertItem(ItemModel itemModel){
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues contentValues = new ContentValues();
        contentValues.put(COL_2, itemModel.getName());
        contentValues.put(COL_3, itemModel.getQty());
        long result = db.insert(TABLE_NAME,null,contentValues);
        if (result == -1) { //if -1 returns error
            return false;
        }
        else {
            return true;
        }
    }

    public boolean updateItem(ItemModel itemModel){
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues contentValues = new ContentValues();

        contentValues.put(COL_3, itemModel.getQty());
        int rowsUpdated = db.update(TABLE_NAME, contentValues, COL_2+ "= ?", null);
        return rowsUpdated >0;


    }

    public boolean deleteItem(ItemModel itemModel){
        SQLiteDatabase db = getWritableDatabase();
        int rowsDeleted = db.delete(TABLE_NAME, COL_2 + " = ?",
                new String[] {COL_2});
        return rowsDeleted > 0;
    }

    public List<ItemModel> getAll(){
        List<ItemModel> returnList = new ArrayList<>();
        String queryString= "SELECT * FROM " + TABLE_NAME;
        SQLiteDatabase db = this.getReadableDatabase();

        Cursor cursor= db.rawQuery(queryString, new String[]{});

        if(cursor.moveToFirst()){
            do{
                int itemID = cursor.getInt(0);
                String itemName= cursor.getString(1);
                int itemQty= cursor.getInt(2);

                ItemModel itemModel = new ItemModel(itemID,itemName,itemQty);
                returnList.add(itemModel);



            }while(cursor.moveToFirst());

        }
        else{
            //failure do not add to list
        }
        cursor.close();
        return returnList;
    }

}
