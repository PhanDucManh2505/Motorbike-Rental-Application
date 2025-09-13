#include <iostream>
#include <ctime>
#include <algorithm>
#include <ctime>
#include <string>
#include "utils.h"
#include "models.h"
#include "data_manager.h"
#include <cctype>
#include <iomanip>
#include <fstream>
#include <sstream>

// Prototype for member menu and related functions
void showMemberMenu(const std::string& username);
void registerEbikeForRent(const std::string& ownerUsername);
void searchAndRentEbike(const User& member);
void approveRentalRequests(const std::string& ownerUsername);
void cancelOrReturnRentedEbike(User& member);
// Helper functions for date validation and parsing
bool isValidDateDMY(const std::string& dateStr);
time_t parseDateDMY(const std::string& dateStr);
// Profile management functions
void manageProfile(User& member);
void viewProfile(const User& member);
void updateProfile(User& member);
void changePassword(User& member);
void depositCreditPoints(User& member);
void viewDepositHistory(const User& member);
// Prototype for viewing rental history and ratings
void viewRentalHistoryAndRatings(const User& member);
void unregisterEbike(const std::string& ownerUsername);

// Loop menu for member

void memberMenuLoop() {
    std::vector<User> users = DataManager::loadUsers("users.csv");
    std::string username, password;
    std::cout << "\n--- Member Login ---\n";
    std::cout << "Username: "; std::getline(std::cin, username);
    // Reject whitespace in username
    if (std::any_of(username.begin(), username.end(), [](unsigned char c){ return std::isspace(c); })) {
        std::cout << "Invalid username: username cannot contain spaces or whitespace characters.\n";
        return;
    }
    std::cout << "Password: "; std::getline(std::cin, password);
    User* member = nullptr;
    for (auto& u : users) {
        if (u.username == username && u.password == password) {
            member = &u;
            break;
        }
    }
    if (!member) {
        std::cout << "Login failed!\n";
        return;
    }
    int choice = 0;
    do {
        showMemberMenu(member->username);
        std::string input;
        std::getline(std::cin, input);
        input = trim(input);
        if (!isAllDigits(input)) { std::cout << "Invalid option!\n"; continue; }
        choice = safeStringToInt(input);
        switch (choice) {
            case 1:
                registerEbikeForRent(member->username);
                break;
            case 2:
                searchAndRentEbike(*member);
                break;
            case 3:
                manageProfile(*member);
                break;
            case 4:
                viewRentalHistoryAndRatings(*member);
                break;
            case 5:
                unregisterEbike(member->username);
                break;
            case 6:
                approveRentalRequests(member->username);
                break;
            case 7:
                cancelOrReturnRentedEbike(*member);
                break;
            case 8:
                std::cout << "Logging out...\n";
                break;
            default:
                std::cout << "Invalid option!\n";
        }
    } while (choice != 8);
}

// See rental history and ratings
void viewRentalHistoryAndRatings(const User& member) {
    std::cout << "\n--- Rental History & Ratings ---\n";
    std::vector<RentalRequest> requests = DataManager::loadRentalRequests("rental_requests.csv");
    bool found = false;
    for (const auto& req : requests) {
        if (req.renterUsername == member.username) {
            std::cout << "- EBike: " << req.ebikeLicensePlate
                      << ", From: " << formatTimestamp(req.fromDate)
                      << ", To: " << formatTimestamp(req.toDate);
            if (!req.comment.empty() || req.rating > 0) {
                std::cout << ", Rating: " << req.rating;
                if (!req.comment.empty()) std::cout << ", Comment: " << req.comment;
            }
            std::cout << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No rental history found.\n";
    std::cout << "Current rating: " << member.rating << "\n";
    std::cout << "Press Enter to continue...";
    std::string dummy; std::getline(std::cin, dummy);
}

// Unregister EBike
void unregisterEbike(const std::string& ownerUsername) {
    std::vector<EBike> bikes = DataManager::loadMotorbikes("ebikes.csv");
    auto it = std::find_if(bikes.begin(), bikes.end(), [&](const EBike& b) {
        return toLower(trim(b.ownerUsername)) == toLower(trim(ownerUsername));
    });
    if (it == bikes.end()) {
        std::cout << "\n[!] You have not registered any EBike.\n";
        std::cout << "Press Enter to continue...";
        std::string dummy; std::getline(std::cin, dummy);
        return;
    }
    std::vector<RentalRequest> requests = DataManager::loadRentalRequests("rental_requests.csv");
    std::time_t now = std::time(nullptr);
    for (const auto& req : requests) {
        if (req.ebikeLicensePlate == it->licensePlate && req.isAccepted && req.toDate >= now) {
            std::cout << "\n[!] Cannot unregister: Your EBike is currently rented or has a confirmed rental request!\n";
            std::cout << "Press Enter to continue...";
            std::string dummy; std::getline(std::cin, dummy);
            return;
        }
    }
    std::cout << "\nAre you sure you want to unregister your EBike (" << it->licensePlate << ")? (y/n): ";
    std::string confirm; std::getline(std::cin, confirm);
    if (confirm != "y" && confirm != "Y") {
        std::cout << "Unregister cancelled.\n";
        std::cout << "Press Enter to continue...";
        std::string dummy; std::getline(std::cin, dummy);
        return;
    }
    bikes.erase(it);
    DataManager::saveMotorbikes("ebikes.csv", bikes);
    std::cout << "Your EBike has been unregistered successfully!\n";
    std::cout << "Press Enter to continue...";
    std::string dummy; std::getline(std::cin, dummy);
}
// Profile management menu
void manageProfile(User& member) {
    int opt = 0;
    do {
        std::cout << "\n========================================\n";
        std::cout << "|         MANAGE PROFILE MENU             |\n";
        std::cout << "========================================\n";
        std::cout << "| 1. View profile                         |\n";
        std::cout << "| 2. Update profile                       |\n";
        std::cout << "| 3. Change password                      |\n";
        std::cout << "| 4. Deposit credit points                |\n";
        std::cout << "| 5. View deposit history                 |\n";
        std::cout << "| 6. Back                                 |\n";
        std::cout << "========================================\n";
        std::cout << "Select option: ";
        std::string input; std::getline(std::cin, input);
        if (!isAllDigits(input)) { std::cout << "Invalid option!\n"; continue; }
        opt = safeStringToInt(input);
        switch (opt) {
            case 1:
                viewProfile(member);
                break;
            case 2:
                updateProfile(member);
                break;
            case 3:
                changePassword(member);
                break;
            case 4:
                depositCreditPoints(member);
                break;
            case 5:
                viewDepositHistory(member);
                break;
            case 6:
                std::cout << "Returning to member menu...\n";
                break;
            default:
                std::cout << "Invalid option!\n";
        }
    } while (opt != 6);
}
// View profile
void viewProfile(const User& member) {
    std::cout << "\n--- Profile Information ---\n";
    std::cout << "Username: " << member.username << "\n";
    std::cout << "Full name: " << member.fullName << "\n";
    std::cout << "Email: " << member.email << "\n";
    std::cout << "Phone: " << member.phone << "\n";
    std::cout << "ID type: " << member.idType << "\n";
    std::cout << "ID number: " << member.idNumber << "\n";
    std::cout << "License number: " << member.licenseNumber << "\n";
    std::cout << "License expiry: " << member.licenseExpiry << "\n";
    std::cout << "Credit points: " << member.creditPoints << "\n";
    std::cout << "Rating: " << member.rating << "\n";
    std::cout << "Press Enter to continue...";
    std::string dummy; std::getline(std::cin, dummy);
}
// Update profile
void updateProfile(User& member) {
    std::cout << "\n--- Update Profile ---\n";
    std::vector<User> users = DataManager::loadUsers("users.csv");
    // Update every field except username and role
    std::cout << "Current full name: " << member.fullName << "\nNew full name (leave blank to keep): ";
    std::string input;
    std::getline(std::cin, input);
    if (!input.empty()) member.fullName = input;

    std::cout << "Current email: " << member.email << "\nNew email (leave blank to keep): ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        // Check if email is valid
        bool exists = false;
        for (const auto& u : users) {
            if (u.username != member.username && u.email == input) { exists = true; break; }
        }
        if (exists) std::cout << "Email already exists!\n";
        else member.email = input;
    }

    std::cout << "Current phone: " << member.phone << "\nNew phone (leave blank to keep): ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        bool valid = true;
        std::string reason;
        if (input.length() < 9 || input.length() > 11) {
            valid = false;
            reason = "Phone must be 9-11 digits.";
        } else {
            for (char c : input) {
                if (!isdigit(c)) { valid = false; reason = "Phone must contain only digits."; break; }
            }
        }
        // Check duplicate phone
        bool exists = false;
        for (const auto& u : users) {
            if (u.username != member.username && u.phone == input) { exists = true; break; }
        }
        if (exists) {
            valid = false;
            reason = "Phone number already exists.";
        }
        if (!valid) {
            std::cout << "Invalid phone number! " << reason << "\n";
        } else {
            member.phone = input;
        }
    }

    std::cout << "Current ID type: " << member.idType << "\nNew ID type (CCCD/Passport, leave blank to keep): ";
    std::getline(std::cin, input);
    if (!input.empty() && (input == "CCCD" || input == "Passport")) member.idType = input;

    std::cout << "Current ID number: " << member.idNumber << "\nNew ID number (leave blank to keep): ";
    std::getline(std::cin, input);
    if (!input.empty()) member.idNumber = input;

    std::cout << "Current license number: " << member.licenseNumber << "\nNew license number (leave blank to keep): ";
    std::getline(std::cin, input);
    if (!input.empty()) member.licenseNumber = input;

    std::cout << "Current license expiry: " << member.licenseExpiry << "\nNew license expiry (dd/mm/yyyy, leave blank to keep): ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        if (isValidDateDMY(input)) {
            member.licenseExpiry = parseDateDMY(input);
        } else {
            std::cout << "Invalid date format!\n";
        }
    }

    // Confirm before saving
    std::cout << "\nAre you sure you want to save changes? (y/n): ";
    std::string confirm; std::getline(std::cin, confirm);
    if (confirm == "y" || confirm == "Y") {
        for (auto& u : users) {
            if (u.username == member.username) {
                u = member;
                break;
            }
        }
        DataManager::saveUsers("users.csv", users);
        std::cout << "Profile updated!\n";
        std::cout << "Press Enter to continue...";
        std::string dummy; std::getline(std::cin, dummy);
    } else {
        std::cout << "Update cancelled. No changes saved.\n";
    }
}
// Change password
void changePassword(User& member) {
    std::cout << "\n--- Change Password ---\n";
    std::vector<User> users = DataManager::loadUsers("users.csv");
    std::string oldPw, newPw, confirmPw;
    std::cout << "Enter current password: ";
    std::getline(std::cin, oldPw);
    if (oldPw != member.password) {
        std::cout << "[!] Incorrect current password. Password not changed.\n";
        return;
    }
    // Enter new password
    while (true) {
        std::cout << "Enter new password: ";
        std::getline(std::cin, newPw);
        bool pwExists = false;
        for (const auto& u : users) {
            if (u.password == newPw) { pwExists = true; break; }
        }
        if (pwExists) {
            std::cout << "[!] Password already exists! Please choose another.\n";
            continue;
        }
        std::vector<std::string> reasons = getPasswordWeaknesses(newPw);
        if (!reasons.empty()) {
            std::cout << "[!] Password is weak!\n";
            for (const auto& r : reasons) std::cout << "- " << r << "\n";
            continue;
        }
        std::cout << "Confirm new password: ";
        std::getline(std::cin, confirmPw);
        if (newPw != confirmPw) {
            std::cout << "[!] Passwords do not match!\n";
            continue;
        }
        break;
    }
    // Confirm before changing
    std::cout << "\nYou are about to change your password.\n";
    std::cout << "Are you sure you want to change your password? (y/n): ";
    std::string confirm; std::getline(std::cin, confirm);
    if (confirm == "y" || confirm == "Y") {
        for (auto& u : users) {
            if (u.username == member.username) {
                u.password = newPw;
                member.password = newPw;
                break;
            }
        }
        DataManager::saveUsers("users.csv", users);
        std::cout << "[✓] Password changed successfully!\n";
        std::cout << "Press Enter to continue...";
        std::string dummy; std::getline(std::cin, dummy);
    } else {
        std::cout << "[!] Password change cancelled. No changes made.\n";
    }
}
// Deposit credit points
void depositCreditPoints(User& member) {
    std::cout << "\n--- Deposit Credit Points ---\n";
    std::vector<User> users = DataManager::loadUsers("users.csv");
    std::string pw, amountStr;
    std::cout << "Enter your password to confirm: ";
    std::getline(std::cin, pw);
    if (pw != member.password) {
        std::cout << "[!] Incorrect password. Deposit cancelled.\n";
        return;
    }
    int amount = 0;
    while (true) {
        std::cout << "Enter amount to deposit (USD, 1 USD = 1 CP): ";
        std::getline(std::cin, amountStr);
        if (!isAllDigits(amountStr)) {
            std::cout << "[!] Invalid input! Please enter a number.\n";
            continue;
        }
        amount = safeStringToInt(amountStr);
        if (amount <= 0) {
            std::cout << "[!] Amount must be positive!\n";
            continue;
        }
        break;
    }
    // Confirm deposit
    std::cout << "\nYou are about to deposit " << amount << " CP.\n";
    std::cout << "Are you sure you want to deposit " << amount << " CP? (y/n): ";
    std::string confirm; std::getline(std::cin, confirm);
    if (confirm != "y" && confirm != "Y") {
        std::cout << "Deposit cancelled.\n";
        return;
    }
    // Perform deposit
    member.creditPoints += amount;
    for (auto& u : users) {
        if (u.username == member.username) {
            u.creditPoints = member.creditPoints;
            break;
        }
    }
    DataManager::saveUsers("users.csv", users);
    // Write to deposit history
    std::ofstream ofs("deposit_history.csv", std::ios::app);
    std::time_t now = std::time(nullptr);
    ofs << member.username << "," << amount << "," << now << "\n";
    ofs.close();
    std::cout << "[✓] Deposit successful! Your new balance: " << member.creditPoints << " CP\n";
    std::cout << "Press Enter to continue...";
    std::string dummy; std::getline(std::cin, dummy);
}
// View deposit history
void viewDepositHistory(const User& member) {
    std::cout << "\n--- Deposit History ---\n";
    std::ifstream ifs("deposit_history.csv");
    if (!ifs) {
        std::cout << "No deposit history found.\n";
        return;
    }
    std::string line;
    bool found = false;
    std::cout << std::left << std::setw(20) << "Date/Time" << std::setw(10) << "Amount" << "\n";
    std::cout << "----------------------------------------\n";
    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        std::string uname, amountStr, timeStr;
        if (!std::getline(iss, uname, ',')) continue;
        if (!std::getline(iss, amountStr, ',')) continue;
        if (!std::getline(iss, timeStr, ',')) continue;
        if (uname == member.username) {
            std::time_t t = std::stol(timeStr);
            std::cout << std::left << std::setw(20) << formatTimestamp(t) << std::setw(10) << amountStr << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No deposit history for this account.\n";
    std::cout << "Press Enter to continue...";
    std::string dummy; std::getline(std::cin, dummy);
}

// Show member menu
void showMemberMenu(const std::string& username) {
    std::cout << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "|                MEMBER MENU                   |" << std::endl;
    std::cout << "===============================================|" << std::endl;
    std::cout << "| 1. Register an EBike for rent                |" << std::endl;
    std::cout << "| 2. Search and rent an EBike                  |" << std::endl;
    std::cout << "| 3. Manage profile                            |" << std::endl;
    std::cout << "| 4. View rental history & ratings             |" << std::endl;
    std::cout << "| 5. Unregister your EBike                     |" << std::endl;
    std::cout << "| 6. Approve rental requests (for owners)      |" << std::endl;
    std::cout << "| 7. Cancel/Return rented EBike                |" << std::endl;
    std::cout << "| 8. Logout                                    |" << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "Welcome, " << username << "!" << std::endl;
    std::cout << "Please select an option: ";
}
// Register an EBike for rent
void registerEbikeForRent(const std::string& ownerUsername) {
    std::vector<EBike> bikes = DataManager::loadMotorbikes("ebikes.csv");
    // Check if member already registered a bike
    for (const auto& b : bikes) {
        if (toLower(trim(b.ownerUsername)) == toLower(trim(ownerUsername))) {
            std::cout << "\n[!] You have already registered an EBike. Cannot register more!\n";
            std::cout << "Press Enter to continue...";
            std::string dummy; std::getline(std::cin, dummy);
            return;
        }
    }
    EBike m;
    std::cout << "\n--- Register an EBike for Rent ---\n";
    // Brand
    std::cout << "Brand: ";
    std::getline(std::cin, m.brand);
    // Model
    std::cout << "Model: ";
    std::getline(std::cin, m.model);
    // Color
    std::cout << "Color: ";
    std::getline(std::cin, m.color);
    // CapacityCC
    while (true) {
        std::cout << "Capacity (CC): ";
        std::string capStr;
        std::getline(std::cin, capStr);
        if (!isAllDigits(capStr)) {
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        m.capacityCC = safeStringToInt(capStr);
        if (m.capacityCC <= 0) {
            std::cout << "Capacity must be positive!\n";
            continue;
        }
        break;
    }
    // Year
    while (true) {
        std::cout << "Year: ";
        std::string yearStr;
        std::getline(std::cin, yearStr);
        if (!isAllDigits(yearStr)) {
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        m.year = safeStringToInt(yearStr);
        if (m.year < 1990 || m.year > 2100) {
            std::cout << "Year must be between 1990 and 2100!\n";
            continue;
        }
        break;
    }
    // License plate
    while (true) {
        std::cout << "License plate: ";
        std::getline(std::cin, m.licensePlate);
        m.licensePlate = trim(m.licensePlate);
        bool duplicate = false;
        for (const auto& b : bikes) {
            if (b.licensePlate == m.licensePlate) {
                duplicate = true;
                break;
            }
        }
        if (m.licensePlate.empty()) {
            std::cout << "License plate cannot be empty.\n";
            continue;
        }
        if (duplicate) {
            std::cout << "License plate already exists!\n";
            continue;
        }
        break;
    }
    // City
    while (true) {
        std::cout << "City [1] HCMC   [2] Hanoi: ";
        std::string cityChoice;
        std::getline(std::cin, cityChoice);
        if (cityChoice == "1") { m.city = "HCMC"; break; }
        if (cityChoice == "2") { m.city = "HANOI"; break; }
        std::cout << "Invalid choice! Please enter 1 or 2.\n";
    }
    // PricePerDayCP
    while (true) {
        std::cout << "Price per day (CP): ";
        std::string priceStr;
        std::getline(std::cin, priceStr);
        if (!isAllDigits(priceStr)) {
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        m.pricePerDayCP = safeStringToInt(priceStr);
        if (m.pricePerDayCP <= 0) {
            std::cout << "Price must be positive!\n";
            continue;
        }
        break;
    }
    // MinRenterRating
    while (true) {
        std::cout << "Minimum renter rating (1-5): ";
        std::string ratingStr;
        std::getline(std::cin, ratingStr);
        if (!isAllDigits(ratingStr)) {
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        m.minRenterRating = safeStringToInt(ratingStr);
        if (m.minRenterRating < 1 || m.minRenterRating > 5) {
            std::cout << "Rating must be from 1 to 5!\n";
            continue;
        }
        break;
    }
    // Booked periods
    std::string fromDateStr, toDateStr;
    time_t fromDate, toDate;
    // ...existing code...
    while (true) {
        std::cout << "Rental start date (dd/mm/yyyy): ";
        std::getline(std::cin, fromDateStr);
        if (!isValidDateDMY(fromDateStr)) {
            std::cout << "Invalid date format!\n";
            continue;
        }
        fromDate = parseDateDMY(fromDateStr);
        time_t now = std::time(nullptr);
        // Chỉ lấy phần ngày, bỏ giờ phút giây
        std::tm* nowTm = std::localtime(&now);
        nowTm->tm_hour = 0; nowTm->tm_min = 0; nowTm->tm_sec = 0;
        time_t today = mktime(nowTm);
        if (fromDate < today) {
            std::cout << "Start date must be today or in the future!\n";
            continue;
        }
        break;
    }
    while (true) {
        std::cout << "Rental end date (dd/mm/yyyy): ";
        std::getline(std::cin, toDateStr);
        if (!isValidDateDMY(toDateStr)) {
            std::cout << "Invalid date format!\n";
            continue;
        }
        toDate = parseDateDMY(toDateStr);
        if (toDate < fromDate) {
            std::cout << "End date must be after start date!\n";
            continue;
        }
        break;
    }
    m.bookedPeriods.clear();
    m.bookedPeriods.push_back({fromDate, toDate});
    m.ownerUsername = ownerUsername;
    m.isAvailable = true;
    bikes.push_back(m);
    DataManager::saveMotorbikes("ebikes.csv", bikes);
    std::cout << "EBike registered successfully!\n";
}
// Search and rent an EBike
void searchAndRentEbike(const User& member) {
    // Load all rental requests and bikes
    std::vector<RentalRequest> allRequests = DataManager::loadRentalRequests("rental_requests.csv");
    std::vector<EBike> bikes = DataManager::loadMotorbikes("ebikes.csv");
    // Check if member has an active rental already (accepted request with toDate >= today)
    time_t now = std::time(nullptr);
    for (const auto& req : allRequests) {
        if (req.renterUsername == member.username && req.isAccepted && req.toDate >= now) {
            std::cout << "You already have an active rental. Please return/cancel it before renting another EBike.\n";
            return;
        }
    }

    std::cout << "\n--- Search and Rent an EBike ---\n";
    std::cout << "1. View all EBikes\n2. Search by city\nChoose option: ";
    std::string opt; std::getline(std::cin, opt);

    std::vector<EBike> filtered;
    if (opt == "1") {
        filtered = bikes;
    } else {
        std::string city;
        while (true) {
            std::cout << "City [1] HCMC   [2] Hanoi: ";
            std::string cityChoice; std::getline(std::cin, cityChoice);
            if (cityChoice == "1") { city = "HCMC"; break; }
            if (cityChoice == "2") { city = "HANOI"; break; }
            std::cout << "Invalid choice!\n";
        }
        for (const auto& m : bikes) {
            if (toLower(m.city) == toLower(city)) filtered.push_back(m);
        }
    }
    if (filtered.empty()) {
        std::cout << "No EBikes found matching your criteria.\n";
        return;
    }

    // Enter rental period
    std::string fromDateStr, toDateStr;
    time_t fromDate, toDate;
    while (true) {
        std::cout << "Enter rental start date (dd/mm/yyyy): ";
        std::getline(std::cin, fromDateStr);
        fromDate = parseDateDMY(trim(fromDateStr));
        if (fromDate == -1) { std::cout << "Invalid date format!\n"; continue; }
        time_t now = std::time(nullptr);
        std::tm* nowTm = std::localtime(&now);
        nowTm->tm_hour = 0; nowTm->tm_min = 0; nowTm->tm_sec = 0;
        time_t today = mktime(nowTm);
        if (fromDate < today) { std::cout << "Start date must be today or later!\n"; continue; }
        break;
    }
    while (true) {
        std::cout << "Enter rental end date (dd/mm/yyyy): ";
        std::getline(std::cin, toDateStr);
        toDate = parseDateDMY(trim(toDateStr));
        if (toDate == -1) { std::cout << "Invalid date format!\n"; continue; }
        if (toDate < fromDate) { std::cout << "End date must be after start date!\n"; continue; }
        break;
    }

    // Check availability for the period
    std::vector<EBike> availableBikes;
    for (const auto& m : filtered) {
        bool isRented = false;
        for (const auto& req : allRequests) {
            if (req.ebikeLicensePlate == m.licensePlate && req.isAccepted) {
                if (!(toDate < req.fromDate || fromDate > req.toDate)) {
                    isRented = true; break;
                }
            }
        }
        if (!isRented) availableBikes.push_back(m);
    }
    if (availableBikes.empty()) {
        std::cout << "No available EBikes for the selected period.\n";
        return;
    }

    // Show available bikes
    std::cout << "\n============================ AVAILABLE EBIKES ==============================\n";
    std::cout << "| No | Brand/Model      | Color   | CC   | Year | Plate     | City   | Price      |\n";
    std::cout << "-------------------------------------------------------------------------------\n";
    for (size_t i = 0; i < availableBikes.size(); ++i) {
        const auto& m = availableBikes[i];
        std::cout << "| " << std::setw(2) << std::left << i+1 << " "
                  << "| " << std::setw(14) << std::left << (m.brand + " " + m.model)
                  << "| " << std::setw(8) << std::left << m.color
                  << "| " << std::setw(5) << std::left << (std::to_string(m.capacityCC) + "cc")
                  << "| " << std::setw(5) << std::left << m.year
                  << "| " << std::setw(10) << std::left << m.licensePlate
                  << "| " << std::setw(7) << std::left << m.city
                  << "| " << std::setw(10) << std::left << (std::to_string(m.pricePerDayCP) + " CP/day")
                  << "|\n";
    }
    std::cout << "-------------------------------------------------------------------------------\n";

    // User selects a bike
    std::cout << "\nEnter the number of the EBike to rent (0 to cancel): ";
    std::string sel; std::getline(std::cin, sel);
    int idx = safeStringToInt(sel) - 1;
    if (idx < 0 || idx >= (int)availableBikes.size()) {
        std::cout << "Cancelled.\n"; return;
    }
    const auto& chosen = availableBikes[idx];

    // Check member eligibility
    int days = (int)((toDate - fromDate) / (60*60*24)) + 1;
    int totalPrice = chosen.pricePerDayCP * days;
    if (member.creditPoints < totalPrice) {
        std::cout << "Not enough credit points!\n"; return;
    }
    if (member.rating < chosen.minRenterRating) {
        std::cout << "Your rating is too low!\n"; return;
    }
    if (chosen.capacityCC > 50 && (member.licenseNumber.empty() || member.licenseExpiry < std::time(nullptr))) {
        std::cout << "You need a valid license to rent this EBike!\n"; return;
    }

    // Make rental request
    RentalRequest req;
    req.renterUsername = member.username;
    req.ebikeLicensePlate = chosen.licensePlate;
    req.motorbikeLicensePlate = req.ebikeLicensePlate; // compatibility
    req.fromDate = fromDate;
    req.toDate = toDate;
    req.isAccepted = false;

    allRequests.push_back(req);
    DataManager::saveRentalRequests("rental_requests.csv", allRequests);
    std::cout << "Rental request sent!\n";
}

// Check if date string is valid dd/mm/yyyy
bool isValidDateDMY(const std::string& dateStr) {
    if (dateStr.size() != 10 || dateStr[2] != '/' || dateStr[5] != '/') return false;
    int d, m, y;
    if (sscanf(dateStr.c_str(), "%d/%d/%d", &d, &m, &y) != 3) return false;
    if (y < 1990 || y > 2100 || m < 1 || m > 12 || d < 1 || d > 31) return false;
    static const int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    int dim = daysInMonth[m-1];
    if (m == 2 && ((y%4==0 && y%100!=0) || (y%400==0))) dim = 29;
    if (d > dim) return false;
    return true;
}
// Parse date string dd/mm/yyyy to time_t
time_t parseDateDMY(const std::string& dateStr) {
    if (!isValidDateDMY(dateStr)) return -1;
    int d, m, y;
    sscanf(dateStr.c_str(), "%d/%d/%d", &d, &m, &y);
    std::tm tm = {};
    tm.tm_mday = d;
    tm.tm_mon = m-1;
    tm.tm_year = y-1900;
    tm.tm_hour = 0; tm.tm_min = 0; tm.tm_sec = 0;
    return mktime(&tm);
}
// Format timestamp to readable string
void approveRentalRequests(const std::string& ownerUsername) {
    std::vector<EBike> bikes = DataManager::loadMotorbikes("ebikes.csv");
    auto it = std::find_if(bikes.begin(), bikes.end(), [&](const EBike& b) {
        return toLower(trim(b.ownerUsername)) == toLower(trim(ownerUsername));
    });
    if (it == bikes.end()) {
        std::cout << "\n[!] You have not registered any EBike.\n";
        std::cout << "Press Enter to continue...";
        std::string dummy; std::getline(std::cin, dummy);
        return;
    }
    std::vector<RentalRequest> requests = DataManager::loadRentalRequests("rental_requests.csv");
    bool hasRequest = false;
    std::vector<User> users = DataManager::loadUsers("users.csv");
    for (size_t i = 0; i < requests.size(); ++i) {
        if (requests[i].ebikeLicensePlate == it->licensePlate && !requests[i].isAccepted) {
            hasRequest = true;
            std::cout << "\nRental request from: " << requests[i].renterUsername << ", Sent at: "
                      << formatTimestamp(requests[i].fromDate) << "\n";
            std::cout << "Approve this request? (y/n): ";
            std::string confirm; std::getline(std::cin, confirm);
            if (confirm == "y" || confirm == "Y") {
                // Calculate rental price
                int days = 1;
                if (requests[i].toDate > requests[i].fromDate) {
                    days = (int)((requests[i].toDate - requests[i].fromDate) / (60*60*24)) + 1;
                }
                int price = it->pricePerDayCP * days;
                // Deduct credit points from renter
                for (auto& u : users) {
                    if (u.username == requests[i].renterUsername) {
                        if (u.creditPoints < price) {
                            std::cout << "[!] Renter does not have enough credit points.\n";
                            break;
                        }
                        u.creditPoints -= price;
                        break;
                    }
                }
                DataManager::saveUsers("users.csv", users);
                // Mark request as accepted
                it->bookedPeriods.push_back({requests[i].fromDate, requests[i].toDate});
                DataManager::saveMotorbikes("ebikes.csv", bikes);
                requests[i].isAccepted = true;

                // Archive the EBike record into Rental.csv to mark it as rented
                {
                    std::ofstream rofs("Rental.csv", std::ios::app);
                    if (rofs) {
                        // Ensure header exists is handled elsewhere; append row
                        rofs << it->brand << ',' << it->model << ',' << it->color << ',' << it->capacityCC << ','
                             << it->year << ',' << it->licensePlate << ',' << it->ownerUsername << ',' << it->city << ','
                             << it->pricePerDayCP << ',' << it->minRenterRating << '\n';
                        rofs.close();
                    }
                }
                // Remove overlapping pending requests for the same EBike
                for (size_t j = 0; j < requests.size(); ++j) {
                    if (j == i) continue;
                    if (requests[j].ebikeLicensePlate == it->licensePlate && !requests[j].isAccepted) {
                        // Check if periods overlap
                        bool overlap = !(requests[j].toDate < requests[i].fromDate || requests[j].fromDate > requests[i].toDate);
                        if (overlap) {
                            // Remove this request
                            requests.erase(requests.begin() + j);
                            if (j < i) --i; // adjust i since we removed an earlier element
                            --j;
                        }
                    }
                }
                std::cout << "Rental request approved!\n";
            } else {
                std::cout << "Request skipped.\n";
            }
        }
    }
    if (!hasRequest) {
        std::cout << "\nNo rental requests to approve.\n";
    } else {
        DataManager::saveRentalRequests("rental_requests.csv", requests);
    }
    std::cout << "Press Enter to continue...";
    std::string dummy; std::getline(std::cin, dummy);
}

// Cancel or return rented EBike (or cancel pending request)
void cancelOrReturnRentedEbike(User& member){
    std::vector<RentalRequest> requests = DataManager::loadRentalRequests("rental_requests.csv");
    std::vector<EBike> bikes = DataManager::loadMotorbikes("ebikes.csv");
    bool found = false;

    // If request is accepted (isAccepted == true)
    for (size_t i = 0; i < requests.size(); ++i) {
        if (requests[i].renterUsername == member.username && requests[i].isAccepted) {
            found = true;
            std::cout << "\nYou are currently renting: " << requests[i].ebikeLicensePlate 
                      << ". Return this EBike? (y/n): ";
            std::string confirm; std::getline(std::cin, confirm);
            if (confirm == "y" || confirm == "Y") {
                std::cout << "EBike returned successfully!\n";

                // rating EBikes
                std::cout << "Would you like to rate this EBike? (y/n): ";
                std::string rateOpt; std::getline(std::cin, rateOpt);
                if (rateOpt == "y" || rateOpt == "Y") {
                    int score = 0;
                    while (true) {
                        std::cout << "Enter rating (1-5): ";
                        std::string scoreStr; std::getline(std::cin, scoreStr);
                        if (!isAllDigits(scoreStr)) { std::cout << "Invalid!\n"; continue; }
                        score = safeStringToInt(scoreStr);
                        if (score < 1 || score > 5) { std::cout << "Rating must be 1-5!\n"; continue; }
                        break;
                    }
                    std::cout << "Enter your comment (optional): ";
                    std::string comment; std::getline(std::cin, comment);
                    requests[i].rating = score;
                    requests[i].comment = comment;

                    for (auto& b : bikes) {
                        if (b.licensePlate == requests[i].ebikeLicensePlate) {
                            Rating r;
                            r.score = score;
                            r.comment = comment;
                            r.reviewerUsername = member.username;
                            r.timestamp = std::time(nullptr);
                            b.ratings.push_back(r);
                        }
                    }

                    // Update member's average rating
                    int sum = 0, count = 0;
                    for (const auto& req : requests) {
                        if (req.renterUsername == member.username && req.rating > 0) {
                            sum += req.rating;
                            count++;
                        }
                    }
                    if (count > 0) {
                        int avg = (sum + count/2) / count;
                        std::vector<User> users = DataManager::loadUsers("users.csv");
                        for (auto& u : users) {
                            if (u.username == member.username) {
                                u.rating = avg;
                                member.rating = avg;
                                break;
                            }
                        }
                        DataManager::saveUsers("users.csv", users);
                    }
                    DataManager::saveMotorbikes("ebikes.csv", bikes);
                    std::cout << "Thank you for your rating!\n";
                }

                // ❌ Remove the rental request
                // Also remove the EBike record from Rental.csv to mark it as available again
                requests.erase(requests.begin() + i);
                DataManager::saveRentalRequests("rental_requests.csv", requests);
            } else {
                std::cout << "Skipped.\n";
            }
            std::cout << "Press Enter to continue...";
            std::string dummy; std::getline(std::cin, dummy);
            return;
        }
    }

    // if request is pending (isAccepted == false)
    for (size_t i = 0; i < requests.size(); ++i) {
        if (requests[i].renterUsername == member.username && !requests[i].isAccepted) {
            found = true;
            std::cout << "\nYou have a pending rental request for " << requests[i].ebikeLicensePlate 
                      << ". Cancel this request? (y/n): ";
            std::string confirm; std::getline(std::cin, confirm);
            if (confirm == "y" || confirm == "Y") {
                requests.erase(requests.begin() + i);
                std::cout << "Rental request cancelled!\n";
                DataManager::saveRentalRequests("rental_requests.csv", requests);
            } else {
                std::cout << "Skipped.\n";
            }
            std::cout << "Press Enter to continue...";
            std::string dummy; std::getline(std::cin, dummy);
            return;
        }
    }

    if (!found) {
        std::cout << "\nYou have no active rentals or pending requests.\n";
        std::cout << "Press Enter to continue...";
        std::string dummy; std::getline(std::cin, dummy);
    }
}




