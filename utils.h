#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <limits>
#include <random>
#include <iomanip>

#ifdef _WIN32
#include <conio.h>
#define CLEAR "cls"
#define getch _getch
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

// Color definitions
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[5m"
#define REVERSE "\033[7m"
#define HIDDEN "\033[8m"

// Foreground colors
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// Background colors
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

// Bright foreground colors
#define BRIGHT_BLACK "\033[90m"
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN "\033[96m"
#define BRIGHT_WHITE "\033[97m"

using namespace std;

// Clear the screen
void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

// Center text on the screen
void centerText(const string& text) {
    int screenWidth = 80;
    int padding = (screenWidth - text.length()) / 2;
    for (int i = 0; i < padding; ++i) cout << " ";
    cout << text << endl;
}

// Generate a random color
string randomColor() {
    string colors[] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
    return colors[rand() % 6];
}

// Enhanced loading screen with animation
void loadingScreen(const string& message = "Loading...") {
    const int barWidth = 50;
    const string progressChars = "▁▂▃▄▅▆▇█";
    clearScreen();
    
    cout << "\n\n";
    centerText(string(BOLD) + string(CYAN) + "╔══════════════════════════════════════════════════════════╗");
    centerText(string(BOLD) + string(CYAN) + "║                                                          ║");
    centerText(string(BOLD) + string(CYAN) + "║  " + string(YELLOW) + message + string(50 - message.length(), ' ') + string(CYAN) + "  ║");
    centerText(string(BOLD) + string(CYAN) + "║                                                          ║");
    centerText(string(BOLD) + string(CYAN) + "╚══════════════════════════════════════════════════════════╝");
    cout << "\n\n";

    // Animated progress bar
    for (int i = 0; i <= barWidth; ++i) {
        string progressBar = "[";
        for (int j = 0; j < i; ++j) progressBar += "█";
        for (int j = i; j < barWidth; ++j) progressBar += " ";
        progressBar += "]";
        
        centerText(string(BRIGHT_GREEN) + progressBar + " " + to_string(int((i * 100) / barWidth)) + "% " + 
                  progressChars[i % progressChars.length()]);
        
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(20));
        
        if (i < barWidth) {
            cout << "\033[1A";  // Move cursor up one line
            cout << "\r";       // Move cursor to beginning of line
        }
    }

    this_thread::sleep_for(chrono::milliseconds(300));
    clearScreen();
}

// Get a single character input
char singleInput() {
    char ch = getch();
    cout << ch << endl;
    return ch;
}

// Get masked password input
string getMaskedPassword() {
    string password;
    char ch;
    
    while (true) {
        ch = getch();
        
        // Enter key pressed - end input
        if (ch == '\r' || ch == '\n') {
            cout << endl;
            break;
        }
        // Backspace pressed - remove last character
        else if (ch == '\b' || ch == 127) {
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
        // Regular character - add to password
        else {
            password.push_back(ch);
            cout << '*';
        }
    }
    
    return password;
}

// Wait for any key press to continue
void waitForAnyKey() {
    cout << "\n" << string(YELLOW) << "Press any key to continue..." << string(RESET);
    getch();  // Wait for any key
}

// Display a fancy banner
void displayBanner() {
    clearScreen();
    
    cout << "\n\n";
    cout << string(CYAN) << string(BOLD);
    cout << "╔═══════════════════════════════════════════════════════════════════╗\n";
    cout << "║                                                                   ║\n";
    cout << "║  " << string(BRIGHT_MAGENTA) << " ____   __  __  ____    __  __    ___   _   _    ___    ____  " << string(CYAN) << " ║\n";
    cout << "║  " << string(BRIGHT_MAGENTA) << "| __ ) |  \\/  |/ ___|  |  \\/  |  / _ \\ | \\ | |  / _ \\  / ___| " << string(CYAN) << " ║\n";
    cout << "║  " << string(BRIGHT_MAGENTA) << "|  _ \\ | |\\/| |\\___ \\  | |\\/| | | | | ||  \\| | | | | | \\___ \\ " << string(CYAN) << " ║\n";
    cout << "║  " << string(BRIGHT_MAGENTA) << "| |_) || |  | | ___) | | |  | | | |_| || |\\  | | |_| |  ___) |" << string(CYAN) << " ║\n";
    cout << "║  " << string(BRIGHT_MAGENTA) << "|____/ |_|  |_||____/  |_|  |_|  \\___/ |_| \\_|  \\___/  |____/ " << string(CYAN) << " ║\n";
    cout << "║                                                                   ║\n";
    cout << "╚═══════════════════════════════════════════════════════════════════╝\n";
    cout << string(RESET);
    
    cout << "\n";
    centerText(string(YELLOW) + string(BOLD) + "WAREHOUSE Management System" + string(RESET));
    cout << "\n";
    
    centerText(string(CYAN) + "╔═════════════════════════════════════════════════════╗");
    centerText(string(CYAN) + "║                  " + string(GREEN) + "Group Members" + string(CYAN) + "                   ║");
    centerText(string(CYAN) + "╠═════════════════════════════════════════════════════╣");
    centerText(string(CYAN) + "║ " + string(BRIGHT_WHITE) + "1. AREEBA TARIQ    - Roll Number: 23021519-099" + string(CYAN) + " ║");
    // centerText(string(CYAN) + "║ " + string(BRIGHT_WHITE) + "2. ABDULLAH NAEEM   - Roll Number: 230121519-088" + string(CYAN) + " ║");
    centerText(string(CYAN) + "╚═════════════════════════════════════════════════════╝");
    
    cout << "\n";
    centerText(string(YELLOW) + "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *");
    cout << "\n";
    
    centerText(string(BRIGHT_GREEN) + "Press any key to continue..." + string(RESET));
    getch();  // Wait for any key
}

// Display a success message
void showSuccess(const string& message) {
    clearScreen();
    cout << "\n\n";
    centerText(string(BOLD) + string(GREEN) + "╔══════════════════════════════════════════════════════════╗");
    centerText(string(BOLD) + string(GREEN) + "║                        SUCCESS                           ║");
    centerText(string(BOLD) + string(GREEN) + "╠══════════════════════════════════════════════════════════╣");
    centerText(string(BOLD) + string(GREEN) + "║                                                          ║");
    centerText(string(BOLD) + string(GREEN) + "║  " + string(WHITE) + message + string(50 - message.length(), ' ') + string(GREEN) + "  ║");
    centerText(string(BOLD) + string(GREEN) + "║                                                          ║");
    centerText(string(BOLD) + string(GREEN) + "╚══════════════════════════════════════════════════════════╝");
    cout << "\n\n";
    waitForAnyKey();
}

// Display an error message
void showError(const string& message) {
    clearScreen();
    cout << "\n\n";
    centerText(string(BOLD) + string(RED) + "╔══════════════════════════════════════════════════════════╗");
    centerText(string(BOLD) + string(RED) + "║                         ERROR                            ║");
    centerText(string(BOLD) + string(RED) + "╠══════════════════════════════════════════════════════════╣");
    centerText(string(BOLD) + string(RED) + "║                                                          ║");
    centerText(string(BOLD) + string(RED) + "║  " + string(WHITE) + message + string(50 - message.length(), ' ') + string(RED) + "  ║");
    centerText(string(BOLD) + string(RED) + "║                                                          ║");
    centerText(string(BOLD) + string(RED) + "╚══════════════════════════════════════════════════════════╝");
    cout << "\n\n";
    waitForAnyKey();
}

// Display a warning message
void showWarning(const string& message) {
    clearScreen();
    cout << "\n\n";
    centerText(string(BOLD) + string(YELLOW) + "╔══════════════════════════════════════════════════════════╗");
    centerText(string(BOLD) + string(YELLOW) + "║                        WARNING                           ║");
    centerText(string(BOLD) + string(YELLOW) + "╠══════════════════════════════════════════════════════════╣");
    centerText(string(BOLD) + string(YELLOW) + "║                                                          ║");
    centerText(string(BOLD) + string(YELLOW) + "║  " + string(WHITE) + message + string(50 - message.length(), ' ') + string(YELLOW) + "  ║");
    centerText(string(BOLD) + string(YELLOW) + "║                                                          ║");
    centerText(string(BOLD) + string(YELLOW) + "╚══════════════════════════════════════════════════════════╝");
    cout << "\n\n";
    waitForAnyKey();
}

// Display a fancy menu header
void displayMenuHeader(const string& title) {
    clearScreen();
    cout << "\n";
    centerText(string(BOLD) + string(CYAN) + "╔══════════════════════════════════════════════════════════╗");
    centerText(string(BOLD) + string(CYAN) + "║" + string((50 - title.length()) / 2, ' ') + string(BRIGHT_WHITE) + title + 
              string(CYAN) + string((50 - title.length() + 1) / 2, ' ') + "║");
    centerText(string(BOLD) + string(CYAN) + "╚══════════════════════════════════════════════════════════╝");
    cout << "\n";
}

#endif

