#ifndef MENUS_H
#define MENUS_H

#include <iostream>
#include "utils.h"
#include "product_manager.h"
#include "supplier_manager.h"
#include "order_manager.h"
#include "staff_manager.h"
#include "auth.h"

using namespace std;

// ========== Menu Functions ==========
void showAdminMenu() {
    clearScreen();
    cout << MAGENTA << BOLD << "========= ADMIN MENU =========\n" << RESET;
    cout << "1. Product Management\n";
    cout << "2. Supplier Management\n";
    cout << "3. Order Management\n";
    cout << "4. Staff Management\n";
    cout << "5. Register New Staff\n";
    cout << "6. Logout\n";
    cout << "7. Exit\n";
    cout << "==============================\n";
    cout << CYAN << "Select an option (1-7): " << RESET;
}

void showManagerMenu() {
    clearScreen();
    cout << MAGENTA << BOLD << "========= MANAGER MENU =========\n" << RESET;
    cout << "1. Product Management\n";
    cout << "2. Supplier Management\n";
    cout << "3. Order Management\n";
    cout << "4. Logout\n";
    cout << "5. Exit\n";
    cout << "================================\n";
    cout << CYAN << "Select an option (1-5): " << RESET;
}

void showStaffMenu() {
    clearScreen();
    cout << MAGENTA << BOLD << "========= STAFF MENU =========\n" << RESET;
    cout << "1. View Products\n";
    cout << "2. Search Product\n";
    cout << "3. View Orders\n";
    cout << "4. Create Order\n";
    cout << "5. Logout\n";
    cout << "6. Exit\n";
    cout << "==============================\n";
    cout << CYAN << "Select an option (1-6): " << RESET;
}

// ========== Main Menu Handlers ==========
void handleAdminMenu(const string& productsFile, const string& suppliersFile, 
                     const string& ordersFile, const string& orderItemsFile, 
                     const string& staffFile, Staff& currentUser, bool& isLoggedIn) {
    char choice;
    
    while (true) {
        showAdminMenu();
        choice = singleInput();
        
        switch (choice) {
            case '1': handleProductMenu(productsFile); break;
            case '2': handleSupplierMenu(suppliersFile); break;
            case '3': handleOrderMenu(ordersFile, orderItemsFile, productsFile); break;
            case '4': handleStaffManagementMenu(staffFile, currentUser); break;
            case '5': signup(staffFile, currentUser); break;
            case '6': 
                logout();
                isLoggedIn = false;
                return;
            case '7':
                loadingScreen("Exiting...");
                cout << GREEN << "Thank you for using the system!\n" << RESET;
                exit(0);
            default:
                cout << RED << "Invalid choice. Try again.\n" << RESET;
                waitForEnter();
        }
    }
}

void handleManagerMenu(const string& productsFile, const string& suppliersFile, 
                       const string& ordersFile, const string& orderItemsFile, 
                       bool& isLoggedIn) {
    char choice;
    
    while (true) {
        showManagerMenu();
        choice = singleInput();
        
        switch (choice) {
            case '1': handleProductMenu(productsFile); break;
            case '2': handleSupplierMenu(suppliersFile); break;
            case '3': handleOrderMenu(ordersFile, orderItemsFile, productsFile); break;
            case '4': 
                logout();
                isLoggedIn = false;
                return;
            case '5':
                loadingScreen("Exiting...");
                cout << GREEN << "Thank you for using the system!\n" << RESET;
                exit(0);
            default:
                cout << RED << "Invalid choice. Try again.\n" << RESET;
                waitForEnter();
        }
    }
}

void handleStaffMenu(const string& productsFile, const string& ordersFile, 
                     const string& orderItemsFile, bool& isLoggedIn) {
    vector<Product> inventory = Product::loadAllFromFile(productsFile);
    vector<Order> orders = Order::loadAllFromFile(ordersFile, orderItemsFile);
    char choice;
    
    while (true) {
        showStaffMenu();
        choice = singleInput();
        
        switch (choice) {
            case '1': viewProducts(inventory); break;
            case '2': searchProduct(inventory); break;
            case '3': viewOrders(orders); break;
            case '4': createOrder(orders, inventory, ordersFile, productsFile); break;
            case '5': 
                logout();
                isLoggedIn = false;
                return;
            case '6':
                loadingScreen("Exiting...");
                cout << GREEN << "Thank you for using the system!\n" << RESET;
                exit(0);
            default:
                cout << RED << "Invalid choice. Try again.\n" << RESET;
                waitForEnter();
        }
    }
}

#endif
