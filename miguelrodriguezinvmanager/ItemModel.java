package com.zybooks.miguelrodriguezinvmanager;

public class ItemModel {
    private int id;
    private String name;
    private int qty;

    public ItemModel(int id, String name, int qty) {
        this.id = id;
        this.name = name;
        this.qty = qty;
    }

    public ItemModel() {
    }

    @Override
    public String toString() {
        return "ItemModel{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", qty=" + qty +
                '}';
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getQty() {
        return qty;
    }

    public void setQty(int qty) {
        this.qty = qty;
    }
}
