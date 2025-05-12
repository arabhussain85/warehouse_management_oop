#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>
#include <cstdlib>
#include "utils.h"
#include "staff.h"
#include "supplier.h"
#include "product.h"
#include "order.h"
#include "auth.h"

using namespace std;

// File paths
const string PRODUCTS_FILE = "products.csv";
const string SUPPLIERS_FILE = "suppliers.csv";
const string STAFF_FILE = "staff.csv";
const string ORDERS_FILE = "orders.csv";
const string ORDER_ITEMS_FILE = "order_items.csv";

// Function prototypes
void handleProductMenu(vector<Product>& inventory);
void handleSupplierMenu(vector<Supplier>& suppliers, const Staff& currentUser);
void handleOrderMenu(vector<Order>& orders, vector<Product>& inventory);
void handleStaffMenu(vector<Staff>& staffList, const Staff& currentUser);
void handleSupplierDashboard(Supplier& currentSupplier, vector<Product>& inventory);

// Product management functions
void addProduct(vector<Product>& inventory) {
    displayMenuHeader("ADD NEW PRODUCT");
    
    string name, category, description;
    float price;
    int quantity;
    
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "Enter Product Name: " << RESET;
    getline(cin, name);
    
    cout << "│ " << YELLOW << "Enter Category: " << RESET;
    getline(cin, category);
    
    cout << "│ " << YELLOW << "Enter Price: $" << RESET;
    cin >> price;
    
    cout << "│ " << YELLOW << "Enter Quantity: " << RESET;
    cin >> quantity;
    
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "Enter Description (optional): " << RESET;
    getline(cin, description);
    
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    Product newProduct(name, price, quantity, category, description);
    
    loadingScreen("Adding new product");
    
    inventory.push_back(newProduct);
    newProduct.saveToFile(PRODUCTS_FILE);
    
    showSuccess("Product added successfully!");
}

void viewProducts(const vector<Product>& inventory) {
    displayMenuHeader("PRODUCT INVENTORY");
    
    if (inventory.empty()) {
        showWarning("No products available.");
        return;
    }
    
    cout << CYAN << BOLD << "Total Products: " << RESET << inventory.size() << "\n\n";
    
    for (const auto& p : inventory) {
        p.display();
        cout << "\n";
    }
    
    waitForAnyKey();
}

void updateProduct(vector<Product>& inventory) {
    displayMenuHeader("UPDATE PRODUCT");
    
    int updateID;
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "Enter Product ID to update: " << RESET;
    cin >> updateID;
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    bool updateFound = false;
    for (auto& p : inventory) {
        if (p.getID() == updateID) {
            updateFound = true;
            string newName, newCategory, newDesc;
            float newPrice;
            int newQuantity;
            
            displayMenuHeader("UPDATE PRODUCT #" + to_string(updateID));
            cout << CYAN << "Current Product Details:\n\n" << RESET;
            p.display();
            cout << "\n";
            
            cout << CYAN << "┌─────────────────────────────────────────┐\n";
            
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            cout << "│ " << YELLOW << "New Name (press Enter to keep current): " << RESET;
            getline(cin, newName);
            if (!newName.empty()) p.setName(newName);
            
            cout << "│ " << YELLOW << "New Category (press Enter to keep current): " << RESET;
            getline(cin, newCategory);
            if (!newCategory.empty()) p.setCategory(newCategory);
            
            cout << "│ " << YELLOW << "New Price (0 to keep current): $" << RESET;
            cin >> newPrice;
            if (newPrice > 0) p.setPrice(newPrice);
            
            cout << "│ " << YELLOW << "New Quantity (-1 to keep current): " << RESET;
            cin >> newQuantity;
            if (newQuantity >= 0) p.setQuantity(newQuantity);
            
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            cout << "│ " << YELLOW << "New Description (press Enter to keep current): " << RESET;
            getline(cin, newDesc);
            if (!newDesc.empty()) p.setDescription(newDesc);
            
            cout << CYAN << "└─────────────────────────────────────────┘\n";
            
            loadingScreen("Updating product");
            
            // Update file
            ofstream file(PRODUCTS_FILE);
            for (const auto& product : inventory) {
                file << product.getID() << "," << product.getName() << "," 
                     << product.getPrice() << "," << product.getQuantity() << ","
                     << product.getCategory() << "," << product.getDescription() << "\n";
            }
            file.close();
            
            showSuccess("Product updated successfully!");
            break;
        }
    }
    
    if (!updateFound) {
        showError("Product not found.");
    }
}

void deleteProduct(vector<Product>& inventory) {
    displayMenuHeader("DELETE PRODUCT");
    
    int deleteID;
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "Enter Product ID to delete: " << RESET;
    cin >> deleteID;
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    bool deleteFound = false;
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->getID() == deleteID) {
            deleteFound = true;
            
            displayMenuHeader("DELETE PRODUCT #" + to_string(deleteID));
            cout << CYAN << "Product Details:\n\n" << RESET;
            it->display();
            cout << "\n";
            
            cout << RED << "Are you sure you want to delete this product? (y/n): " << RESET;
            char confirm;
            cin >> confirm;
            
            if (confirm == 'y' || confirm == 'Y') {
                inventory.erase(it);
                
                loadingScreen("Deleting product");
                
                // Update file
                ofstream file(PRODUCTS_FILE);
                for (const auto& product : inventory) {
                    file << product.getID() << "," << product.getName() << "," 
                         << product.getPrice() << "," << product.getQuantity() << ","
                         << product.getCategory() << "," << product.getDescription() << "\n";
                }
                file.close();
                
                showSuccess("Product deleted successfully!");
            } else {
                showWarning("Delete operation cancelled.");
            }
            break;
        }
    }
    
    if (!deleteFound) {
        showError("Product not found.");
    }
}

void searchProduct(const vector<Product>& inventory) {
    displayMenuHeader("SEARCH PRODUCT");
    
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "1. Search by ID" << RESET << "                       │\n";
    cout << "│ " << YELLOW << "2. Search by Name" << RESET << "                     │\n";
    cout << "│ " << YELLOW << "3. Back to Product Menu" << RESET << "               │\n";
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    cout << CYAN << "Select an option (1-3): " << RESET;
    
    char choice = singleInput();
    
    switch (choice) {
        case '1': {
            displayMenuHeader("SEARCH BY ID");
            
            int searchID;
            cout << CYAN << "┌─────────────────────────────────────────┐\n";
            cout << "│ " << YELLOW << "Enter Product ID to search: " << RESET;
            cin >> searchID;
            cout << CYAN << "└─────────────────────────────────────────┘\n";
            
            loadingScreen("Searching for product");
            
            bool searchFound = false;
            for (const auto& p : inventory) {
                if (p.getID() == searchID) {
                    displayMenuHeader("SEARCH RESULTS");
                    cout << GREEN << "Product found:\n\n" << RESET;
                    p.display();
                    searchFound = true;
                    break;
                }
            }
            
            if (!searchFound) {
                showError("Product not found.");
            } else {
                waitForAnyKey();
            }
            break;
        }
        case '2': {
            displayMenuHeader("SEARCH BY NAME");
            
            string searchName;
            cout << CYAN << "┌─────────────────────────────────────────┐\n";
            cout << "│ " << YELLOW << "Enter Product Name to search: " << RESET;
            
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            getline(cin, searchName);
            cout << CYAN << "└─────────────────────────────────────────┘\n";
            
            loadingScreen("Searching for products");
            
            vector<Product> results = Product::searchByName(inventory, searchName);
            
            if (results.empty()) {
                showError("No products found matching '" + searchName + "'.");
            } else {
                displayMenuHeader("SEARCH RESULTS");
                cout << GREEN << "Found " << results.size() << " product(s) matching '" << searchName << "':\n\n" << RESET;
                
                for (const auto& p : results) {
                    p.display();
                    cout << "\n";
                }
                
                waitForAnyKey();
            }
            break;
        }
        case '3':
            return;
        default:
            showError("Invalid choice. Try again.");
    }
}

// Supplier management functions
void addSupplier(vector<Supplier>& suppliers) {
    displayMenuHeader("ADD NEW SUPPLIER");
    
    string name, contactPerson, phone, email, address, username, password;
    
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "Enter Supplier Name: " << RESET;
    getline(cin, name);
    
    cout << "│ " << YELLOW << "Enter Contact Person: " << RESET;
    getline(cin, contactPerson);
    
    cout << "│ " << YELLOW << "Enter Phone: " << RESET;
    getline(cin, phone);
    
    cout << "│ " << YELLOW << "Enter Email: " << RESET;
    getline(cin, email);
    
    cout << "│ " << YELLOW << "Enter Address: " << RESET;
    getline(cin, address);
    
    cout << "│ " << YELLOW << "Create Username: " << RESET;
    cin >> username;
    
    // Check if username already exists
    Supplier existingSupplier = Supplier::findByUsername(SUPPLIERS_FILE, username);
    if (!existingSupplier.getUsername().empty()) {
        cout << CYAN << "└─────────────────────────────────────────┘\n";
        showError("Username already exists!");
        return;
    }
    
    cout << "│ " << YELLOW << "Create Password: " << RESET;
    password = getMaskedPassword();
    
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    Supplier newSupplier(name, contactPerson, phone, email, address, username, password);
    
    loadingScreen("Adding new supplier");
    
    suppliers.push_back(newSupplier);
    newSupplier.saveToFile(SUPPLIERS_FILE);
    
    showSuccess("Supplier added successfully!");
}

void viewSuppliers(const vector<Supplier>& suppliers) {
    displayMenuHeader("SUPPLIER LIST");
    
    if (suppliers.empty()) {
        showWarning("No suppliers available.");
        return;
    }
    
    cout << CYAN << BOLD << "Total Suppliers: " << RESET << suppliers.size() << "\n\n";
    
    for (const auto& s : suppliers) {
        s.display();
        cout << "\n";
    }
    
    waitForAnyKey();
}

void updateSupplier(vector<Supplier>& suppliers) {
    displayMenuHeader("UPDATE SUPPLIER");
    
    int updateID;
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "Enter Supplier ID to update: " << RESET;
    cin >> updateID;
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    bool updateFound = false;
    for (auto& s : suppliers) {
        if (s.getID() == updateID) {
            updateFound = true;
            string newName, newCP, newPhone, newEmail, newAddress, newUsername, newPassword;
            int newStatus;
            
            displayMenuHeader("UPDATE SUPPLIER #" + to_string(updateID));
            cout << CYAN << "Current Supplier Details:\n\n" << RESET;
            s.display();
            cout << "\n";
            
            cout << CYAN << "┌─────────────────────────────────────────┐\n";
            
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            cout << "│ " << YELLOW << "New Name (press Enter to keep current): " << RESET;
            getline(cin, newName);
            if (!newName.empty()) s.setName(newName);
            
            cout << "│ " << YELLOW << "New Contact Person (press Enter to keep current): " << RESET;
            getline(cin, newCP);
            if (!newCP.empty()) s.setContactPerson(newCP);
            
            cout << "│ " << YELLOW << "New Phone (press Enter to keep current): " << RESET;
            getline(cin, newPhone);
            if (!newPhone.empty()) s.setPhone(newPhone);
            
            cout << "│ " << YELLOW << "New Email (press Enter to keep current): " << RESET;
            getline(cin, newEmail);
            if (!newEmail.empty()) s.setEmail(newEmail);
            
            cout << "│ " << YELLOW << "New Address (press Enter to keep current): " << RESET;
            getline(cin, newAddress);
            if (!newAddress.empty()) s.setAddress(newAddress);
            
            cout << "│ " << YELLOW << "New Username (press Enter to keep current): " << RESET;
            getline(cin, newUsername);
            if (!newUsername.empty()) {
                // Check if username already exists
                Supplier existingSupplier = Supplier::findByUsername(SUPPLIERS_FILE, newUsername);
                if (!existingSupplier.getUsername().empty() && existingSupplier.getID() != s.getID()) {
                    cout << CYAN << "└─────────────────────────────────────────┘\n";
                    showError("Username already exists!");
                    return;
                }
                s.setUsername(newUsername);
            }
            
            cout << "│ " << YELLOW << "New Password (press Enter to keep current): " << RESET;
            newPassword = getMaskedPassword();
            if (!newPassword.empty()) s.setPassword(newPassword);
            
            cout << "│ " << YELLOW << "New Status (1=Active, 2=Inactive, 3=Pending, 0=Keep current): " << RESET;
            cin >> newStatus;
            if (newStatus >= 1 && newStatus <= 3) s.setStatus(static_cast<SupplierStatus>(newStatus));
            
            cout << CYAN << "└─────────────────────────────────────────┘\n";
            
            loadingScreen("Updating supplier");
            
            // Update file
            ofstream file(SUPPLIERS_FILE);
            for (const auto& supplier : suppliers) {
                file << supplier.getID() << "," << supplier.getName() << "," 
                     << supplier.getContactPerson() << "," << supplier.getPhone() << ","
                     << supplier.getEmail() << "," << supplier.getAddress() << ","
                     << supplier.getUsername() << "," << supplier.getPassword() << ","
                     << supplier.getStatus() << "\n";
            }
            file.close();
            
            showSuccess("Supplier updated successfully!");
            break;
        }
    }
    
    if (!updateFound) {
        showError("Supplier not found.");
    }
}

void deleteSupplier(vector<Supplier>& suppliers) {
    displayMenuHeader("DELETE SUPPLIER");
    
    int deleteID;
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "Enter Supplier ID to delete: " << RESET;
    cin >> deleteID;
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    bool deleteFound = false;
    for (auto it = suppliers.begin(); it != suppliers.end(); ++it) {
        if (it->getID() == deleteID) {
            deleteFound = true;
            
            displayMenuHeader("DELETE SUPPLIER #" + to_string(deleteID));
            cout << CYAN << "Supplier Details:\n\n" << RESET;
            it->display();
            cout << "\n";
            
            cout << RED << "Are you sure you want to delete this supplier? (y/n): " << RESET;
            char confirm;
            cin >> confirm;
            
            if (confirm == 'y' || confirm == 'Y') {
                suppliers.erase(it);
                
                loadingScreen("Deleting supplier");
                
                // Update file
                ofstream file(SUPPLIERS_FILE);
                for (const auto& supplier : suppliers) {
                    file << supplier.getID() << "," << supplier.getName() << "," 
                         << supplier.getContactPerson() << "," << supplier.getPhone() << ","
                         << supplier.getEmail() << "," << supplier.getAddress() << ","
                         << supplier.getUsername() << "," << supplier.getPassword() << ","
                         << supplier.getStatus() << "\n";
                }
                file.close();
                
                showSuccess("Supplier deleted successfully!");
            } else {
                showWarning("Delete operation cancelled.");
            }
            break;
        }
    }
    
    if (!deleteFound) {
        showError("Supplier not found.");
    }
}

// Order management functions
void createOrder(vector<Order>& orders, vector<Product>& inventory) {
    displayMenuHeader("CREATE NEW ORDER");
    
    int customerID;
    string customerName;
    char addMore = 'y';
    
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "Enter Customer ID: " << RESET;
    cin >> customerID;
    
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "Enter Customer Name: " << RESET;
    getline(cin, customerName);
    
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    Order newOrder(customerID, customerName);
    
    while (addMore == 'y' || addMore == 'Y') {
        displayMenuHeader("ADD ITEMS TO ORDER");
        
        cout << CYAN << "Available Products:\n\n" << RESET;
        
        if (inventory.empty()) {
            showWarning("No products available.");
            return;
        }
        
        for (const auto& p : inventory) {
            cout << CYAN << "ID: " << p.getID() << " | " << p.getName() 
                 << " | Price: $" << fixed << setprecision(2) << p.getPrice() 
                 << " | Stock: " << p.getQuantity() << "\n" << RESET;
        }
        
        cout << "\n";
        cout << CYAN << "┌─────────────────────────────────────────┐\n";
        cout << "│ " << YELLOW << "Enter Product ID to add: " << RESET;
        
        int productID;
        cin >> productID;
        
        bool productFound = false;
        for (auto& p : inventory) {
            if (p.getID() == productID) {
                productFound = true;
                cout << "│ " << YELLOW << "Enter Quantity: " << RESET;
                
                int quantity;
                cin >> quantity;
                
                if (quantity <= 0) {
                    cout << CYAN << "└─────────────────────────────────────────┘\n";
                    showError("Quantity must be positive.");
                } else if (quantity > p.getQuantity()) {
                    cout << CYAN << "└─────────────────────────────────────────┘\n";
                    showError("Not enough stock available.");
                } else {
                    newOrder.addItem(p, quantity);
                    p.removeStock(quantity);
                    cout << CYAN << "└─────────────────────────────────────────┘\n";
                    showSuccess("Item added to order.");
                }
                break;
            }
        }
        
        if (!productFound) {
            cout << CYAN << "└─────────────────────────────────────────┘\n";
            showError("Product not found.");
        }
        
        cout << YELLOW << "Add more items? (y/n): " << RESET;
        cin >> addMore;
    }
    
    if (newOrder.getItems().empty()) {
        showWarning("Order is empty. Operation cancelled.");
        return;
    }
    
    loadingScreen("Creating order");
    
    orders.push_back(newOrder);
    newOrder.saveToFile(ORDERS_FILE);
    
    // Update product inventory in file
    ofstream file(PRODUCTS_FILE);
    for (const auto& product : inventory) {
        file << product.getID() << "," << product.getName() << "," 
             << product.getPrice() << "," << product.getQuantity() << ","
             << product.getCategory() << "," << product.getDescription() << "\n";
    }
    file.close();
    
    showSuccess("Order created successfully!");
}

void viewOrders(const vector<Order>& orders) {
    displayMenuHeader("ORDER LIST");
    
    if (orders.empty()) {
        showWarning("No orders available.");
        return;
    }
    
    cout << CYAN << BOLD << "Total Orders: " << RESET << orders.size() << "\n\n";
    
    for (const auto& o : orders) {
        o.display();
        cout << "\n";
    }
    
    waitForAnyKey();
}

void updateOrderStatus(vector<Order>& orders) {
    displayMenuHeader("UPDATE ORDER STATUS");
    
    int updateID;
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "Enter Order ID to update: " << RESET;
    cin >> updateID;
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    bool updateFound = false;
    for (auto& o : orders) {
        if (o.getID() == updateID) {
            updateFound = true;
            
            displayMenuHeader("UPDATE ORDER #" + to_string(updateID));
            cout << CYAN << "Current Order Details:\n\n" << RESET;
            o.display();
            cout << "\n";
            
            cout << CYAN << "┌─────────────────────────────────────────┐\n";
            cout << "│ " << YELLOW << "Current Status: " << RESET << o.getStatusString() << "\n";
            cout << "│ " << YELLOW << "Select New Status:" << RESET << "\n";
            cout << "│ " << YELLOW << "1. Pending" << RESET << "\n";
            cout << "│ " << YELLOW << "2. Processing" << RESET << "\n";
            cout << "│ " << YELLOW << "3. Shipped" << RESET << "\n";
            cout << "│ " << YELLOW << "4. Delivered" << RESET << "\n";
            cout << "│ " << YELLOW << "5. Cancelled" << RESET << "\n";
            cout << "│ " << YELLOW << "Enter choice (1-5): " << RESET;
            
            int statusChoice;
            cin >> statusChoice;
            cout << CYAN << "└─────────────────────────────────────────┘\n";
            
            if (statusChoice < 1 || statusChoice > 5) {
                showError("Invalid status choice.");
            } else {
                o.setStatus(static_cast<OrderStatus>(statusChoice));
                
                loadingScreen("Updating order status");
                
                // Update file
                ofstream file(ORDERS_FILE);
                for (const auto& order : orders) {
                    file << order.getID() << "," << order.getCustomerID() << "," 
                         << order.getCustomerName() << "," << order.getTotalAmount() << ","
                         << order.getOrderDate() << "," << order.getStatus() << "\n";
                }
                file.close();
                
                showSuccess("Order status updated successfully!");
            }
            break;
        }
    }
    
    if (!updateFound) {
        showError("Order not found.");
    }
}

// Staff management functions
void viewStaffList(const vector<Staff>& staffList) {
    displayMenuHeader("STAFF LIST");
    
    if (staffList.empty()) {
        showWarning("No staff records available.");
        return;
    }
    
    cout << CYAN << BOLD << "Total Staff: " << RESET << staffList.size() << "\n\n";
    
    for (const auto& s : staffList) {
        s.display();
        cout << "\n";
    }
    
    waitForAnyKey();
}

void updateStaffMember(vector<Staff>& staffList, const Staff& currentUser) {
    displayMenuHeader("UPDATE STAFF");
    
    if (currentUser.getRole() != ADMIN) {
        showError("Only administrators can update staff records.");
        return;
    }
    
    int updateID;
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "Enter Staff ID to update: " << RESET;
    cin >> updateID;
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    bool updateFound = false;
    for (auto& s : staffList) {
        if (s.getID() == updateID) {
            updateFound = true;
            string newName, newPhone, newEmail, newUsername, newPassword;
            int newRole;
            
            displayMenuHeader("UPDATE STAFF #" + to_string(updateID));
            cout << CYAN << "Current Staff Details:\n\n" << RESET;
            s.display();
            cout << "\n";
            
            cout << CYAN << "┌─────────────────────────────────────────┐\n";
            
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            cout << "│ " << YELLOW << "New Name (press Enter to keep current): " << RESET;
            getline(cin, newName);
            if (!newName.empty()) s.setName(newName);
            
            cout << "│ " << YELLOW << "New Phone (press Enter to keep current): " << RESET;
            getline(cin, newPhone);
            if (!newPhone.empty()) s.setPhone(newPhone);
            
            cout << "│ " << YELLOW << "New Email (press Enter to keep current): " << RESET;
            getline(cin, newEmail);
            if (!newEmail.empty()) s.setEmail(newEmail);
            
            cout << "│ " << YELLOW << "New Username (press Enter to keep current): " << RESET;
            getline(cin, newUsername);
            if (!newUsername.empty()) {
                // Check if username already exists
                Staff existingUser = Staff::findByUsername(STAFF_FILE, newUsername);
                if (!existingUser.getUsername().empty() && existingUser.getID() != s.getID()) {
                    cout << CYAN << "└─────────────────────────────────────────┘\n";
                    showError("Username already exists!");
                    return;
                }
                s.setUsername(newUsername);
            }
            
            cout << "│ " << YELLOW << "New Password (press Enter to keep current): " << RESET;
            newPassword = getMaskedPassword();
            if (!newPassword.empty()) s.setPassword(newPassword);
            
            cout << "│ " << YELLOW << "New Role (1=Admin, 2=Manager, 3=Staff, 0=Keep current): " << RESET;
            cin >> newRole;
            if (newRole >= 1 && newRole <= 3) s.setRole(static_cast<Role>(newRole));
            
            cout << CYAN << "└─────────────────────────────────────────┘\n";
            
            loadingScreen("Updating staff record");
            
            // Update file
            ofstream file(STAFF_FILE);
            for (const auto& staff : staffList) {
                file << staff.getID() << "," << staff.getUsername() << "," 
                     << staff.getPassword() << "," << staff.getName() << ","
                     << staff.getPhone() << "," << staff.getEmail() << ","
                     << staff.getRole() << "\n";
            }
            file.close();
            
            showSuccess("Staff record updated successfully!");
            break;
        }
    }
    
    if (!updateFound) {
        showError("Staff not found.");
    }
}

void deleteStaffMember(vector<Staff>& staffList, const Staff& currentUser) {
    displayMenuHeader("DELETE STAFF");
    
    if (currentUser.getRole() != ADMIN) {
        showError("Only administrators can delete staff records.");
        return;
    }
    
    int deleteID;
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "Enter Staff ID to delete: " << RESET;
    cin >> deleteID;
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    if (deleteID == currentUser.getID()) {
        showError("You cannot delete your own account.");
        return;
    }
    
    bool deleteFound = false;
    for (auto it = staffList.begin(); it != staffList.end(); ++it) {
        if (it->getID() == deleteID) {
            deleteFound = true;
            
            displayMenuHeader("DELETE STAFF #" + to_string(deleteID));
            cout << CYAN << "Staff Details:\n\n" << RESET;
            it->display();
            cout << "\n";
            
            cout << RED << "Are you sure you want to delete this staff member? (y/n): " << RESET;
            char confirm;
            cin >> confirm;
            
            if (confirm == 'y' || confirm == 'Y') {
                staffList.erase(it);
                
                loadingScreen("Deleting staff record");
                
                // Update file
                ofstream file(STAFF_FILE);
                for (const auto& staff : staffList) {
                    file << staff.getID() << "," << staff.getUsername() << "," 
                         << staff.getPassword() << "," << staff.getName() << ","
                         << staff.getPhone() << "," << staff.getEmail() << ","
                         << staff.getRole() << "\n";
                }
                file.close();
                
                showSuccess("Staff record deleted successfully!");
            } else {
                showWarning("Delete operation cancelled.");
            }
            break;
        }
    }
    
    if (!deleteFound) {
        showError("Staff not found.");
    }
}

// Supplier dashboard functions
void viewSupplierProfile(const Supplier& supplier) {
    displayMenuHeader("SUPPLIER PROFILE");
    
    cout << CYAN << "Your Profile Details:\n\n" << RESET;
    supplier.display();
    
    waitForAnyKey();
}

void updateSupplierProfile(Supplier& supplier) {
    displayMenuHeader("UPDATE PROFILE");
    
    string newContactPerson, newPhone, newEmail, newAddress, newPassword;
    
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "New Contact Person (press Enter to keep current): " << RESET;
    getline(cin, newContactPerson);
    if (!newContactPerson.empty()) supplier.setContactPerson(newContactPerson);
    
    cout << "│ " << YELLOW << "New Phone (press Enter to keep current): " << RESET;
    getline(cin, newPhone);
    if (!newPhone.empty()) supplier.setPhone(newPhone);
    
    cout << "│ " << YELLOW << "New Email (press Enter to keep current): " << RESET;
    getline(cin, newEmail);
    if (!newEmail.empty()) supplier.setEmail(newEmail);
    
    cout << "│ " << YELLOW << "New Address (press Enter to keep current): " << RESET;
    getline(cin, newAddress);
    if (!newAddress.empty()) supplier.setAddress(newAddress);
    
    cout << "│ " << YELLOW << "New Password (press Enter to keep current): " << RESET;
    newPassword = getMaskedPassword();
    if (!newPassword.empty()) supplier.setPassword(newPassword);
    
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    loadingScreen("Updating profile");
    
    // Update file - need to load all suppliers first
    vector<Supplier> suppliers = Supplier::loadAllFromFile(SUPPLIERS_FILE);
    for (auto& s : suppliers) {
        if (s.getID() == supplier.getID()) {
            s = supplier;
            break;
        }
    }
    
    ofstream file(SUPPLIERS_FILE);
    for (const auto& s : suppliers) {
        file << s.getID() << "," << s.getName() << "," 
             << s.getContactPerson() << "," << s.getPhone() << ","
             << s.getEmail() << "," << s.getAddress() << ","
             << s.getUsername() << "," << s.getPassword() << ","
             << s.getStatus() << "\n";
    }
    file.close();
    
    showSuccess("Profile updated successfully!");
}

void viewSupplierProducts(const vector<Product>& inventory) {
    displayMenuHeader("VIEW PRODUCTS");
    
    if (inventory.empty()) {
        showWarning("No products available in the system.");
        return;
    }
    
    cout << CYAN << BOLD << "Total Products: " << RESET << inventory.size() << "\n\n";
    
    for (const auto& p : inventory) {
        p.display();
        cout << "\n";
    }
    
    waitForAnyKey();
}

// Menu handlers
void handleProductMenu(vector<Product>& inventory) {
    while (true) {
        displayMenuHeader("PRODUCT MANAGEMENT");
        
        cout << CYAN << "┌─────────────────────────────────────────┐\n";
        cout << "│ " << YELLOW << "1. Add Product" << RESET << "                        │\n";
        cout << "│ " << YELLOW << "2. View All Products" << RESET << "                  │\n";
        cout << "│ " << YELLOW << "3. Update Product" << RESET << "                     │\n";
        cout << "│ " << YELLOW << "4. Delete Product" << RESET << "                     │\n";
        cout << "│ " << YELLOW << "5. Search Product" << RESET << "                     │\n";
        cout << "│ " << YELLOW << "6. Back to Main Menu" << RESET << "                  │\n";
        cout << CYAN << "└─────────────────────────────────────────┘\n";
        cout << CYAN << "Select an option (1-6): " << RESET;
        
        char choice = singleInput();
        
        switch (choice) {
            case '1': 
                loadingScreen("Opening Add Product");
                addProduct(inventory); 
                break;
            case '2': 
                loadingScreen("Loading Products");
                viewProducts(inventory); 
                break;
            case '3': 
                loadingScreen("Opening Update Product");
                updateProduct(inventory); 
                break;
            case '4': 
                loadingScreen("Opening Delete Product");
                deleteProduct(inventory); 
                break;
            case '5': 
                loadingScreen("Opening Search Product");
                searchProduct(inventory); 
                break;
            case '6': 
                loadingScreen("Returning to Main Menu");
                return;
            default:
                showError("Invalid choice. Try again.");
        }
    }
}

void handleSupplierMenu(vector<Supplier>& suppliers, const Staff& currentUser) {
    while (true) {
        displayMenuHeader("SUPPLIER MANAGEMENT");
        
        cout << CYAN << "┌─────────────────────────────────────────┐\n";
        cout << "│ " << YELLOW << "1. Add Supplier" << RESET << "                      │\n";
        cout << "│ " << YELLOW << "2. View All Suppliers" << RESET << "                │\n";
        cout << "│ " << YELLOW << "3. Update Supplier" << RESET << "                   │\n";
        cout << "│ " << YELLOW << "4. Delete Supplier" << RESET << "                   │\n";
        cout << "│ " << YELLOW << "5. Register Supplier Account" << RESET << "         │\n";
        cout << "│ " << YELLOW << "6. Back to Main Menu" << RESET << "                 │\n";
        cout << CYAN << "└─────────────────────────────────────────┘\n";
        cout << CYAN << "Select an option (1-6): " << RESET;
        
        char choice = singleInput();
        
        switch (choice) {
            case '1': 
                loadingScreen("Opening Add Supplier");
                addSupplier(suppliers); 
                break;
            case '2': 
                loadingScreen("Loading Suppliers");
                viewSuppliers(suppliers); 
                break;
            case '3': 
                loadingScreen("Opening Update Supplier");
                updateSupplier(suppliers); 
                break;
            case '4': 
                loadingScreen("Opening Delete Supplier");
                deleteSupplier(suppliers); 
                break;
            case '5': 
                loadingScreen("Opening Register Supplier Account");
                registerSupplier(SUPPLIERS_FILE, currentUser); 
                break;
            case '6': 
                loadingScreen("Returning to Main Menu");
                return;
            default:
                showError("Invalid choice. Try again.");
        }
    }
}

void handleOrderMenu(vector<Order>& orders, vector<Product>& inventory) {
    while (true) {
        displayMenuHeader("ORDER MANAGEMENT");
        
        cout << CYAN << "┌─────────────────────────────────────────┐\n";
        cout << "│ " << YELLOW << "1. Create Order" << RESET << "                      │\n";
        cout << "│ " << YELLOW << "2. View All Orders" << RESET << "                   │\n";
        cout << "│ " << YELLOW << "3. Update Order Status" << RESET << "               │\n";
        cout << "│ " << YELLOW << "4. Back to Main Menu" << RESET << "                 │\n";
        cout << CYAN << "└─────────────────────────────────────────┘\n";
        cout << CYAN << "Select an option (1-4): " << RESET;
        
        char choice = singleInput();
        
        switch (choice) {
            case '1': 
                loadingScreen("Opening Create Order");
                createOrder(orders, inventory); 
                break;
            case '2': 
                loadingScreen("Loading Orders");
                viewOrders(orders); 
                break;
            case '3': 
                loadingScreen("Opening Update Order Status");
                updateOrderStatus(orders); 
                break;
            case '4': 
                loadingScreen("Returning to Main Menu");
                return;
            default:
                showError("Invalid choice. Try again.");
        }
    }
}

void handleStaffMenu(vector<Staff>& staffList, const Staff& currentUser) {
    while (true) {
        displayMenuHeader("STAFF MANAGEMENT");
        
        cout << CYAN << "┌─────────────────────────────────────────┐\n";
        cout << "│ " << YELLOW << "1. View All Staff" << RESET << "                    │\n";
        cout << "│ " << YELLOW << "2. Update Staff" << RESET << "                      │\n";
        cout << "│ " << YELLOW << "3. Delete Staff" << RESET << "                      │\n";
        cout << "│ " << YELLOW << "4. Register New Staff" << RESET << "                │\n";
        cout << "│ " << YELLOW << "5. Back to Main Menu" << RESET << "                 │\n";
        cout << CYAN << "└─────────────────────────────────────────┘\n";
        cout << CYAN << "Select an option (1-5): " << RESET;
        
        char choice = singleInput();
        
        switch (choice) {
            case '1': 
                loadingScreen("Loading Staff List");
                viewStaffList(staffList); 
                break;
            case '2': 
                loadingScreen("Opening Update Staff");
                updateStaffMember(staffList, currentUser); 
                break;
            case '3': 
                loadingScreen("Opening Delete Staff");
                deleteStaffMember(staffList, currentUser); 
                break;
            case '4': 
                loadingScreen("Opening Register Staff");
                registerStaff(STAFF_FILE, currentUser); 
                break;
            case '5': 
                loadingScreen("Returning to Main Menu");
                return;
            default:
                showError("Invalid choice. Try again.");
        }
    }
}

void handleSupplierDashboard(Supplier& currentSupplier, vector<Product>& inventory) {
    while (true) {
        displayMenuHeader("SUPPLIER DASHBOARD");
        
        cout << CYAN << "┌─────────────────────────────────────────┐\n";
        cout << "│ " << YELLOW << "Welcome, " << currentSupplier.getName() << RESET << string(39 - currentSupplier.getName().length(), ' ') << "│\n";
        cout << CYAN << "├─────────────────────────────────────────┤\n";
        cout << "│ " << YELLOW << "1. View Profile" << RESET << "                      │\n";
        cout << "│ " << YELLOW << "2. Update Profile" << RESET << "                    │\n";
        cout << "│ " << YELLOW << "3. View Products" << RESET << "                     │\n";
        cout << "│ " << YELLOW << "4. Logout" << RESET << "                            │\n";
        cout << CYAN << "└─────────────────────────────────────────┘\n";
        cout << CYAN << "Select an option (1-4): " << RESET;
        
        char choice = singleInput();
        
        switch (choice) {
            case '1': 
                loadingScreen("Loading Profile");
                viewSupplierProfile(currentSupplier); 
                break;
            case '2': 
                loadingScreen("Opening Update Profile");
                updateSupplierProfile(currentSupplier); 
                break;
            case '3': 
                loadingScreen("Loading Products");
                viewSupplierProducts(inventory); 
                break;
            case '4': 
                logout();
                return;
            default:
                showError("Invalid choice. Try again.");
        }
    }
}

// Main function
int main() {
    // Seed random number generator
    srand(time(nullptr));
    
    // Display banner
    displayBanner();
    
    // Create default admin and supplier if needed
    createDefaultAdmin(STAFF_FILE);
    createDefaultSupplier(SUPPLIERS_FILE);
    
    // Create data files if they don't exist
    ofstream productsFile(PRODUCTS_FILE, ios::app);
    productsFile.close();
    
    ofstream suppliersFile(SUPPLIERS_FILE, ios::app);
    suppliersFile.close();
    
    ofstream ordersFile(ORDERS_FILE, ios::app);
    ordersFile.close();
    
    ofstream orderItemsFile(ORDER_ITEMS_FILE, ios::app);
    orderItemsFile.close();
    
    Staff currentUser;
    Supplier currentSupplier;
    bool isStaffLoggedIn = false;
    bool isSupplierLoggedIn = false;
    
    // Load data
    vector<Product> inventory = Product::loadAllFromFile(PRODUCTS_FILE);
    vector<Supplier> suppliers = Supplier::loadAllFromFile(SUPPLIERS_FILE);
    vector<Order> orders = Order::loadAllFromFile(ORDERS_FILE, ORDER_ITEMS_FILE);
    vector<Staff> staffList = Staff::loadAllFromFile(STAFF_FILE);
    
    while (true) {
        if (!isStaffLoggedIn && !isSupplierLoggedIn) {
            displayMenuHeader("BUSINESS MANAGEMENT SYSTEM");
            
            cout << CYAN << "┌─────────────────────────────────────────┐\n";
            cout << "│ " << YELLOW << "1. Staff Login" << RESET << "                        │\n";
            cout << "│ " << YELLOW << "2. Supplier Login" << RESET << "                     │\n";
            cout << "│ " << YELLOW << "3. Exit" << RESET << "                               │\n";
            cout << CYAN << "└─────────────────────────────────────────┘\n";
            cout << CYAN << "Select an option (1-3): " << RESET;
            
            char choice = singleInput();
            
            switch (choice) {
                case '1':
                    loadingScreen("Opening Staff Login");
                    if (staffLogin(STAFF_FILE, currentUser)) {
                        isStaffLoggedIn = true;
                    }
                    break;
                case '2':
                    loadingScreen("Opening Supplier Login");
                    if (supplierLogin(SUPPLIERS_FILE, currentSupplier)) {
                        isSupplierLoggedIn = true;
                    }
                    break;
                case '3':
                    loadingScreen("Exiting System");
                    showSuccess("Thank you for using the system!");
                    return 0;
                default:
                    showError("Invalid choice. Try again.");
            }
        } else if (isSupplierLoggedIn) {
            // Supplier is logged in
            handleSupplierDashboard(currentSupplier, inventory);
            isSupplierLoggedIn = false;
        } else {
            // Staff is logged in
            // Show menu based on role
            if (currentUser.getRole() == ADMIN) {
                displayMenuHeader("ADMIN MENU");
                
                cout << CYAN << "┌─────────────────────────────────────────┐\n";
                cout << "│ " << YELLOW << "Welcome, " << currentUser.getName() << RESET << string(39 - currentUser.getName().length(), ' ') << "│\n";
                cout << CYAN << "├─────────────────────────────────────────┤\n";
                cout << "│ " << YELLOW << "1. Product Management" << RESET << "                 │\n";
                cout << "│ " << YELLOW << "2. Supplier Management" << RESET << "                │\n";
                cout << "│ " << YELLOW << "3. Order Management" << RESET << "                   │\n";
                cout << "│ " << YELLOW << "4. Staff Management" << RESET << "                   │\n";
                cout << "│ " << YELLOW << "5. Logout" << RESET << "                             │\n";
                cout << "│ " << YELLOW << "6. Exit" << RESET << "                               │\n";
                cout << CYAN << "└─────────────────────────────────────────┘\n";
                cout << CYAN << "Select an option (1-6): " << RESET;
                
                char choice = singleInput();
                
                switch (choice) {
                    case '1': 
                        loadingScreen("Opening Product Management");
                        handleProductMenu(inventory); 
                        break;
                    case '2': 
                        loadingScreen("Opening Supplier Management");
                        handleSupplierMenu(suppliers, currentUser); 
                        break;
                    case '3': 
                        loadingScreen("Opening Order Management");
                        handleOrderMenu(orders, inventory); 
                        break;
                    case '4': 
                        loadingScreen("Opening Staff Management");
                        handleStaffMenu(staffList, currentUser); 
                        break;
                    case '5': 
                        logout();
                        isStaffLoggedIn = false;
                        break;
                    case '6':
                        loadingScreen("Exiting System");
                        showSuccess("Thank you for using the system!");
                        return 0;
                    default:
                        showError("Invalid choice. Try again.");
                }
            } else if (currentUser.getRole() == MANAGER) {
                displayMenuHeader("MANAGER MENU");
                
                cout << CYAN << "┌─────────────────────────────────────────┐\n";
                cout << "│ " << YELLOW << "Welcome, " << currentUser.getName() << RESET << string(39 - currentUser.getName().length(), ' ') << "│\n";
                cout << CYAN << "├─────────────────────────────────────────┤\n";
                cout << "│ " << YELLOW << "1. Product Management" << RESET << "                 │\n";
                cout << "│ " << YELLOW << "2. Supplier Management" << RESET << "                │\n";
                cout << "│ " << YELLOW << "3. Order Management" << RESET << "                   │\n";
                cout << "│ " << YELLOW << "4. Logout" << RESET << "                             │\n";
                cout << "│ " << YELLOW << "5. Exit" << RESET << "                               │\n";
                cout << CYAN << "└─────────────────────────────────────────┘\n";
                cout << CYAN << "Select an option (1-5): " << RESET;
                
                char choice = singleInput();
                
                switch (choice) {
                    case '1': 
                        loadingScreen("Opening Product Management");
                        handleProductMenu(inventory); 
                        break;
                    case '2': 
                        loadingScreen("Opening Supplier Management");
                        handleSupplierMenu(suppliers, currentUser); 
                        break;
                    case '3': 
                        loadingScreen("Opening Order Management");
                        handleOrderMenu(orders, inventory); 
                        break;
                    case '4': 
                        logout();
                        isStaffLoggedIn = false;
                        break;
                    case '5':
                        loadingScreen("Exiting System");
                        showSuccess("Thank you for using the system!");
                        return 0;
                    default:
                        showError("Invalid choice. Try again.");
                }
            } else { // STAFF
                displayMenuHeader("STAFF MENU");
                
                cout << CYAN << "┌─────────────────────────────────────────┐\n";
                cout << "│ " << YELLOW << "Welcome, " << currentUser.getName() << RESET << string(39 - currentUser.getName().length(), ' ') << "│\n";
                cout << CYAN << "├─────────────────────────────────────────┤\n";
                cout << "│ " << YELLOW << "1. View Products" << RESET << "                     │\n";
                cout << "│ " << YELLOW << "2. Search Product" << RESET << "                    │\n";
                cout << "│ " << YELLOW << "3. View Orders" << RESET << "                       │\n";
                cout << "│ " << YELLOW << "4. Create Order" << RESET << "                      │\n";
                cout << "│ " << YELLOW << "5. Logout" << RESET << "                            │\n";
                cout << "│ " << YELLOW << "6. Exit" << RESET << "                              │\n";
                cout << CYAN << "└─────────────────────────────────────────┘\n";
                cout << CYAN << "Select an option (1-6): " << RESET;
                
                char choice = singleInput();
                
                switch (choice) {
                    case '1': 
                        loadingScreen("Loading Products");
                        viewProducts(inventory); 
                        break;
                    case '2': 
                        loadingScreen("Opening Search Product");
                        searchProduct(inventory); 
                        break;
                    case '3': 
                        loadingScreen("Loading Orders");
                        viewOrders(orders); 
                        break;
                    case '4': 
                        loadingScreen("Opening Create Order");
                        createOrder(orders, inventory); 
                        break;
                    case '5': 
                        logout();
                        isStaffLoggedIn = false;
                        break;
                    case '6':
                        loadingScreen("Exiting System");
                        showSuccess("Thank you for using the system!");
                        return 0;
                    default:
                        showError("Invalid choice. Try again.");
                }
            }
        }
    }
    
    return 0;
}
