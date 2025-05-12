#ifndef SUPPLIER_PRODUCT_MANAGER_H
#define SUPPLIER_PRODUCT_MANAGER_H

#include <iostream>
#include <vector>
#include <fstream>
#include "product.h"
#include "supplier.h"
#include "utils.h"

using namespace std;

// ========== Supplier Product Management Functions ==========
void addProductAsSupplier(vector<Product>& inventory, const Supplier& currentSupplier, const string& filename) {
    clearScreen();
    loadingScreen("Opening Add Product");

    string name, category, description;
    float price;
    int quantity;

    cout << CYAN << BOLD << "ADD NEW PRODUCT\n" << RESET;
    cout << "┌───────────────────────────────────────────────────┐\n";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "│ " << YELLOW << "Enter Product Name: " << RESET;
    getline(cin, name);
    
    cout << "│ " << YELLOW << "Enter Category: " << RESET;
    getline(cin, category);
    
    cout << "│ " << YELLOW << "Enter Price: $" << RESET;
    cin >> price;
    
    cout << "│ " << YELLOW << "Enter Quantity: " << RESET;
    cin >> quantity;
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "Enter Description (optional): " << RESET;
    getline(cin, description);
    
    cout << "└───────────────────────────────────────────────────┘\n";

    // Create product with supplier ID
    Product newProduct(name, price, quantity, category, description, currentSupplier.getID());
    inventory.push_back(newProduct);
    newProduct.saveToFile(filename);
    
    cout << "\n" << GREEN << "✅ Product added successfully!\n" << RESET;
    waitForAnyKey();
}

void viewSupplierProducts(const vector<Product>& inventory, int supplierID) {
    clearScreen();
    loadingScreen("Fetching Your Products");

    // Filter products by supplier ID
    vector<Product> supplierProducts = Product::getProductsBySupplier(inventory, supplierID);
    
    if (supplierProducts.empty()) {
        cout << "\n" << YELLOW << "⚠️ You haven't added any products yet.\n" << RESET;
    } else {
        cout << CYAN << BOLD << "--- YOUR PRODUCTS ---\n\n" << RESET;
        cout << CYAN << BOLD << "Total Products: " << RESET << supplierProducts.size() << "\n\n";
        
        for (const auto& p : supplierProducts)
            p.display();
    }
    waitForAnyKey();
}

void updateSupplierProduct(vector<Product>& inventory, int supplierID, const string& filename) {
    clearScreen();
    loadingScreen("Opening Update Product");

    // Filter products by supplier ID
    vector<Product> supplierProducts = Product::getProductsBySupplier(inventory, supplierID);
    
    if (supplierProducts.empty()) {
        cout << "\n" << YELLOW << "⚠️ You haven't added any products yet.\n" << RESET;
        waitForAnyKey();
        return;
    }
    
    cout << CYAN << BOLD << "YOUR PRODUCTS:\n\n" << RESET;
    for (const auto& p : supplierProducts) {
        cout << "ID: " << p.getID() << " | " << p.getName() << " | $" << fixed << setprecision(2) << p.getPrice() << " | Stock: " << p.getQuantity() << "\n";
    }
    cout << "\n";

    int id;
    cout << CYAN << "┌───────────────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "Enter Product ID to update: " << RESET;
    cin >> id;
    cout << "└───────────────────────────────────────────────────┘\n";

    int idx = -1;
    for (size_t i = 0; i < inventory.size(); ++i) {
        if (inventory[i].getID() == id && inventory[i].getSupplierID() == supplierID) {
            idx = i;
            break;
        }
    }
    
    if (idx == -1) {
        cout << RED << "❌ Product not found or you don't have permission to update it.\n" << RESET;
        waitForAnyKey();
        return;
    }

    string name, category, description;
    float price;
    int quantity;
    
    displayMenuHeader("UPDATE PRODUCT #" + to_string(id));
    cout << CYAN << "Current Product Details:\n\n" << RESET;
    inventory[idx].display();
    cout << "\n";
    
    cout << CYAN << "┌───────────────────────────────────────────────────┐\n";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "New Name (press Enter to keep current): " << RESET;
    getline(cin, name);
    if (!name.empty()) inventory[idx].setName(name);
    
    cout << "│ " << YELLOW << "New Category (press Enter to keep current): " << RESET;
    getline(cin, category);
    if (!category.empty()) inventory[idx].setCategory(category);
    
    cout << "│ " << YELLOW << "New Price (0 to keep current): $" << RESET;
    cin >> price;
    if (price > 0) inventory[idx].setPrice(price);
    
    cout << "│ " << YELLOW << "New Quantity (-1 to keep current): " << RESET;
    cin >> quantity;
    if (quantity >= 0) inventory[idx].setQuantity(quantity);
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "New Description (press Enter to keep current): " << RESET;
    getline(cin, description);
    if (!description.empty()) inventory[idx].setDescription(description);
    
    cout << "└───────────────────────────────────────────────────┘\n";

    // Update the file
    ofstream file(filename);
    for (const auto& product : inventory) {
        file << product.getID() << "," << product.getName() << "," 
             << product.getPrice() << "," << product.getQuantity() << ","
             << product.getCategory() << "," << product.getDescription() << ","
             << product.getSupplierID() << "\n";
    }
    file.close();

    cout << GREEN << "✅ Product updated successfully!\n" << RESET;
    waitForAnyKey();
}

void deleteSupplierProduct(vector<Product>& inventory, int supplierID, const string& filename) {
    clearScreen();
    loadingScreen("Opening Delete Product");

    // Filter products by supplier ID
    vector<Product> supplierProducts = Product::getProductsBySupplier(inventory, supplierID);
    
    if (supplierProducts.empty()) {
        cout << "\n" << YELLOW << "⚠️ You haven't added any products yet.\n" << RESET;
        waitForAnyKey();
        return;
    }
    
    cout << CYAN << BOLD << "YOUR PRODUCTS:\n\n" << RESET;
    for (const auto& p : supplierProducts) {
        cout << "ID: " << p.getID() << " | " << p.getName() << " | $" << fixed << setprecision(2) << p.getPrice() << " | Stock: " << p.getQuantity() << "\n";
    }
    cout << "\n";

    int id;
    cout << CYAN << "┌───────────────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "Enter Product ID to delete: " << RESET;
    cin >> id;
    cout << "└───────────────────────────────────────────────────┘\n";

    bool found = false;
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->getID() == id && it->getSupplierID() == supplierID) {
            found = true;
            
            displayMenuHeader("DELETE PRODUCT #" + to_string(id));
            cout << CYAN << "Product Details:\n\n" << RESET;
            it->display();
            cout << "\n";
            
            cout << RED << "Are you sure you want to delete this product? (y/n): " << RESET;
            char confirm;
            cin >> confirm;
            
            if (confirm == 'y' || confirm == 'Y') {
                inventory.erase(it);
                
                // Update file
                ofstream file(filename);
                for (const auto& product : inventory) {
                    file << product.getID() << "," << product.getName() << "," 
                         << product.getPrice() << "," << product.getQuantity() << ","
                         << product.getCategory() << "," << product.getDescription() << ","
                         << product.getSupplierID() << "\n";
                }
                file.close();
                
                cout << GREEN << "🗑️ Product deleted successfully!\n" << RESET;
            } else {
                cout << YELLOW << "Delete operation cancelled.\n" << RESET;
            }
            break;
        }
    }

    if (!found) {
        cout << RED << "❌ Product not found or you don't have permission to delete it.\n" << RESET;
    }
    
    waitForAnyKey();
}

void showSupplierProductMenu() {
    clearScreen();
    cout << MAGENTA << BOLD << "┌───────────────────────────────────────────────────┐\n";
    cout << "│           SUPPLIER PRODUCT MANAGEMENT            │\n";
    cout << "├───────────────────────────────────────────────────┤\n";
    cout << "│ 1. Add New Product                               │\n";
    cout << "│ 2. View My Products                              │\n";
    cout << "│ 3. Update Product                                │\n";
    cout << "│ 4. Delete Product                                │\n";
    cout << "│ 5. Back to Supplier Dashboard                    │\n";
    cout << "└───────────────────────────────────────────────────┘\n" << RESET;
    cout << CYAN << "Select an option (1-5): " << RESET;
}

void handleSupplierProductMenu(vector<Product>& inventory, const Supplier& currentSupplier, const string& productsFile) {
    char choice;
    
    while (true) {
        showSupplierProductMenu();
        choice = singleInput();
        
        switch (choice) {
            case '1': 
                addProductAsSupplier(inventory, currentSupplier, productsFile); 
                break;
            case '2': 
                viewSupplierProducts(inventory, currentSupplier.getID()); 
                break;
            case '3': 
                updateSupplierProduct(inventory, currentSupplier.getID(), productsFile); 
                break;
            case '4': 
                deleteSupplierProduct(inventory, currentSupplier.getID(), productsFile); 
                break;
            case '5': 
                return;
            default:
                cout << RED << "Invalid choice. Try again.\n" << RESET;
                waitForAnyKey();
        }
    }
}

#endif
