#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>

class Product {
private:
    static int nextID;
    int productID;
    std::string name;
    float price;
    int quantity;

public:
    Product() {
        productID = nextID++;
        name = "Unnamed";
        price = 0.0f;
        quantity = 0;
    }

    Product(std::string n, float p, int q) {
        productID = nextID++;
        name = n;
        price = p;
        quantity = q;
    }

    // Getters
    int getID() const { return productID; }
    std::string getName() const { return name; }
    float getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setPrice(float newPrice) { price = newPrice; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }

    // Stock Management
    void addStock(int amount) {
        if (amount > 0) quantity += amount;
    }

    bool removeStock(int amount) {
        if (amount > 0 && quantity >= amount) {
            quantity -= amount;
            return true;
        }
        return false;
    }

    // Display product details
    void display() const {
        std::cout << "Product ID: " << productID << "\n"
                  << "Name: " << name << "\n"
                  << "Price: $" << price << "\n"
                  << "Quantity: " << quantity << "\n"
                  << "---------------------------\n";
    }
};

int Product::nextID = 1;

#endif 
