#ifndef SUPPLIER_MANAGER_H
#define SUPPLIER_MANAGER_H

#include <iostream>
#include <vector>
#include <fstream>
#include "supplier.h"
#include "utils.h"

using namespace std;

// ========== Supplier Management Functions ==========
void addSupplier(vector<Supplier>& suppliers, const string& filename) {
    clearScreen();
    loadingScreen("Opening Add Supplier");

    string name, contactPerson, phone, email, address;

    cout << CYAN << BOLD << "ADD NEW SUPPLIER\n" << RESET;
    cout << "------------------------\n";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Supplier Name: ";
    getline(cin, name);
    cout << "Enter Contact Person: ";
    getline(cin, contactPerson);
    cout << "Enter Phone: ";
    getline(cin, phone);
    cout << "Enter Email: ";
    getline(cin, email);
    cout << "Enter Address: ";
    getline(cin, address);

    Supplier newSupplier(name, contactPerson, phone, email, address);
    suppliers.push_back(newSupplier);
    newSupplier.saveToFile(filename);
    
    cout << "\n" << GREEN << "✅ Supplier added successfully!\n" << RESET;
    waitForEnter();
}

void viewSuppliers(const vector<Supplier>& suppliers) {
    clearScreen();
    loadingScreen("Fetching Suppliers");

    if (suppliers.empty()) {
        cout << "\n" << YELLOW << "⚠️ No suppliers available.\n" << RESET;
    } else {
        cout << CYAN << BOLD << "--- SUPPLIER LIST ---\n\n" << RESET;
        for (const auto& s : suppliers)
            s.display();
    }
    waitForEnter();
}

int findSupplierIndexByID(const vector<Supplier>& suppliers, int id) {
    for (size_t i = 0; i < suppliers.size(); ++i)
        if (suppliers[i].getID() == id) return i;
    return -1;
}

void updateSupplier(vector<Supplier>& suppliers, const string& filename) {
    clearScreen();
    loadingScreen("Opening Update Supplier");

    int id;
    cout << CYAN << BOLD << "UPDATE SUPPLIER\n" << RESET;
    cout << "------------------------\n";
    cout << "Enter Supplier ID to update: ";
    cin >> id;

    int idx = findSupplierIndexByID(suppliers, id);
    if (idx == -1) {
        cout << RED << "❌ Supplier not found.\n" << RESET;
        waitForEnter();
        return;
    }

    string name, contactPerson, phone, email, address;
    cin.ignore();

    cout << "Current Name: " << suppliers[idx].getName() << "\n";
    cout << "New Name (press Enter to keep current): ";
    getline(cin, name);
    if (name.empty()) name = suppliers[idx].getName();

    cout << "Current Contact Person: " << suppliers[idx].getContactPerson() << "\n";
    cout << "New Contact Person (press Enter to keep current): ";
    getline(cin, contactPerson);
    if (contactPerson.empty()) contactPerson = suppliers[idx].getContactPerson();

    cout << "Current Phone: " << suppliers[idx].getPhone() << "\n";
    cout << "New Phone (press Enter to keep current): ";
    getline(cin, phone);
    if (phone.empty()) phone = suppliers[idx].getPhone();

    cout << "Current Email: " << suppliers[idx].getEmail() << "\n";
    cout << "New Email (press Enter to keep current): ";
    getline(cin, email);
    if (email.empty()) email = suppliers[idx].getEmail();

    cout << "Current Address: " << suppliers[idx].getAddress() << "\n";
    cout << "New Address (press Enter to keep current): ";
    getline(cin, address);
    if (address.empty()) address = suppliers[idx].getAddress();

    suppliers[idx].setName(name);
    suppliers[idx].setContactPerson(contactPerson);
    suppliers[idx].setPhone(phone);
    suppliers[idx].setEmail(email);
    suppliers[idx].setAddress(address);

    // Update the file
    ofstream file(filename);
    for (const auto& supplier : suppliers) {
        file << supplier.getID() << "," << supplier.getName() << "," 
             << supplier.getContactPerson() << "," << supplier.getPhone() << ","
             << supplier.getEmail() << "," << supplier.getAddress() << "\n";
    }
    file.close();

    cout << GREEN << "✅ Supplier updated successfully!\n" << RESET;
    waitForEnter();
}

void deleteSupplier(vector<Supplier>& suppliers, const string& filename) {
    clearScreen();
    loadingScreen("Opening Delete Supplier");

    int id;
    cout << CYAN << BOLD << "DELETE SUPPLIER\n" << RESET;
    cout << "------------------------\n";
    cout << "Enter Supplier ID to delete: ";
    cin >> id;

    int idx = findSupplierIndexByID(suppliers, id);
    if (idx == -1) {
        cout << RED << "❌ Supplier not found.\n" << RESET;
        waitForEnter();
        return;
    }

    suppliers.erase(suppliers.begin() + idx);
    
    // Update the file
    ofstream file(filename);
    for (const auto& supplier : suppliers) {
        file << supplier.getID() << "," << supplier.getName() << "," 
             << supplier.getContactPerson() << "," << supplier.getPhone() << ","
             << supplier.getEmail() << "," << supplier.getAddress() << "\n";
    }
    file.close();

    cout << GREEN << "🗑️ Supplier deleted successfully!\n" << RESET;
    waitForEnter();
}

void showSupplierMenu() {
    clearScreen();
    cout << MAGENTA << BOLD << "========= SUPPLIER MANAGEMENT =========\n" << RESET;
    cout << "1. Add Supplier\n";
    cout << "2. View All Suppliers\n";
    cout << "3. Update Supplier\n";
    cout << "4. Delete Supplier\n";
    cout << "5. Back to Main Menu\n";
    cout << "========================================\n";
    cout << CYAN << "Select an option (1-5): " << RESET;
}

void handleSupplierMenu(const string& suppliersFile) {
    vector<Supplier> suppliers = Supplier::loadAllFromFile(suppliersFile);
    char choice;
    
    while (true) {
        showSupplierMenu();
        choice = singleInput();
        
        switch (choice) {
            case '1': addSupplier(suppliers, suppliersFile); break;
            case '2': viewSuppliers(suppliers); break;
            case '3': updateSupplier(suppliers, suppliersFile); break;
            case '4': deleteSupplier(suppliers, suppliersFile); break;
            case '5': return;
            default:
                cout << RED << "Invalid choice. Try again.\n" << RESET;
                waitForEnter();
        }
    }
}

#endif
