#include <iostream>
#include <vector>
#include <limits>
#include <thread>
#include <chrono>
#include <cstdlib>  // For system()
#ifdef _WIN32
#include <conio.h>
#define CLEAR "cls"
#else
#include <termios.h>
#include <unistd.h>
#define CLEAR "clear"
char getch() {
    struct termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

#include "product.h"
using namespace std;

void loadingScreen(string msg = "Loading") {
    cout << "\033[1;36m" << msg;
    for (int i = 0; i < 5; ++i) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(300));
    }
    cout << "\033[0m\n";
    this_thread::sleep_for(chrono::milliseconds(500));
    system(CLEAR);
}

void splashScreen() {
    system(CLEAR);
    cout << "\033[1;34m";
    cout << "==============================\n";
    cout << "  WAREHOUSE MANAGEMENT SYSTEM\n";
    cout << "==============================\n\n";
                                                                                                                                                    
    cout << "\033[0;32m";
    cout << "  Developed by: Areeba Tariq\n";
    cout << "  Roll No: 2301222131\n\n";

    cout << "\033[1;37mPress any key to continue...\033[0m\n";
#ifdef _WIN32
    _getch();
#else
    getch();
#endif
    loadingScreen("Initializing");
}

void showMenu() {
    cout << "\033[1;35m========= WAREHOUSE MENU =========\033[0m\n";
    cout << "1. Add Product\n";
    cout << "2. View All Products\n";
    cout << "3. Update Product\n";
    cout << "4. Delete Product\n";
    cout << "5. Search Product by ID\n";
    cout << "6. Exit\n";
    cout << "==================================\n";
    cout << "\033[1;36mSelect an option (1-6): \033[0m";
}

void addProduct(vector<Product>& inventory) {
    system(CLEAR);
    loadingScreen("Opening Add Product");

    string name;
    float price;
    int quantity;

    cout << "Enter Product Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Price: ";
    cin >> price;
    cout << "Enter Quantity: ";
    cin >> quantity;

    inventory.push_back(Product(name, price, quantity));
    cout << "\n✅ Product added!\n";
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void viewProducts(const vector<Product>& inventory) {
    system(CLEAR);
    loadingScreen("Fetching Inventory");

    if (inventory.empty()) {
        cout << "\n⚠️ No products available.\n";
    } else {
        cout << "\033[1;34m--- Product Inventory ---\033[0m\n\n";
        for (const auto& p : inventory)
            p.display();
    }
    this_thread::sleep_for(chrono::milliseconds(1000));
}

int findProductIndexByID(const vector<Product>& inventory, int id) {
    for (size_t i = 0; i < inventory.size(); ++i)
        if (inventory[i].getID() == id) return i;
    return -1;
}

void updateProduct(vector<Product>& inventory) {
    system(CLEAR);
    loadingScreen("Opening Update Module");

    int id;
    cout << "Enter Product ID to update: ";
    cin >> id;

    int idx = findProductIndexByID(inventory, id);
    if (idx == -1) {
        cout << "❌ Product not found.\n";
        return;
    }

    string name;
    float price;
    int quantity;
    cin.ignore();

    cout << "New Name: ";
    getline(cin, name);
    cout << "New Price: ";
    cin >> price;
    cout << "New Quantity: ";
    cin >> quantity;

    inventory[idx].setName(name);
    inventory[idx].setPrice(price);
    inventory[idx].setQuantity(quantity);

    cout << "✅ Product updated!\n";
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void deleteProduct(vector<Product>& inventory) {
    system(CLEAR);
    loadingScreen("Opening Delete Module");

    int id;
    cout << "Enter Product ID to delete: ";
    cin >> id;

    int idx = findProductIndexByID(inventory, id);
    if (idx == -1) {
        cout << "❌ Product not found.\n";
        return;
    }

    inventory.erase(inventory.begin() + idx);
    cout << "🗑️ Product deleted!\n";
    this_thread::sleep_for(chrono::milliseconds(1000));
}

void searchProduct(const vector<Product>& inventory) {
    system(CLEAR);
    loadingScreen("Searching Product");

    int id;
    cout << "Enter Product ID to search: ";
    cin >> id;

    int idx = findProductIndexByID(inventory, id);
    if (idx == -1) {
        cout << "❌ Product not found.\n";
    } else {
        cout << "\n🔍 Product Details:\n";
        inventory[idx].display();
    }
    this_thread::sleep_for(chrono::milliseconds(1000));
}

int main() {
    splashScreen();

    vector<Product> inventory;
    char choice;

    while (true) {
        system(CLEAR);
        showMenu();

#ifdef _WIN32
        choice = _getch();
#else
        choice = getch();
#endif
        cout << choice << "\n";
        loadingScreen();

        switch (choice) {
            case '1': addProduct(inventory); break;
            case '2': viewProducts(inventory); break;
            case '3': updateProduct(inventory); break;
            case '4': deleteProduct(inventory); break;
            case '5': searchProduct(inventory); break;
            case '6':
                cout << "\n👋 Exiting program...\n";
                this_thread::sleep_for(chrono::milliseconds(1000));
                return 0;
            default:
                cout << "⚠️ Invalid choice.\n";
                this_thread::sleep_for(chrono::milliseconds(1000));
        }
    }

    return 0;
}
