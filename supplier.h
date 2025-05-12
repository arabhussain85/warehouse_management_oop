#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

enum SupplierStatus {
    SUPPLIER_ACTIVE = 1,
    SUPPLIER_INACTIVE = 2,
    SUPPLIER_PENDING = 3
};

class Supplier {
private:
    static int nextID;
    int supplierID;
    string name;
    string contactPerson;
    string phone;
    string email;
    string address;
    string username;
    string password;
    SupplierStatus status;

public:
    Supplier() {
        supplierID = nextID++;
        name = "Unnamed";
        contactPerson = "";
        phone = "";
        email = "";
        address = "";
        username = "";
        password = "";
        status = SUPPLIER_PENDING;
    }

    Supplier(string n, string cp, string p, string e, string a, string u = "", string pwd = "") {
        supplierID = nextID++;
        name = n;
        contactPerson = cp;
        phone = p;
        email = e;
        address = a;
        username = u;
        password = pwd;
        status = SUPPLIER_ACTIVE;
    }

    int getID() const { return supplierID; }
    string getName() const { return name; }
    string getContactPerson() const { return contactPerson; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    string getAddress() const { return address; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    SupplierStatus getStatus() const { return status; }

    void setName(const string& newName) { name = newName; }
    void setContactPerson(const string& newCP) { contactPerson = newCP; }
    void setPhone(const string& newPhone) { phone = newPhone; }
    void setEmail(const string& newEmail) { email = newEmail; }
    void setAddress(const string& newAddress) { address = newAddress; }
    void setUsername(const string& newUsername) { username = newUsername; }
    void setPassword(const string& newPassword) { password = newPassword; }
    void setStatus(SupplierStatus newStatus) { status = newStatus; }

    bool authenticate(const string& pwd) const {
        return password == pwd;
    }

    string getStatusString() const {
        switch (status) {
            case SUPPLIER_ACTIVE: return "Active";
            case SUPPLIER_INACTIVE: return "Inactive";
            case SUPPLIER_PENDING: return "Pending";
            default: return "Unknown";
        }
    }

    void display() const {
        cout << "┌───────────────────────────────────────────────────┐\n";
        cout << "│ " << CYAN << BOLD << "Supplier ID: " << RESET << setw(10) << left << supplierID << "                          │\n";
        cout << "│ " << CYAN << BOLD << "Name: " << RESET << setw(42) << left << name << " │\n";
        cout << "│ " << CYAN << BOLD << "Contact Person: " << RESET << setw(32) << left << contactPerson << " │\n";
        cout << "│ " << CYAN << BOLD << "Phone: " << RESET << setw(41) << left << phone << " │\n";
        cout << "│ " << CYAN << BOLD << "Email: " << RESET << setw(41) << left << email << " │\n";
        cout << "│ " << CYAN << BOLD << "Address: " << RESET << setw(39) << left << address << " │\n";
        cout << "│ " << CYAN << BOLD << "Status: " << RESET << setw(40) << left << getStatusString() << " │\n";
        cout << "└───────────────────────────────────────────────────┘\n";
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << supplierID << "," << name << "," << contactPerson << "," 
                 << phone << "," << email << "," << address << ","
                 << username << "," << password << "," << status << "\n";
            file.close();
        } else {
            cout << "Unable to open file for writing\n";
        }
    }

    static Supplier loadFromFile(const string& filename, int id) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            int fileID;
            string fileName, fileCP, filePhone, fileEmail, fileAddress, fileUsername, filePassword;
            int fileStatus;
            
            getline(ss, field, ',');
            fileID = stoi(field);
            getline(ss, fileName, ',');
            getline(ss, fileCP, ',');
            getline(ss, filePhone, ',');
            getline(ss, fileEmail, ',');
            getline(ss, fileAddress, ',');
            getline(ss, fileUsername, ',');
            getline(ss, filePassword, ',');
            getline(ss, field);
            fileStatus = stoi(field);
            
            if (fileID == id) {
                Supplier s;
                s.supplierID = fileID;
                s.name = fileName;
                s.contactPerson = fileCP;
                s.phone = filePhone;
                s.email = fileEmail;
                s.address = fileAddress;
                s.username = fileUsername;
                s.password = filePassword;
                s.status = static_cast<SupplierStatus>(fileStatus);
                return s;
            }
        }
        file.close();
        return Supplier();
    }

    static vector<Supplier> loadAllFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        vector<Supplier> suppliers;
        
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            int fileID;
            string fileName, fileCP, filePhone, fileEmail, fileAddress, fileUsername, filePassword;
            int fileStatus;
            
            getline(ss, field, ',');
            fileID = stoi(field);
            getline(ss, fileName, ',');
            getline(ss, fileCP, ',');
            getline(ss, filePhone, ',');
            getline(ss, fileEmail, ',');
            getline(ss, fileAddress, ',');
            getline(ss, fileUsername, ',');
            getline(ss, filePassword, ',');
            getline(ss, field);
            fileStatus = stoi(field);
            
            Supplier s;
            s.supplierID = fileID;
            s.name = fileName;
            s.contactPerson = fileCP;
            s.phone = filePhone;
            s.email = fileEmail;
            s.address = fileAddress;
            s.username = fileUsername;
            s.password = filePassword;
            s.status = static_cast<SupplierStatus>(fileStatus);
            suppliers.push_back(s);
        }
        file.close();
        return suppliers;
    }

    static Supplier findByUsername(const string& filename, const string& username) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string field;
            int fileID;
            string fileName, fileCP, filePhone, fileEmail, fileAddress, fileUsername, filePassword;
            int fileStatus;
            
            getline(ss, field, ',');
            fileID = stoi(field);
            getline(ss, fileName, ',');
            getline(ss, fileCP, ',');
            getline(ss, filePhone, ',');
            getline(ss, fileEmail, ',');
            getline(ss, fileAddress, ',');
            getline(ss, fileUsername, ',');
            
            if (fileUsername == username) {
                getline(ss, filePassword, ',');
                getline(ss, field);
                fileStatus = stoi(field);
                
                Supplier s;
                s.supplierID = fileID;
                s.name = fileName;
                s.contactPerson = fileCP;
                s.phone = filePhone;
                s.email = fileEmail;
                s.address = fileAddress;
                s.username = fileUsername;
                s.password = filePassword;
                s.status = static_cast<SupplierStatus>(fileStatus);
                file.close();
                return s;
            } else {
                getline(ss, field);
            }
        }
        file.close();
        return Supplier();
    }
};

int Supplier::nextID = 1;

#endif
