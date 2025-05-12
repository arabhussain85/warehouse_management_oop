#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include "product.h"
#include "utils.h"

using namespace std;

enum OrderStatus {
    ORDER_PENDING = 1,
    ORDER_PROCESSING = 2,
    ORDER_SHIPPED = 3,
    ORDER_DELIVERED = 4,
    ORDER_CANCELLED = 5
};

struct OrderItem {
    int productID;
    string productName;
    float price;
    int quantity;
    float subtotal;
};

class Order {
private:
    static int nextID;
    int orderID;
    int customerID;
    string customerName;
    vector<OrderItem> items;
    float totalAmount;
    time_t orderDate;
    OrderStatus status;

public:
    Order() {
        orderID = nextID++;
        customerID = 0;
        customerName = "";
        totalAmount = 0.0f;
        orderDate = time(nullptr);
        status = ORDER_PENDING;
    }

    Order(int custID, string custName) {
        orderID = nextID++;
        customerID = custID;
        customerName = custName;
        totalAmount = 0.0f;
        orderDate = time(nullptr);
        status = ORDER_PENDING;
    }

    int getID() const { return orderID; }
    int getCustomerID() const { return customerID; }
    string getCustomerName() const { return customerName; }
    float getTotalAmount() const { return totalAmount; }
    time_t getOrderDate() const { return orderDate; }
    OrderStatus getStatus() const { return status; }
    vector<OrderItem> getItems() const { return items; }

    void setCustomerID(int id) { customerID = id; }
    void setCustomerName(const string& name) { customerName = name; }
    void setStatus(OrderStatus newStatus) { status = newStatus; }

    void addItem(const Product& product, int quantity) {
        OrderItem item;
        item.productID = product.getID();
        item.productName = product.getName();
        item.price = product.getPrice();
        item.quantity = quantity;
        item.subtotal = item.price * quantity;
        
        items.push_back(item);
        totalAmount += item.subtotal;
    }

    bool removeItem(int productID) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i].productID == productID) {
                totalAmount -= items[i].subtotal;
                items.erase(items.begin() + i);
                return true;
            }
        }
        return false;
    }

    string getStatusString() const {
        switch (status) {
            case ORDER_PENDING: return "Pending";
            case ORDER_PROCESSING: return "Processing";
            case ORDER_SHIPPED: return "Shipped";
            case ORDER_DELIVERED: return "Delivered";
            case ORDER_CANCELLED: return "Cancelled";
            default: return "Unknown";
        }
    }

    string getFormattedDate() const {
        char buffer[26];
        struct tm* timeinfo = localtime(&orderDate);
        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", timeinfo);
        return string(buffer);
    }

    void display() const {
        cout << "┌───────────────────────────────────────────────────┐\n";
        cout << "│ " << CYAN << BOLD << "Order ID: " << RESET << setw(10) << left << orderID << "                          │\n";
        cout << "│ " << CYAN << BOLD << "Customer ID: " << RESET << setw(10) << left << customerID << "                       │\n";
        cout << "│ " << CYAN << BOLD << "Customer Name: " << RESET << setw(32) << left << customerName << " │\n";
        cout << "│ " << CYAN << BOLD << "Order Date: " << RESET << setw(35) << left << getFormattedDate() << " │\n";
        cout << "│ " << CYAN << BOLD << "Status: " << RESET << setw(40) << left << getStatusString() << " │\n";
        cout << "│ " << CYAN << BOLD << "Items:" << RESET << "                                           │\n";
        
        if (items.empty()) {
            cout << "│   No items in this order                             │\n";
        } else {
            cout << "├───────────────────────────────────────────────────┤\n";
            cout << "│ " << CYAN << setw(4) << left << "ID" << " | " 
                 << setw(20) << left << "Product" << " | " 
                 << setw(6) << left << "Price" << " | " 
                 << setw(4) << left << "Qty" << " | " 
                 << setw(8) << left << "Subtotal" << RESET << " │\n";
            cout << "├───────────────────────────────────────────────────┤\n";
            
            for (const auto& item : items) {
                cout << "│ " << setw(4) << left << item.productID << " | " 
                     << setw(20) << left << (item.productName.length() > 20 ? item.productName.substr(0, 17) + "..." : item.productName) << " | " 
                     << "$" << setw(5) << left << fixed << setprecision(2) << item.price << " | " 
                     << setw(4) << left << item.quantity << " | " 
                     << "$" << setw(7) << left << fixed << setprecision(2) << item.subtotal << " │\n";
            }
        }
        
        cout << "├───────────────────────────────────────────────────┤\n";
        cout << "│ " << CYAN << BOLD << "Total Amount: " << RESET << "$" << setw(32) << left << fixed << setprecision(2) << totalAmount << " │\n";
        cout << "└───────────────────────────────────────────────────┘\n";
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << orderID << "," << customerID << "," << customerName << "," 
                 << totalAmount << "," << orderDate << "," << status << "\n";
            file.close();
        } else {
            cout << "Unable to open file for writing\n";
            return;
        }
        
        string itemsFilename = "order_items.txt";
        ofstream itemsFile(itemsFilename, ios::app);
        if (itemsFile.is_open()) {
            for (const auto& item : items) {
                itemsFile << orderID << "," << item.productID << "," << item.productName << "," 
                         << item.price << "," << item.quantity << "," << item.subtotal << "\n";
            }
            itemsFile.close();
        } else {
            cout << "Unable to open items file for writing\n";
        }
    }

    static Order loadFromFile(const string& filename, const string& itemsFilename, int id) {
        ifstream file(filename);
        string line;
        Order order;
        bool found = false;
        
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            int fileID;
            
            getline(ss, field, ',');
            fileID = stoi(field);
            
            if (fileID == id) {
                order.orderID = fileID;
                
                getline(ss, field, ',');
                order.customerID = stoi(field);
                
                getline(ss, order.customerName, ',');
                
                getline(ss, field, ',');
                order.totalAmount = stof(field);
                
                getline(ss, field, ',');
                order.orderDate = stoll(field);
                
                getline(ss, field);
                order.status = static_cast<OrderStatus>(stoi(field));
                
                found = true;
                break;
            }
        }
        file.close();
        
        if (!found) return Order();
        
        ifstream itemsFile(itemsFilename);
        while (getline(itemsFile, line)) {
            stringstream ss(line);
            string field;
            int fileOrderID;
            
            getline(ss, field, ',');
            fileOrderID = stoi(field);
            
            if (fileOrderID == id) {
                OrderItem item;
                
                getline(ss, field, ',');
                item.productID = stoi(field);
                
                getline(ss, item.productName, ',');
                
                getline(ss, field, ',');
                item.price = stof(field);
                
                getline(ss, field, ',');
                item.quantity = stoi(field);
                
                getline(ss, field);
                item.subtotal = stof(field);
                
                order.items.push_back(item);
            }
        }
        itemsFile.close();
        
        return order;
    }

    static vector<Order> loadAllFromFile(const string& filename, const string& itemsFilename) {
        ifstream file(filename);
        string line;
        vector<Order> orders;
        
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            Order order;
            
            getline(ss, field, ',');
            order.orderID = stoi(field);
            
            getline(ss, field, ',');
            order.customerID = stoi(field);
            
            getline(ss, order.customerName, ',');
            
            getline(ss, field, ',');
            order.totalAmount = stof(field);
            
            getline(ss, field, ',');
            order.orderDate = stoll(field);
            
            getline(ss, field);
            order.status = static_cast<OrderStatus>(stoi(field));
            
            orders.push_back(order);
        }
        file.close();
        
        ifstream itemsFile(itemsFilename);
        while (getline(itemsFile, line)) {
            stringstream ss(line);
            string field;
            int fileOrderID;
            
            getline(ss, field, ',');
            fileOrderID = stoi(field);
            
            for (auto& order : orders) {
                if (order.orderID == fileOrderID) {
                    OrderItem item;
                    
                    getline(ss, field, ',');
                    item.productID = stoi(field);
                    
                    getline(ss, item.productName, ',');
                    
                    getline(ss, field, ',');
                    item.price = stof(field);
                    
                    getline(ss, field, ',');
                    item.quantity = stoi(field);
                    
                    getline(ss, field);
                    item.subtotal = stof(field);
                    
                    order.items.push_back(item);
                    break;
                }
            }
        }
        itemsFile.close();
        
        return orders;
    }
};

int Order::nextID = 1;

#endif
