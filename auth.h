#ifndef AUTH_H
#define AUTH_H

#include <iostream>
#include <fstream>
#include <string>
#include "staff.h"
#include "supplier.h"
#include "utils.h"

using namespace std;

// Create default admin account if no staff exists
void createDefaultAdmin(const string& staffFile) {
    ifstream file(staffFile);
    if (!file || file.peek() == ifstream::traits_type::eof()) {
        // Create default admin if file doesn't exist or is empty
        Staff admin("admin", "admin123", "Administrator", "N/A", "admin@bms.com", ADMIN);
        admin.saveToFile(staffFile);
        showSuccess("Default admin account created.");
    }
    file.close();
}

// Create default supplier account if no suppliers exist
void createDefaultSupplier(const string& supplierFile) {
    ifstream file(supplierFile);
    if (!file || file.peek() == ifstream::traits_type::eof()) {
        // Create default supplier if file doesn't exist or is empty
        Supplier supplier("ABC Supplies", "John Doe", "555-1234", "john@abcsupplies.com", "123 Main St", "supplier", "supplier123");
        supplier.saveToFile(supplierFile);
        showSuccess("Default supplier account created.");
    }
    file.close();
}

// Staff login function
bool staffLogin(const string& staffFile, Staff& currentUser) {
    displayMenuHeader("STAFF LOGIN");
    
    // Get username
    string username;
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "Enter your username: " << RESET;
    cin >> username;
    
    // Clear input buffer
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    // Get password
    string password;
    cout << CYAN << "│ " << YELLOW << "Enter your password: " << RESET;
    password = getMaskedPassword();
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    // Find user by username
    Staff user = Staff::findByUsername(staffFile, username);
    if (user.getUsername().empty()) {
        showError("User not found!");
        return false;
    }
    
    // Authenticate
    if (user.authenticate(password)) {
        currentUser = user;
        loadingScreen("Logging in as " + user.getName());
        showSuccess("Login successful! Welcome, " + user.getName() + "!");
        return true;
    } else {
        showError("Invalid password!");
        return false;
    }
}

// Supplier login function
bool supplierLogin(const string& supplierFile, Supplier& currentSupplier) {
    displayMenuHeader("SUPPLIER LOGIN");
    
    // Get username
    string username;
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    cout << "│ " << YELLOW << "Enter your username: " << RESET;
    cin >> username;
    
    // Clear input buffer
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    // Get password
    string password;
    cout << CYAN << "│ " << YELLOW << "Enter your password: " << RESET;
    password = getMaskedPassword();
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    // Find supplier by username
    Supplier supplier = Supplier::findByUsername(supplierFile, username);
    if (supplier.getUsername().empty()) {
        showError("Supplier not found!");
        return false;
    }
    
    // Check if supplier is active
    if (supplier.getStatus() != SUPPLIER_ACTIVE) {
        showError("Your account is not active. Please contact the administrator.");
        return false;
    }
    
    // Authenticate
    if (supplier.authenticate(password)) {
        currentSupplier = supplier;
        loadingScreen("Logging in as " + supplier.getName());
        showSuccess("Login successful! Welcome, " + supplier.getName() + "!");
        return true;
    } else {
        showError("Invalid password!");
        return false;
    }
}

// Register new staff (admin only)
void registerStaff(const string& staffFile, const Staff& currentUser) {
    displayMenuHeader("REGISTER NEW STAFF");
    
    // Only admin can create new staff accounts
    if (currentUser.getRole() != ADMIN) {
        showError("Only administrators can create new staff accounts!");
        return;
    }
    
    string username, password, name, phone, email;
    int roleChoice;
    
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    
    // Clear input buffer
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "Full Name: " << RESET;
    getline(cin, name);
    
    cout << "│ " << YELLOW << "Username: " << RESET;
    cin >> username;
    
    // Check if username already exists
    Staff existingUser = Staff::findByUsername(staffFile, username);
    if (!existingUser.getUsername().empty()) {
        cout << CYAN << "└─────────────────────────────────────────┘\n";
        showError("Username already exists!");
        return;
    }
    
    // Clear input buffer
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "Password: " << RESET;
    password = getMaskedPassword();
    
    cout << "│ " << YELLOW << "Phone: " << RESET;
    getline(cin, phone);
    
    cout << "│ " << YELLOW << "Email: " << RESET;
    getline(cin, email);
    
    cout << "│ " << YELLOW << "Role (1=Admin, 2=Manager, 3=Staff): " << RESET;
    cin >> roleChoice;
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    if (roleChoice < 1 || roleChoice > 3) {
        showWarning("Invalid role! Defaulting to Staff.");
        roleChoice = 3;
    }
    
    Role role = static_cast<Role>(roleChoice);
    Staff newStaff(username, password, name, phone, email, role);
    
    loadingScreen("Registering new staff member");
    
    newStaff.saveToFile(staffFile);
    showSuccess("Staff registered successfully!");
}

// Register new supplier (admin only)
void registerSupplier(const string& supplierFile, const Staff& currentUser) {
    displayMenuHeader("REGISTER NEW SUPPLIER");
    
    // Only admin can create new supplier accounts
    if (currentUser.getRole() != ADMIN) {
        showError("Only administrators can create new supplier accounts!");
        return;
    }
    
    string name, contactPerson, phone, email, address, username, password;
    
    cout << CYAN << "┌─────────────────────────────────────────┐\n";
    
    // Clear input buffer
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "Supplier Name: " << RESET;
    getline(cin, name);
    
    cout << "│ " << YELLOW << "Contact Person: " << RESET;
    getline(cin, contactPerson);
    
    cout << "│ " << YELLOW << "Phone: " << RESET;
    getline(cin, phone);
    
    cout << "│ " << YELLOW << "Email: " << RESET;
    getline(cin, email);
    
    cout << "│ " << YELLOW << "Address: " << RESET;
    getline(cin, address);
    
    cout << "│ " << YELLOW << "Username: " << RESET;
    cin >> username;
    
    // Check if username already exists
    Supplier existingSupplier = Supplier::findByUsername(supplierFile, username);
    if (!existingSupplier.getUsername().empty()) {
        cout << CYAN << "└─────────────────────────────────────────┘\n";
        showError("Username already exists!");
        return;
    }
    
    // Clear input buffer
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "│ " << YELLOW << "Password: " << RESET;
    password = getMaskedPassword();
    cout << CYAN << "└─────────────────────────────────────────┘\n";
    
    Supplier newSupplier(name, contactPerson, phone, email, address, username, password);
    
    loadingScreen("Registering new supplier");
    
    newSupplier.saveToFile(supplierFile);
    showSuccess("Supplier registered successfully!");
}

// Logout function
void logout() {
    loadingScreen("Logging out");
    showSuccess("Logged out successfully.");
}

#endif

