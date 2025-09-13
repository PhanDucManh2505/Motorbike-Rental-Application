#include "utils.h"

#include "guest.h"
#include "models.h"
#include "data_manager.h"

#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <cctype>

// Gets a valid integer choice from the user within the specified range
int getGuestChoice(int min, int max){
    int n;
    while(true){
        std::cin >> n;
        if(std::cin.fail() || n < min || n > max){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return n;
        }
    }
}

void showGuestMenu() {
    std::cout << std::endl;
    std::cout << "===============================================|" << std::endl;
    std::cout << "|                GUEST MENU                    |" << std::endl;
    std::cout << "===============================================|" << std::endl;
    std::cout << "| 1. Register as a new member                  |" << std::endl;
    std::cout << "| 2. View EBike list                           |" << std::endl;
    std::cout << "| 3. Back to main menu                         |" << std::endl;
    std::cout << "===============================================|" << std::endl;
    std::cout << "Please select an option: ";
}

void guestMenuLoop() {
    int choice;
    
    do {
        
       
        showGuestMenu();
        choice = getGuestChoice(1, 3);
        /*
        std::string input;
        std::getline(std::cin, input);
        // Delet leading/trailing whitespace
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);
        // Check if input is all digits
        bool valid = !input.empty();
        for (char c : input) {
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "Invalid option. Please enter a number.\n";
            continue;
        }
        // Make sure to convert string to int safely
        choice = 0;
        for (char c : input) {
            choice = choice * 10 + (c - '0');
        }
            */
        
        switch (choice) {

            case 1: {
                // Register as a new member and save to users.csv
                std::vector<User> users = DataManager::loadUsers("users.csv");
                User newUser;
                std::cout << "\n--- Member Registration ---\n";
                // Username
                while (true) {
                    std::cout << "Username: ";
                    std::getline(std::cin, newUser.username);
                    newUser.username = trim(newUser.username);
                    if (newUser.username.empty()) {
                        std::cout << "Username cannot be empty!\n";
                        continue;
                    }
                    // Reject username containing any whitespace
                    if (std::any_of(newUser.username.begin(), newUser.username.end(), [](unsigned char c){ return std::isspace(c); })) {
                        std::cout << "Username cannot contain spaces or whitespace characters!\n";
                        continue;
                    }
                    bool exists = false;
                    for (const auto& u : users) {
                        if (u.username == newUser.username) { exists = true; break; }
                    }
                    if (exists) {
                        std::cout << "Username already exists!\n";
                        continue;
                    }
                    break;
                }
                // Password
                while (true) {
                    std::cout << "Password: ";
                    std::getline(std::cin, newUser.password);
                    // Check password strength and uniqueness
                    bool pwExists = false;
                    for (const auto& u : users) {
                        if (u.password == newUser.password) { pwExists = true; break; }
                    }
                    if (pwExists) {
                        std::cout << "Password already exists! Please choose another.\n";
                        continue;
                    }
                    std::vector<std::string> reasons = getPasswordWeaknesses(newUser.password);
                    if (!reasons.empty()) {
                        std::cout << "Password is weak!\n";
                        for (const auto& r : reasons) std::cout << "- " << r << "\n";
                        continue;
                    }
                    break;
                }
                // Full name
                std::cout << "Full name: ";
                std::getline(std::cin, newUser.fullName);
                // Email
                while (true) {
                    std::cout << "Email: ";
                    std::getline(std::cin, newUser.email);
                    newUser.email = trim(newUser.email);
                    auto atPos = newUser.email.find('@');
                    auto dotPos = newUser.email.find('.', atPos != std::string::npos ? atPos : 0);
                    if (atPos == std::string::npos || dotPos == std::string::npos || atPos == 0 || dotPos <= atPos + 1 || dotPos == newUser.email.length() - 1) {
                        std::cout << "Invalid email format! Example: user@example.com\n";
                        continue;
                    }
                    // Check duplicate email
                    bool emailExists = false;
                    for (const auto& u : users) {
                        if (u.email == newUser.email) { emailExists = true; break; }
                    }
                    if (emailExists) {
                        std::cout << "Email already exists!\n";
                        continue;
                    }
                    break;
                }
                // Phone
                while (true) {
                    std::cout << "Phone: ";
                    std::getline(std::cin, newUser.phone);
                    bool valid = true;
                    std::string reason;
                    if (newUser.phone.length() < 9 || newUser.phone.length() > 11) {
                        valid = false;
                        reason = "Phone must be 9-11 digits.";
                    } else if (!isAllDigits(newUser.phone)) {
                        valid = false;
                        reason = "Phone must contain only digits.";
                    }
                    // Check duplicate phone
                    bool phoneExists = false;
                    for (const auto& u : users) {
                        if (u.phone == newUser.phone) { phoneExists = true; break; }
                    }
                    if (phoneExists) {
                        valid = false;
                        reason = "Phone number already exists.";
                    }
                    if (!valid) {
                        std::cout << "Invalid phone number! " << reason << "\n";
                        continue;
                    }
                    break;
                }
                // ID type
                while (true) {
                    std::cout << "ID type [1] CCCD   [2] Passport: ";
                    std::string idTypeChoice;
                    std::getline(std::cin, idTypeChoice);
                    if (idTypeChoice == "1") { newUser.idType = "CCCD"; break; }
                    if (idTypeChoice == "2") { newUser.idType = "Passport"; break; }
                    std::cout << "Invalid choice! Please enter 1 for CCCD or 2 for Passport.\n";
                }
                // ID number
                while (true) {
                    std::cout << "ID number: ";
                    std::getline(std::cin, newUser.idNumber);
                    newUser.idNumber = trim(newUser.idNumber);
                    bool valid = true;
                    std::string reason;
                    if (newUser.idNumber.empty()) {
                        valid = false;
                        reason = "ID number cannot be empty.";
                    } else if (!isAllDigits(newUser.idNumber)) {
                        valid = false;
                        reason = "ID number must contain only digits.";
                    } else if (newUser.idNumber.length() != 12) {
                        valid = false;
                        reason = "ID number must be exactly 12 digits.";
                    }
                    // Check duplicate ID number
                    bool idExists = false;
                    for (const auto& u : users) {
                        if (u.idNumber == newUser.idNumber) { idExists = true; break; }
                    }
                    if (idExists) {
                        valid = false;
                        reason = "ID number already exists.";
                    }
                    if (!valid) {
                        std::cout << "Invalid ID number! " << reason << "\n";
                        continue;
                    }
                    break;
                }
                // License number (optional)
                std::cout << "License number (optional): ";
                std::getline(std::cin, newUser.licenseNumber);
                // License expiry (optional, dd/mm/yyyy)
                while (true) {
                    std::cout << "License expiry (dd/mm/yyyy, optional): ";
                    std::string expiryStr;
                    std::getline(std::cin, expiryStr);
                    if (expiryStr.empty()) {
                        newUser.licenseExpiry = 0;
                        break;
                    }
                    if (expiryStr.length() == 10 && expiryStr[2] == '/' && expiryStr[5] == '/') {
                        std::string dayStr = expiryStr.substr(0,2);
                        std::string monthStr = expiryStr.substr(3,2);
                        std::string yearStr = expiryStr.substr(6,4);
                        if (isAllDigits(dayStr) && isAllDigits(monthStr) && isAllDigits(yearStr)) {
                            int day = safeStringToInt(dayStr);
                            int month = safeStringToInt(monthStr);
                            int year = safeStringToInt(yearStr);
                            if (year >= 1900 && year <= 2100 && month >= 1 && month <= 12) {
                                int maxDay = 31;
                                if (month == 2) {
                                    bool leap = (year%4==0 && (year%100!=0 || year%400==0));
                                    maxDay = leap ? 29 : 28;
                                } else if (month==4 || month==6 || month==9 || month==11) maxDay = 30;
                                if (day >= 1 && day <= maxDay) {
                                    std::time_t now = std::time(nullptr);
                                    std::tm nowTm = *std::localtime(&now);
                                    int nowY = nowTm.tm_year + 1900;
                                    int nowM = nowTm.tm_mon + 1;
                                    int nowD = nowTm.tm_mday;
                                    if (year < nowY || (year == nowY && month < nowM) || (year == nowY && month == nowM && day < nowD)) {
                                        std::cout << "Date cannot be in the past!\n";
                                        continue;
                                    }
                                    newUser.licenseExpiry = safeStol(yearStr + monthStr + dayStr, 0);
                                    break;
                                }
                            }
                        }
                    }
                    std::cout << "Invalid date format! Please enter as dd/mm/yyyy.\n";
                }
                newUser.role = UserRole::Member;
                // Make sure they deposit at least 20 CP to activate account
                int depositCP = 0;
                while (true) {
                    std::cout << "Initial deposit (>=20 CP, 1 CP = 1 USD): ";
                    std::string cpStr;
                    std::getline(std::cin, cpStr);
                    if (!isAllDigits(cpStr)) {
                        std::cout << "Invalid input! Please enter a number.\n";
                        continue;
                    }
                    depositCP = safeStringToInt(cpStr);
                    if (depositCP < 20) {
                        std::cout << "You must deposit at least 20 CP to activate your account!\n";
                        continue;
                    }
                    break;
                }
                newUser.creditPoints = depositCP;
                newUser.rating = 3;
                users.push_back(newUser);
                DataManager::saveUsers("users.csv", users);
                // Write to deposit history
                std::ofstream ofs("deposit_history.csv", std::ios::app);
                std::time_t now = std::time(nullptr);
                ofs << newUser.username << "," << depositCP << "," << now << "\n";
                ofs.close();
                std::cout << "Registration successful! You have " << depositCP << " CP. Your initial renter rating is 3. You can now log in as a member.\n";
                // Clear input buffer
                std::cout << "Press Enter to return to menu...";
                std::cin.clear();
                if (std::cin.rdbuf()->in_avail() > 0) std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            case 2: {
                std::vector<EBike> bikes = DataManager::loadMotorbikes("ebikes.csv");
                if (bikes.empty()) {
                    std::cout << "No EBikes available.\n";
                } else {
                    std::cout << "\n[You are viewing as a guest. Please register or login to see more details or rent an EBike.]\n";
                    std::cout << "==============================================\n";
                    std::cout << "| Brand      | Model         | CC   | City   |\n";
                    std::cout << "----------------------------------------------\n";
                    for (const auto& m : bikes) {
                        std::cout << "| " << std::setw(10) << std::left << m.brand
                                  << " | " << std::setw(13) << std::left << m.model
                                  << " | " << std::setw(5) << m.capacityCC
                                  << " | " << std::setw(6) << std::left << m.city << "|\n";
                    }
                    std::cout <<   "==============================================\n\n";
                }
                std::cout << "Press Enter to return to menu...";
                std::string dummy;
                std::getline(std::cin, dummy);
                std::cin.clear();
                if (std::cin.rdbuf()->in_avail() > 0) std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            case 3:
                std::cout << "Returning to main menu...\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 3);
}
