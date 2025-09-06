#include <regex>
#include <iostream>
#include "models.h"
#include "data_manager.h"
#include <vector>
#include <limits>

void showMainMenu() {
    std::cout << "\n";
    std::cout << "==========================================\n";
    std::cout << "|      MOTORBIKE RENTAL APPLICATION       |\n";
    std::cout << "==========================================\n";
    std::cout << "| 1. Guest                               |\n";
    std::cout << "| 2. Member                              |\n";
    std::cout << "| 3. Admin                               |\n";
    std::cout << "| 4. Exit                                |\n";
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
    std::cout << "ID number: ";
    std::getline(std::cin, u.idNumber);
    std::cout << "License number (optional): ";
    std::getline(std::cin, u.licenseNumber);
    std::cout << "License expiry (YYYYMMDD, 0 if none): ";
    std::string expiryStr;
    std::getline(std::cin, expiryStr);
    u.licenseExpiry = std::stol(expiryStr);
    u.role = UserRole::Member;
    u.creditPoints = 20; // Registration fee
    u.rating = 3; // Default rating
    u.ownedMotorbikeLicense = "";
    u.rentingMotorbikeLicense = "";
    users.push_back(u);
    DataManager::saveUsers("users.csv", users);
    std::cout << "Registration successful! You received 20 CP.\n";
}

int main() {
    int choice;
    do {
        showMainMenu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                std::cout << "[Guest menu - coming soon]\n" << std::endl;
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
