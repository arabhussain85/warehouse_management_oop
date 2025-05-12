#ifndef PRODUCT_MANAGER_H
#define PRODUCT_MANAGER_H

#include <iostream>
#include <vector>
#include <fstream>
#include "product.h"
#include "utils.h"

using namespace std;

// ========== Product Management Functions ==========
void addProduct(vector<Product>& inventory, const string& filename) {
    clearScreen();
    loadingScreen("Opening Add Product");

    string name;
    float price;
    int quantity;

    cout << CYAN << BOLD << "ADD NEW PRODUCT\n" << RESET;
    cout << "------------------------\n";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Product Name: ";
    getline(cin, name);
    cout << "Enter Price: ";
    cin >> price;
    cout << "Enter Quantity: ";
    cin >> quantity;

    Product newProduct(name, price, quantity);
    inventory.push_back(newProduct);
    newProduct.saveToFile(filename);
    
    cout << "\n" << GREEN << "✅ Product added successfully!\n" << RESET;
    waitForEnter();
}

void viewProducts(const vector<Product>& inventory) {
    clearScreen();
    loadingScreen("Fetching Inventory");

    if (inventory.empty()) {
        cout << "\n" << YELLOW << "⚠️ No products available.\n" << RESET;
    } else {
        cout << CYAN << BOLD << "--- PRODUCT INVENTORY ---\n\n" << RESET;
        for (const auto& p : inventory)
            p.display();
    }
    waitForEnter();
}

int findProductIndexByID(const vector<Product>& inventory, int id) {
    for (size_t i = 0; i < inventory.size(); ++i)
        if (inventory[i].getID() == id) return i;
    return -1;
}

void updateProduct(vector<Product>& inventory, const string& filename) {
    clearScreen();
    loadingScreen("Opening Update Module");

    int id;
    cout << CYAN << BOLD << "UPDATE PRODUCT\n" << RESET;
    cout << "------------------------\n";
    cout << "Enter Product ID to update: ";
    cin >> id;

    int idx = findProductIndexByID(inventory, id);
    if (idx == -1) {
        cout << RED << "❌ Product not found.\n" << RESET;
        waitForEnter();
        return;
    }

    string name;
    float price;
    int quantity;
    cin.ignore();

    cout << "Current Name: " << inventory[idx].getName() << "\n";
    cout << "New Name (press Enter to keep current): ";
    getline(cin, name);
    if (name.empty()) name = inventory[idx].getName();

    cout << "Current Price: $" << inventory[idx].getPrice() << "\n";
    cout << "New Price (press Enter to keep current): ";
    string priceStr;
    getline(cin, priceStr);
    price = priceStr.empty() ? inventory[idx].getPrice() : stof(priceStr);

    cout << "Current Quantity: " << inventory[idx].getQuantity() << "\n";
    cout << "New Quantity (press Enter to keep current): ";
    string quantityStr;
    getline(cin, quantityStr);
    quantity = quantityStr.empty() ? inventory[idx].getQuantity() : stoi(quantityStr);

    inventory[idx].setName(name);
    inventory[idx].setPrice(price);
    inventory[idx].setQuantity(quantity);

    // Update the file
    ofstream file(filename);
    for (const auto& product : inventory) {
        file << product.getID() << "," << product.getName() << "," 
             << product.getPrice() << "," << product.getQuantity() << "\n";
    }
    file.close();

    cout << GREEN << "✅ Product updated successfully!\n" << RESET;
    waitForEnter();
}

void deleteProduct(vector<Product>& inventory, const string& filename) {
    clearScreen();
    loadingScreen("Opening Delete Module");

    int id;
    cout << CYAN << BOLD << "DELETE PRODUCT\n" << RESET;
    cout << "------------------------\n";
    cout << "Enter Product ID to delete: ";
    cin >> id;

    int idx = findProductIndexByID(inventory, id);
    if (idx == -1) {
        cout << RED << "❌ Product not found.\n" << RESET;
        waitForEnter();
        return;
    }

    inventory.erase(inventory.begin() + idx);
    
    // Update the file
    ofstream file(filename);
    for (const auto& product : inventory) {
        file << product.getID() << "," << product.getName() << "," 
             << product.getPrice() << "," << product.getQuantity() << "\n";
    }
    file.close();

    cout << GREEN << "🗑️ Product deleted successfully!\n" << RESET;
    waitForEnter();
}

void searchProduct(const vector<Product>& inventory) {
    clearScreen();
    loadingScreen("Searching Product");

    int id;
    cout << CYAN << BOLD << "SEARCH PRODUCT\n" << RESET;
    cout << "------------------------\n";
    cout << "Enter Product ID to search: ";
    cin >> id;

    int idx = findProductIndexByID(inventory, id);
    if (idx == -1) {
        cout << RED << "❌ Product not found.\n" << RESET;
    } else {
        cout << "\n" << GREEN << "🔍 Product Found:\n" << RESET;
        inventory[idx].display();
    }
    waitForEnter();
}

void showProductMenu() {
    clearScreen();
    cout << MAGENTA << BOLD << "========= PRODUCT MANAGEMENT =========\n" << RESET;
    cout << "1. Add Product\n";
    cout << "2. View All Products\n";
    cout << "3. Update Product\n";
    cout << "4. Delete Product\n";
    cout << "5. Search Product\n";
    cout << "6. Back to Main Menu\n";
    cout << "======================================\n";
    cout << CYAN << "Select an option (1-6): " << RESET;
}

void handleProductMenu(const string& productsFile) {
    vector<Product> inventory = Product::loadAllFromFile(productsFile);
    char choice;
    
    while (true) {
        showProductMenu();
        choice = singleInput();
        
        switch (choice) {
            case '1': addProduct(inventory, productsFile); break;
            case '2': viewProducts(inventory); break;
            case '3': updateProduct(inventory, productsFile); break;
            case '4': deleteProduct(inventory, productsFile); break;
            case '5': searchProduct(inventory); break;
            case '6': return;
            default:
                cout << RED << "Invalid choice. Try again.\n" << RESET;
                waitForEnter();
        }
    }
}

#endif
