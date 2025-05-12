#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <iostream>
#include <vector>
#include <fstream>
#include "order.h"
#include "product.h"
#include "utils.h"
#include "product_manager.h"

using namespace std;

// ========== Order Management Functions ==========
void createOrder(vector<Order>& orders, vector<Product>& inventory, const string& ordersFile, const string& productsFile) {
    clearScreen();
    loadingScreen("Opening Create Order");

    int customerID;
    string customerName;
    char addMore = 'y';

    cout << CYAN << BOLD << "CREATE NEW ORDER\n" << RESET;
    cout << "------------------------\n";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Customer ID: ";
    cin >> customerID;
    cin.ignore();
    cout << "Enter Customer Name: ";
    getline(cin, customerName);

    Order newOrder(customerID, customerName);

    while (addMore == 'y' || addMore == 'Y') {
        clearScreen();
        cout << CYAN << BOLD << "ADD ITEMS TO ORDER\n" << RESET;
        cout << "------------------------\n";
        
        // Display available products
        cout << "Available Products:\n";
        for (const auto& p : inventory) {
            cout << "ID: " << p.getID() << " | " << p.getName() 
                 << " | Price: $" << p.getPrice() << " | Stock: " << p.getQuantity() << "\n";
        }
        
        int productID, quantity;
        cout << "\nEnter Product ID to add: ";
        cin >> productID;
        
        int idx = findProductIndexByID(inventory, productID);
        if (idx == -1) {
            cout << RED << "❌ Product not found.\n" << RESET;
        } else {
            cout << "Enter Quantity: ";
            cin >> quantity;
            
            if (quantity <= 0) {
                cout << RED << "❌ Quantity must be positive.\n" << RESET;
            } else if (quantity > inventory[idx].getQuantity()) {
                cout << RED << "❌ Not enough stock available.\n" << RESET;
            } else {
                newOrder.addItem(inventory[idx], quantity);
                inventory[idx].removeStock(quantity);
                cout << GREEN << "✅ Item added to order.\n" << RESET;
            }
        }
        
        cout << "Add more items? (y/n): ";
        cin >> addMore;
    }

    // Save the order
    orders.push_back(newOrder);
    newOrder.saveToFile(ordersFile);
    
    // Update product inventory in file
    ofstream file(productsFile);
    for (const auto& product : inventory) {
        file << product.getID() << "," << product.getName() << "," 
             << product.getPrice() << "," << product.getQuantity() << "\n";
    }
    file.close();
    
    cout << "\n" << GREEN << "✅ Order created successfully!\n" << RESET;
    waitForEnter();
}

void viewOrders(const vector<Order>& orders) {
    clearScreen();
    loadingScreen("Fetching Orders");

    if (orders.empty()) {
        cout << "\n" << YELLOW << "⚠️ No orders available.\n" << RESET;
    } else {
        cout << CYAN << BOLD << "--- ORDER LIST ---\n\n" << RESET;
        for (const auto& o : orders)
            o.display();
    }
    waitForEnter();
}

int findOrderIndexByID(const vector<Order>& orders, int id) {
    for (size_t i = 0; i < orders.size(); ++i)
        if (orders[i].getID() == id) return i;
    return -1;
}

void updateOrderStatus(vector<Order>& orders, const string& ordersFile) {
    clearScreen();
    loadingScreen("Opening Update Order Status");

    int id;
    cout << CYAN << BOLD << "UPDATE ORDER STATUS\n" << RESET;
    cout << "------------------------\n";
    cout << "Enter Order ID to update: ";
    cin >> id;

    int idx = findOrderIndexByID(orders, id);
    if (idx == -1) {
        cout << RED << "❌ Order not found.\n" << RESET;
        waitForEnter();
        return;
    }

    cout << "Current Status: " << orders[idx].getStatusString() << "\n";
    cout << "Select New Status:\n";
    cout << "1. Pending\n";
    cout << "2. Processing\n";
    cout << "3. Shipped\n";
    cout << "4. Delivered\n";
    cout << "5. Cancelled\n";
    cout << "Enter choice (1-5): ";
    
    int statusChoice;
    cin >> statusChoice;
    
    if (statusChoice < 1 || statusChoice > 5) {
        cout << RED << "❌ Invalid status choice.\n" << RESET;
        waitForEnter();
        return;
    }
    
    orders[idx].setStatus(static_cast<OrderStatus>(statusChoice));
    
    // Update the file
    ofstream file(ordersFile);
    for (const auto& order : orders) {
        file << order.getID() << "," << order.getCustomerID() << "," 
             << order.getCustomerName() << "," << order.getTotalAmount() << ","
             << order.getOrderDate() << "," << order.getStatus() << "\n";
    }
    file.close();

    cout << GREEN << "✅ Order status updated successfully!\n" << RESET;
    waitForEnter();
}

void showOrderMenu() {
    clearScreen();
    cout << MAGENTA << BOLD << "========= ORDER MANAGEMENT =========\n" << RESET;
    cout << "1. Create Order\n";
    cout << "2. View All Orders\n";
    cout << "3. Update Order Status\n";
    cout << "4. Back to Main Menu\n";
    cout << "====================================\n";
    cout << CYAN << "Select an option (1-4): " << RESET;
}

void handleOrderMenu(const string& ordersFile, const string& orderItemsFile, const string& productsFile) {
    vector<Order> orders = Order::loadAllFromFile(ordersFile, orderItemsFile);
    vector<Product> inventory = Product::loadAllFromFile(productsFile);
    char choice;
    
    while (true) {
        showOrderMenu();
        choice = singleInput();
        
        switch (choice) {
            case '1': createOrder(orders, inventory, ordersFile, productsFile); break;
            case '2': viewOrders(orders); break;
            case '3': updateOrderStatus(orders, ordersFile); break;
            case '4': return;
            default:
                cout << RED << "Invalid choice. Try again.\n" << RESET;
                waitForEnter();
        }
    }
}

#endif
