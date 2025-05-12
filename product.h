#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "utils.h"

using namespace std;

class Product {
private:
    static int nextID;
    int productID;
    string name;
    float price;
    int quantity;
    string category;
    string description;
    int supplierID;  // Added supplierID to track which supplier added the product

public:
    Product() {
        productID = nextID++;
        name = "Unnamed";
        price = 0.0f;
        quantity = 0;
        category = "Uncategorized";
        description = "";
        supplierID = 0;
    }

    Product(string n, float p, int q, string c = "Uncategorized", string d = "", int sid = 0) {
        productID = nextID++;
        name = n;
        price = p;
        quantity = q;
        category = c;
        description = d;
        supplierID = sid;
    }

    int getID() const { return productID; }
    string getName() const { return name; }
    float getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    string getCategory() const { return category; }
    string getDescription() const { return description; }
    int getSupplierID() const { return supplierID; }

    void setName(const string& newName) { name = newName; }
    void setPrice(float newPrice) { price = newPrice; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setCategory(const string& newCategory) { category = newCategory; }
    void setDescription(const string& newDesc) { description = newDesc; }
    void setSupplierID(int sid) { supplierID = sid; }

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

    void display() const {
        cout << "┌───────────────────────────────────────────────────┐\n";
        cout << "│ " << CYAN << BOLD << "Product ID: " << RESET << setw(10) << left << productID << "                          │\n";
        cout << "│ " << CYAN << BOLD << "Name: " << RESET << setw(42) << left << name << " │\n";
        cout << "│ " << CYAN << BOLD << "Price: " << RESET << "$" << setw(40) << left << fixed << setprecision(2) << price << " │\n";
        cout << "│ " << CYAN << BOLD << "Quantity: " << RESET << setw(38) << left << quantity << " │\n";
        cout << "│ " << CYAN << BOLD << "Category: " << RESET << setw(38) << left << category << " │\n";
        
        if (!description.empty()) {
            cout << "│ " << CYAN << BOLD << "Description: " << RESET << "                                   │\n";
            
            // Split description into multiple lines if needed
            string desc = description;
            while (desc.length() > 0) {
                string line = desc.substr(0, 45);
                desc = desc.length() > 45 ? desc.substr(45) : "";
                cout << "│ " << setw(45) << left << line << " │\n";
            }
        }
        
        cout << "└───────────────────────────────────────────────────┘\n";
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << productID << "," << name << "," << price << "," << quantity << "," 
                 << category << "," << description << "," << supplierID << "\n";
            file.close();
        } else {
            cout << "Unable to open file for writing\n";
        }
    }

    static Product loadFromFile(const string& filename, int id) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            int fileID;
            string fileName, fileCategory, fileDesc;
            float filePrice;
            int fileQuantity, fileSupplierID = 0;
            
            getline(ss, field, ',');
            fileID = stoi(field);
            getline(ss, fileName, ',');
            getline(ss, field, ',');
            filePrice = stof(field);
            getline(ss, field, ',');
            fileQuantity = stoi(field);
            getline(ss, fileCategory, ',');
            getline(ss, fileDesc, ',');
            
            // Check if there's a supplier ID field
            if (getline(ss, field)) {
                fileSupplierID = stoi(field);
            }
            
            if (fileID == id) {
                Product p(fileName, filePrice, fileQuantity, fileCategory, fileDesc, fileSupplierID);
                p.productID = fileID;  // Preserve the original ID
                return p;
            }
        }
        file.close();
        return Product();
    }

    static vector<Product> loadAllFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        vector<Product> products;
        
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            int fileID;
            string fileName, fileCategory, fileDesc;
            float filePrice;
            int fileQuantity, fileSupplierID = 0;
            
            getline(ss, field, ',');
            fileID = stoi(field);
            getline(ss, fileName, ',');
            getline(ss, field, ',');
            filePrice = stof(field);
            getline(ss, field, ',');
            fileQuantity = stoi(field);
            getline(ss, fileCategory, ',');
            getline(ss, fileDesc, ',');
            
            // Check if there's a supplier ID field
            if (getline(ss, field)) {
                fileSupplierID = stoi(field);
            }
            
            Product p(fileName, filePrice, fileQuantity, fileCategory, fileDesc, fileSupplierID);
            p.productID = fileID;  // Preserve the original ID
            products.push_back(p);
        }
        file.close();
        return products;
    }
    
    // Search products by name (partial match)
    static vector<Product> searchByName(const vector<Product>& products, const string& searchTerm) {
        vector<Product> results;
        string lowerSearchTerm = searchTerm;
        
        // Convert search term to lowercase
        for (char& c : lowerSearchTerm) {
            c = tolower(c);
        }
        
        for (const auto& product : products) {
            string lowerName = product.getName();
            
            // Convert product name to lowercase
            for (char& c : lowerName) {
                c = tolower(c);
            }
            
            // Check if the product name contains the search term
            if (lowerName.find(lowerSearchTerm) != string::npos) {
                results.push_back(product);
            }
        }
        
        return results;
    }

    // Get products by supplier ID
    static vector<Product> getProductsBySupplier(const vector<Product>& products, int supplierID) {
        vector<Product> results;
        
        for (const auto& product : products) {
            if (product.getSupplierID() == supplierID) {
                results.push_back(product);
            }
        }
        
        return results;
    }
};

int Product::nextID = 1;

#endif
