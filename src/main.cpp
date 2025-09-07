#include <regex>
#include <iostream>
#include "models.h"
#include "data_manager.h"
#include <vector>
#include <limits>

// Function prototype
void registerMember();
#include <regex>
#include <iostream>
#include "models.h"
#include "data_manager.h"
#include <vector>
#include <limits>

void showGuestMenu() {
    std::cout << std::endl;
    std::cout << "===============================================|" << std::endl;
    std::cout << "|                GUEST MENU                    |" << std::endl;
    std::cout << "===============================================|" << std::endl;
    std::cout << "| 1. Register as a new member                  |" << std::endl;
    std::cout << "| 2. View motorbike list                       |" << std::endl;
    std::cout << "| 3. Back to main menu                         |" << std::endl;
    std::cout << "===============================================|" << std::endl;
    std::cout << "Please select an option: ";
}

void guestMenuLoop() {
    int choice;
    do {
        showGuestMenu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                registerMember();
                break;
            case 2:
                std::cout << "[Show motorbike list - coming soon]\n";
                break;
            case 3:
                std::cout << "Returning to main menu...\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 3);
}


void showMainMenu() {
    std::cout << "\n";
    std::cout << "==========================================\n";
    std::cout << "|      MOTORBIKE RENTAL APPLICATION       |\n";
    std::cout << "==========================================\n";
    std::cout << "| 1. Guest                                |\n";
    std::cout << "| 2. Member                               |\n";
    std::cout << "| 3. Admin                                |\n";
    std::cout << "| 4. Exit                                 |\n";
    std::cout << "==========================================\n";
    std::cout << "Please select an option: ";
}




// Improved password strength check: return all reasons
std::vector<std::string> getPasswordWeaknesses(const std::string& pw) {
    std::vector<std::string> reasons;
    if (pw.length() < 8) reasons.push_back("Password must be at least 8 characters.");
    if (pw == "12345" || pw == "password" || pw == "12345678") reasons.push_back("Password is too common.");
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    std::string specialChars = "!@#$%^&*()-_=+[{]}\\|;:'\",<.>/?`~";
    for (char c : pw) {
        if (std::isupper(c)) hasUpper = true;
        if (std::islower(c)) hasLower = true;
        if (std::isdigit(c)) hasDigit = true;
        if (specialChars.find(c) != std::string::npos) hasSpecial = true;
    }
    if (!hasUpper) reasons.push_back("Password must contain at least one uppercase letter.");
    if (!hasLower) reasons.push_back("Password must contain at least one lowercase letter.");
    if (!hasDigit) reasons.push_back("Password must contain at least one digit.");
    if (!hasSpecial) reasons.push_back("Password must contain at least one special character.");
    return reasons;
}

void registerMember() {
    std::vector<User> users = DataManager::loadUsers("users.csv");
    User u;
    std::cout << "\n--- Register New Member ---\n";
    std::cout << "Username: ";
    std::cin >> u.username;
    // Check username exists
    for (const auto& user : users) {
        if (user.username == u.username) {
            std::cout << "Username already exists!\n";
            return;
        }
    }
    // Loop for password only, not whole registration
    while (true) {
        std::cout << "Password: ";
        std::cin >> u.password;
        std::vector<std::string> weaknesses = getPasswordWeaknesses(u.password);
        if (weaknesses.empty()) break;
        std::cout << "Password is too weak for the following reasons:\n";
        for (const auto& w : weaknesses) {
            std::cout << "- " << w << "\n";
        }
        // No ask, just loop until valid
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Full name: ";
    std::getline(std::cin, u.fullName);

    // Email validation
    while (true) {
        std::cout << "Email: ";
        std::getline(std::cin, u.email);
        // Simple regex for email validation
        std::regex email_regex(R"(^[\w.-]+@[\w.-]+\.[a-zA-Z]{2,}$)");
        if (std::regex_match(u.email, email_regex)) break;
        std::cout << "Invalid email format! Please try again.\n";
    }

    // Phone validation
    while (true) {
        std::cout << "Phone: ";
        std::getline(std::cin, u.phone);
        bool valid = true;
        if (u.phone.length() < 9 || u.phone.length() > 11) valid = false;
        for (char c : u.phone) if (!isdigit(c)) valid = false;
        if (valid) break;
        std::cout << "Invalid phone number! Only digits, 9-11 numbers.\n";
    }
    // ID type selection
    while (true) {
        std::cout << "ID type: [1] CCCD   [2] Passport. Please select (1/2): ";
        std::string idChoice;
        std::getline(std::cin, idChoice);
        if (idChoice == "1") { u.idType = "CCCD"; break; }
        if (idChoice == "2") { u.idType = "Passport"; break; }
        std::cout << "Invalid choice! Please enter 1 or 2.\n";
    }
    // ID number validation
    while (true) {
        std::cout << "ID number: ";
        std::getline(std::cin, u.idNumber);
        bool valid = true;
        std::string reason;
        if (u.idType == "CCCD") {
            if (u.idNumber.length() != 12) { valid = false; reason = "CCCD must be exactly 12 digits."; }
            for (char c : u.idNumber) if (!isdigit(c)) { valid = false; reason = "CCCD must contain only digits."; break; }
        } else if (u.idType == "Passport") {
            if (u.idNumber.length() != 8) { valid = false; reason = "Passport must be exactly 8 characters."; }
            else if (!isalpha(u.idNumber[0])) { valid = false; reason = "Passport must start with a letter."; }
            else {
                for (size_t i = 1; i < u.idNumber.length(); ++i) if (!isdigit(u.idNumber[i])) { valid = false; reason = "Passport: after the first letter, all must be digits."; break; }
            }
        }
        if (valid) break;
        std::cout << "Invalid ID number! " << reason << "\n";
    }
    std::cout << "License number (optional): ";
    std::getline(std::cin, u.licenseNumber);
    // License expiry validation (DD/MM/YYYY)
    while (true) {
        std::cout << "License expiry (DD/MM/YYYY): ";
        std::string expiryStr;
        std::getline(std::cin, expiryStr);
        if (expiryStr.length() != 10 || expiryStr[2] != '/' || expiryStr[5] != '/') {
            std::cout << "Invalid date format! Must be DD/MM/YYYY.\n";
            continue;
        }
        std::string dayStr = expiryStr.substr(0,2);
        std::string monthStr = expiryStr.substr(3,2);
        std::string yearStr = expiryStr.substr(6,4);
        bool allDigits = true;
        for (char c : dayStr+monthStr+yearStr) if (!isdigit(c)) allDigits = false;
        if (!allDigits) {
            std::cout << "Invalid date format! Day, month, year must be digits.\n";
            continue;
        }
        int day = std::stoi(dayStr);
        int month = std::stoi(monthStr);
        int year = std::stoi(yearStr);
        bool valid = true;
        std::string reason;
        if (year < 1900 || year > 2100) { valid = false; reason = "Year must be between 1900 and 2100."; }
        else if (month < 1 || month > 12) { valid = false; reason = "Month must be between 1 and 12."; }
        else {
            int maxDay = 31;
            if (month == 2) {
                bool leap = (year%4==0 && (year%100!=0 || year%400==0));
                maxDay = leap ? 29 : 28;
            } else if (month==4 || month==6 || month==9 || month==11) maxDay = 30;
            if (day < 1 || day > maxDay) { valid = false; reason = "Invalid day for the given month/year."; }
        }
        // Check not in the past (today: 07/09/2025)
        long inputDate = std::stol(yearStr+monthStr+dayStr);
        long today = 20250907;
        if (valid && inputDate < today) {
            valid = false;
            reason = "License expiry cannot be in the past.";
        }
        if (valid) {
            // Lưu lại theo dạng YYYYMMDD để tiện xử lý nội bộ
            u.licenseExpiry = inputDate;
            break;
        }
        std::cout << "Invalid date! " << reason << "\n";
    }
    int deposit = 0;
    while (true) {
        std::cout << "Please enter the amount you want to deposit (minimum $20): ";
        std::string depositStr;
        std::getline(std::cin, depositStr);
        try {
            deposit = std::stoi(depositStr);
        } catch (...) {
            std::cout << "Invalid input! Please enter a valid number.\n";
            continue;
        }
        if (deposit < 20) {
            std::cout << "You must deposit at least $20 to register.\n";
            continue;
        }
        break;
    }
    std::cout << "Successfully deposited $" << deposit << ". You will receive " << deposit << " CP.\n";
    u.role = UserRole::Member;
    u.creditPoints = deposit;
    u.rating = 3; // Default rating
    u.ownedMotorbikeLicense = "";
    u.rentingMotorbikeLicense = "";
    users.push_back(u);
    DataManager::saveUsers("users.csv", users);
    std::cout << "Registration successful! Default renter rating is 3.\n";
}

int main() {
    int choice;
    do {
        showMainMenu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                guestMenuLoop();
                break;
            case 2:
                std::cout << "[Member menu - coming soon]\n" << std::endl;
                break;
            case 3:
                std::cout << "[Admin menu - coming soon]\n" << std::endl;
                break;
            case 4:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 4);
    return 0;
}
