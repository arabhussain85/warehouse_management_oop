#ifndef STAFF_MANAGER_H
#define STAFF_MANAGER_H

#include <iostream>
#include <vector>
#include <fstream>
#include "staff.h"
#include "utils.h"

using namespace std;

// ========== Staff Management Functions ==========
void viewStaff(const vector<Staff>& staffList) {
    clearScreen();
    loadingScreen("Fetching Staff List");

    if (staffList.empty()) {
        cout << "\n" << YELLOW << "⚠️ No staff records available.\n" << RESET;
    } else {
        cout << CYAN << BOLD << "--- STAFF LIST ---\n\n" << RESET;
        for (const auto& s : staffList)
            s.display();
    }
    waitForEnter();
}

int findStaffIndexByID(const vector<Staff>& staffList, int id) {
    for (size_t i = 0; i < staffList.size(); ++i)
        if (staffList[i].getID() == id) return i;
    return -1;
}

void updateStaff(vector<Staff>& staffList, const string& filename, const Staff& currentUser) {
    clearScreen();
    loadingScreen("Opening Update Staff");

    // Only admin can update staff
    if (currentUser.getRole() != ADMIN) {
        cout << RED << "❌ Only administrators can update staff records.\n" << RESET;
        waitForEnter();
        return;
    }

    int id;
    cout << CYAN << BOLD << "UPDATE STAFF\n" << RESET;
    cout << "------------------------\n";
    cout << "Enter Staff ID to update: ";
    cin >> id;

    int idx = findStaffIndexByID(staffList, id);
    if (idx == -1) {
        cout << RED << "❌ Staff not found.\n" << RESET;
        waitForEnter();
        return;
    }

    string name, phone, email;
    int roleChoice;
    cin.ignore();

    cout << "Current Name: " << staffList[idx].getName() << "\n";
    cout << "New Name (press Enter to keep current): ";
    getline(cin, name);
    if (name.empty()) name = staffList[idx].getName();

    cout << "Current Phone: " << staffList[idx].getPhone() << "\n";
    cout << "New Phone (press Enter to keep current): ";
    getline(cin, phone);
    if (phone.empty()) phone = staffList[idx].getPhone();

    cout << "Current Email: " << staffList[idx].getEmail() << "\n";
    cout << "New Email (press Enter to keep current): ";
    getline(cin, email);
    if (email.empty()) email = staffList[idx].getEmail();

    cout << "Current Role: " << staffList[idx].getRoleString() << "\n";
    cout << "New Role (1=Admin, 2=Manager, 3=Staff, press Enter to keep current): ";
    string roleStr;
    getline(cin, roleStr);
    if (roleStr.empty()) {
        roleChoice = staffList[idx].getRole();
    } else {
        roleChoice = stoi(roleStr);
        if (roleChoice < 1 || roleChoice > 3) {
            cout << RED << "Invalid role! Keeping current role.\n" << RESET;
            roleChoice = staffList[idx].getRole();
        }
    }

    staffList[idx].setName(name);
    staffList[idx].setPhone(phone);
    staffList[idx].setEmail(email);
    staffList[idx].setRole(static_cast<Role>(roleChoice));

    // Update the file
    ofstream file(filename);
    for (const auto& staff : staffList) {
        file << staff.getID() << "," << staff.getUsername() << "," 
             << staff.getPasswordHash() << "," << staff.getName() << ","
             << staff.getPhone() << "," << staff.getEmail() << ","
             << staff.getRole() << "\n";
    }
    file.close();

    cout << GREEN << "✅ Staff record updated successfully!\n" << RESET;
    waitForEnter();
}

void deleteStaff(vector<Staff>& staffList, const string& filename, const Staff& currentUser) {
    clearScreen();
    loadingScreen("Opening Delete Staff");

    // Only admin can delete staff
    if (currentUser.getRole() != ADMIN) {
        cout << RED << "❌ Only administrators can delete staff records.\n" << RESET;
        waitForEnter();
        return;
    }

    int id;
    cout << CYAN << BOLD << "DELETE STAFF\n" << RESET;
    cout << "------------------------\n";
    cout << "Enter Staff ID to delete: ";
    cin >> id;

    // Prevent deleting yourself
    if (id == currentUser.getID()) {
        cout << RED << "❌ You cannot delete your own account.\n" << RESET;
        waitForEnter();
        return;
    }

    int idx = findStaffIndexByID(staffList, id);
    if (idx == -1) {
        cout << RED << "❌ Staff not found.\n" << RESET;
        waitForEnter();
        return;
    }

    staffList.erase(staffList.begin() + idx);
    
    // Update the file
    ofstream file(filename);
    for (const auto& staff : staffList) {
        file << staff.getID() << "," << staff.getUsername() << "," 
             << staff.getPasswordHash() << "," << staff.getName() << ","
             << staff.getPhone() << "," << staff.getEmail() << ","
             << staff.getRole() << "\n";
    }
    file.close();

    cout << GREEN << "🗑️ Staff record deleted successfully!\n" << RESET;
    waitForEnter();
}

void signup(const string& filename, const Staff& currentUser) {
    clearScreen();
    loadingScreen("Opening Registration Screen");
    
    // Only admin can create new staff accounts
    if (currentUser.getRole() != ADMIN) {
        cout << RED << "Only administrators can create new staff accounts!\n" << RESET;
        waitForEnter();
        return;
    }
    
    string username, password, name, phone, email;
    int roleChoice;
    
    cout << CYAN << BOLD << "REGISTER NEW STAFF\n" << RESET;
    cout << "------------------------\n";
    
    cin.ignore();
    cout << "Full Name: ";
    getline(cin, name);
    
    cout << "Username: ";
    cin >> username;
    
    // Check if username already exists
    Staff existingUser = Staff::findByUsername(filename, username);
    if (!existingUser.getUsername().empty()) {
        cout << RED << "Username already exists!\n" << RESET;
        waitForEnter();
        return;
    }
    
    cout << "Password: ";
    password = getMaskedPassword();
    
    cin.ignore();
    cout << "Phone: ";
    getline(cin, phone);
    
    cout << "Email: ";
    getline(cin, email);
    
    cout << "Role (1=Admin, 2=Manager, 3=Staff): ";
    cin >> roleChoice;
    
    if (roleChoice < 1 || roleChoice > 3) {
        cout << RED << "Invalid role! Defaulting to Staff.\n" << RESET;
        roleChoice = 3;
    }
    
    Role role = static_cast<Role>(roleChoice);
    Staff newStaff(username, password, name, phone, email, role);
    newStaff.saveToFile(filename);
    
    cout << GREEN << "Staff registered successfully!\n" << RESET;
    waitForEnter();
}

void showStaffManagementMenu() {
    clearScreen();
    cout << MAGENTA << BOLD << "========= STAFF MANAGEMENT =========\n" << RESET;
    cout << "1. View All Staff\n";
    cout << "2. Update Staff\n";
    cout << "3. Delete Staff\n";
    cout << "4. Back to Main Menu\n";
    cout << "====================================\n";
    cout << CYAN << "Select an option (1-4): " << RESET;
}

void handleStaffManagementMenu(const string& staffFile, const Staff& currentUser) {
    vector<Staff> staffList = Staff::loadAllFromFile(staffFile);
    char choice;
    
    while (true) {
        showStaffManagementMenu();
        choice = singleInput();
        
        switch (choice) {
            case '1': viewStaff(staffList); break;
            case '2': updateStaff(staffList, staffFile, currentUser); break;
            case '3': deleteStaff(staffList, staffFile, currentUser); break;
            case '4': return;
            default:
                cout << RED << "Invalid choice. Try again.\n" << RESET;
                waitForEnter();
        }
    }
}

#endif
