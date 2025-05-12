#ifndef STAFF_H
#define STAFF_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "utils.h"

using namespace std;

enum Role {
    ADMIN = 1,
    MANAGER = 2,
    STAFF = 3
};

class Staff {
private:
    static int nextID;
    int staffID;
    string username;
    string password;
    string name;
    string phone;
    string email;
    Role role;

public:
    Staff() {
        staffID = nextID++;
        username = "";
        password = "";
        name = "Unnamed";
        phone = "";
        email = "";
        role = STAFF;
    }

    Staff(string u, string p, string n, string ph, string e, Role r) {
        staffID = nextID++;
        username = u;
        password = p;
        name = n;
        phone = ph;
        email = e;
        role = r;
    }

    int getID() const { return staffID; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getName() const { return name; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    Role getRole() const { return role; }

    void setUsername(const string& newUsername) { username = newUsername; }
    void setPassword(const string& newPassword) { password = newPassword; }
    void setName(const string& newName) { name = newName; }
    void setPhone(const string& newPhone) { phone = newPhone; }
    void setEmail(const string& newEmail) { email = newEmail; }
    void setRole(Role newRole) { role = newRole; }

    // Simple authentication - compare passwords directly
    bool authenticate(const string& pwd) const {
        return password == pwd;
    }

    void display() const {
        cout << "┌─────────────────────────────────────────┐\n";
        cout << "│ " << CYAN << BOLD << "Staff ID: " << RESET << staffID << string(32 - to_string(staffID).length(), ' ') << "│\n";
        cout << "│ " << CYAN << BOLD << "Username: " << RESET << username << string(32 - username.length(), ' ') << "│\n";
        cout << "│ " << CYAN << BOLD << "Name: " << RESET << name << string(37 - name.length(), ' ') << "│\n";
        cout << "│ " << CYAN << BOLD << "Phone: " << RESET << phone << string(36 - phone.length(), ' ') << "│\n";
        cout << "│ " << CYAN << BOLD << "Email: " << RESET << email << string(36 - email.length(), ' ') << "│\n";
        cout << "│ " << CYAN << BOLD << "Role: " << RESET << getRoleString() << string(37 - getRoleString().length(), ' ') << "│\n";
        cout << "└─────────────────────────────────────────┘\n";
    }

    string getRoleString() const {
        switch (role) {
            case ADMIN: return "Admin";
            case MANAGER: return "Manager";
            case STAFF: return "Staff";
            default: return "Unknown";
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << staffID << "," << username << "," << password << "," 
                 << name << "," << phone << "," << email << "," << role << "\n";
            file.close();
        } else {
            cout << "Unable to open file for writing\n";
        }
    }

    static Staff loadFromFile(const string& filename, int id) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            int fileID;
            string fileUsername, filePassword, fileName, filePhone, fileEmail;
            int fileRole;
            
            getline(ss, field, ',');
            fileID = stoi(field);
            getline(ss, fileUsername, ',');
            getline(ss, filePassword, ',');
            getline(ss, fileName, ',');
            getline(ss, filePhone, ',');
            getline(ss, fileEmail, ',');
            getline(ss, field);
            fileRole = stoi(field);
            
            if (fileID == id) {
                Staff s;
                s.staffID = fileID;
                s.username = fileUsername;
                s.password = filePassword;
                s.name = fileName;
                s.phone = filePhone;
                s.email = fileEmail;
                s.role = static_cast<Role>(fileRole);
                return s;
            }
        }
        file.close();
        return Staff();
    }

    static vector<Staff> loadAllFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        vector<Staff> staffList;
        
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            int fileID;
            string fileUsername, filePassword, fileName, filePhone, fileEmail;
            int fileRole;
            
            getline(ss, field, ',');
            fileID = stoi(field);
            getline(ss, fileUsername, ',');
            getline(ss, filePassword, ',');
            getline(ss, fileName, ',');
            getline(ss, filePhone, ',');
            getline(ss, fileEmail, ',');
            getline(ss, field);
            fileRole = stoi(field);
            
            Staff s;
            s.staffID = fileID;
            s.username = fileUsername;
            s.password = filePassword;
            s.name = fileName;
            s.phone = filePhone;
            s.email = fileEmail;
            s.role = static_cast<Role>(fileRole);
            staffList.push_back(s);
        }
        file.close();
        return staffList;
    }

    static Staff findByUsername(const string& filename, const string& username) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            int fileID;
            string fileUsername, filePassword, fileName, filePhone, fileEmail;
            int fileRole;
            
            getline(ss, field, ',');
            fileID = stoi(field);
            getline(ss, fileUsername, ',');
            
            if (fileUsername == username) {
                getline(ss, filePassword, ',');
                getline(ss, fileName, ',');
                getline(ss, filePhone, ',');
                getline(ss, fileEmail, ',');
                getline(ss, field);
                fileRole = stoi(field);
                
                Staff s;
                s.staffID = fileID;
                s.username = fileUsername;
                s.password = filePassword;
                s.name = fileName;
                s.phone = filePhone;
                s.email = fileEmail;
                s.role = static_cast<Role>(fileRole);
                file.close();
                return s;
            } else {
                getline(ss, field);
            }
        }
        file.close();
        return Staff();
    }
};

int Staff::nextID = 1;

#endif
