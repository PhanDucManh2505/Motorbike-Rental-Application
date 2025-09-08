// Standard library includes
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <limits>
#include <iomanip>

// Project includes
#include "member.h"
#include "models.h"
#include "data_manager.h"
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

bool isAllDigits(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isdigit(c); });
}

int safeStringToInt(const std::string& s) {
    int val = 0;
    for (char c : s) val = val * 10 + (c - '0');
    return val;
}

int safeStoi(const std::string& s, int defaultVal = 0) {
    if (s.empty() || !std::all_of(s.begin(), s.end(), ::isdigit)){
        return defaultVal;
    }
    int val = 0;
    for (char c : s) val = val * 10 + (c - '0');
    return val;
}

long safeStol(const std::string& s, long defaultVal = 0) {
    if (s.empty() || !std::all_of(s.begin(), s.end(), ::isdigit)) {
        return defaultVal;
    }
    try {
        return std::stol(s);
    } catch (...) {
        return defaultVal;
    }
}

bool isValidPhone(const std::string& s) {
    if (s.length() < 9 || s.length() > 11) return false;
    for (char c : s) if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    return true;
}
// Password strength checker, returns reasons if weak

std::vector<std::string> getPasswordWeaknesses(const std::string& pw) {
    std::vector<std::string> reasons;
    if (pw.length() < 8) reasons.push_back("Password must be at least 8 characters.");
    if (pw == "12345" || pw == "password" || pw == "12345678") reasons.push_back("Password is too common.");
    bool hasUpper = false, hasLower = false, hasDigit = false;
    std::string specialChars = "!@#$%^&*()-_=+[{]}\\|;:'\",<.>/?`~";
    bool hasSpecial = false;
    for (char c : pw) {
        if (std::isupper(c)) hasUpper = true;
        if (std::islower(c)) hasLower = true;
        if (std::isdigit(c)) hasDigit = true;
        if (specialChars.find(c) != std::string::npos) hasSpecial = true;
    }
    if (!hasUpper) reasons.push_back("Password must contain an uppercase letter.");
    if (!hasLower) reasons.push_back("Password must contain a lowercase letter.");
    if (!hasDigit) reasons.push_back("Password must contain a digit.");
    if (!hasSpecial) reasons.push_back("Password must contain a special character.");
    return reasons;
}
// Định nghĩa hàm đăng ký thành viên, tách từ main.cpp sang member.cpp
#include <iostream>
#include <vector>
#include <string>
#include "models.h"
#include "data_manager.h"
void registerMember() {
    std::vector<User> users = DataManager::loadUsers("users.csv");
    User u;
    std::cout << "\n--- Register as a new member ---\n";
    // Check for duplicate username
    while (true) {
        std::cout << "Username: ";
        std::getline(std::cin, u.username);
        u.username = trim(u.username);
        bool duplicate = false;
        for (const auto& user : users) {
            if (user.username == u.username) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) {
            std::cout << "Username already exists, please choose another!\n";
        } else {
            break;
        }
    }
    // Check password strength and duplication
    while (true) {
        std::cout << "Password: ";
        std::getline(std::cin, u.password);
        u.password = trim(u.password);
        std::vector<std::string> reasons = getPasswordWeaknesses(u.password);
        bool duplicatePw = false;
        for (const auto& user : users) {
            if (user.password == u.password) {
                duplicatePw = true;
                break;
            }
        }
        if (duplicatePw) reasons.push_back("This password is already used, please choose another!");
        if (reasons.empty()) break;
        std::cout << "Password is weak or duplicated!\n";
        for (const auto& r : reasons) std::cout << "- " << r << "\n";
        std::cout << "Please enter a stronger and unique password!\n";
    }
    std::cout << "Full name: ";
    std::getline(std::cin, u.fullName);
    u.fullName = trim(u.fullName);
    // Phone validation
    while (true) {
        std::cout << "Phone: ";
        std::getline(std::cin, u.phone);
        u.phone = trim(u.phone);
        if (isValidPhone(u.phone)) break;
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
        u.idNumber = trim(u.idNumber);
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
    u.licenseNumber = trim(u.licenseNumber);
    // License expiry validation (DD/MM/YYYY)
    while (true) {
        std::cout << "License expiry (DD/MM/YYYY): ";
        std::string expiryStr;
        std::getline(std::cin, expiryStr);
        expiryStr = trim(expiryStr);
        if (expiryStr.length() != 10 || expiryStr[2] != '/' || expiryStr[5] != '/') {
            std::cout << "Invalid date format! Must be DD/MM/YYYY.\n";
            continue;
        }
        std::string dayStr = expiryStr.substr(0,2);
        std::string monthStr = expiryStr.substr(3,2);
        std::string yearStr = expiryStr.substr(6,4);
        if (!isAllDigits(dayStr+monthStr+yearStr)) {
            std::cout << "Invalid date format! Day, month, year must be digits.\n";
            continue;
        }
        int day = safeStringToInt(dayStr);
        int month = safeStringToInt(monthStr);
        int year = safeStringToInt(yearStr);
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
        std::string ymd = yearStr + monthStr + dayStr;
        long inputDate = safeStol(ymd, 0);
        long today = 20250907;
        if (valid && inputDate < today) {
            valid = false;
            reason = "License expiry cannot be in the past.";
        }
        if (valid) {
            // Save as YYYYMMDD for internal processing
            u.licenseExpiry = inputDate;
            break;
        }
        std::cout << "Invalid license expiry! " << reason << "\n";
    }
    // Nhập số tiền đặt cọc
    int deposit = 0;
    while (true) {
        std::cout << "Enter deposit amount (USD, >=20): ";
        std::string depositStr;
        std::getline(std::cin, depositStr);
        bool valid = !depositStr.empty();
        for (char c : depositStr) {
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "Invalid input! Please enter a valid number.\n";
            continue;
        }
        deposit = 0;
        for (char c : depositStr) deposit = deposit * 10 + (c - '0');
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
#include "member.h"
#include "models.h"
#include "data_manager.h"
#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>

void showMemberMenu() {
    std::cout << std::endl;
    std::cout << "===============================================|" << std::endl;
    std::cout << "|                MEMBER MENU                   |" << std::endl;
    std::cout << "===============================================|" << std::endl;
    std::cout << "| 1. Register a motorbike for rent             |" << std::endl;
    std::cout << "| 2. Search and rent a motorbike               |" << std::endl;
    std::cout << "| 3. Manage profile                            |" << std::endl;
    std::cout << "| 4. View rental history & ratings             |" << std::endl;
    std::cout << "| 5. Unregister your motorbike                 |" << std::endl;
    std::cout << "| 6. Approve rental requests (for owners)      |" << std::endl;
    std::cout << "| 7. Cancel/Return rented motorbike            |" << std::endl;
    std::cout << "| 8. Logout                                    |" << std::endl;
    std::cout << "===============================================|" << std::endl;
    std::cout << "Please select an option: ";
}

void memberMenuLoop(const std::string& username) {
    int choice = 0;
    do {
        showMemberMenu();
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        if (!choiceStr.empty() && isAllDigits(choiceStr)) {
            choice = safeStringToInt(choiceStr);
        } else {
            choice = 0;
        }
    switch (choice) {
            case 1: {
                // Register a motorbike for rent
                std::vector<Motorbike> bikes = DataManager::loadMotorbikes("motorbikes.csv");
                // Check if user already registered a motorbike
                bool alreadyRegistered = false;
                for (const auto& b : bikes) {
                    if (b.ownerUsername == username) {
                        alreadyRegistered = true;
                        break;
                    }
                }
                if (alreadyRegistered) {
                    std::cout << "You have already registered a motorbike. Only one motorbike per member is allowed!\n";
                    std::cout << "Press Enter to continue...";
                    std::string dummy;
                    std::getline(std::cin, dummy);
                    break;
                }
                Motorbike m;
                std::cout << "\n--- Register a Motorbike for Rent ---\n";
                std::cout << "Brand: ";
                std::getline(std::cin, m.brand);
                std::cout << "Model: ";
                std::getline(std::cin, m.model);
                std::cout << "Color: ";
                std::getline(std::cin, m.color);
                // Capacity CC
                while (true) {
                    std::cout << "Capacity (CC): ";
                    std::string ccStr;
                    std::getline(std::cin, ccStr);
                    if (!ccStr.empty() && isAllDigits(ccStr)) {
                        m.capacityCC = safeStringToInt(ccStr);
                        break;
                    } else {
                        std::cout << "Invalid input! Please enter a number.\n";
                    }
                }
                // Year
                while (true) {
                    std::cout << "Year: ";
                    std::string yearStr;
                    std::getline(std::cin, yearStr);
                    if (!yearStr.empty() && isAllDigits(yearStr)) {
                        m.year = safeStringToInt(yearStr);
                        break;
                    } else {
                        std::cout << "Invalid input! Please enter a number.\n";
                    }
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
                    // Kiểm tra hợp lệ: không rỗng, độ dài 5-10, chỉ chữ/số, không trùng
                    bool valid = true;
                    std::string reason;
                    if (m.licensePlate.empty()) { valid = false; reason = "License plate cannot be empty."; }
                    else if (m.licensePlate.length() < 5 || m.licensePlate.length() > 10) { valid = false; reason = "License plate must be 5-10 characters."; }
                    else if (!std::all_of(m.licensePlate.begin(), m.licensePlate.end(), [](char c){ return std::isalnum(static_cast<unsigned char>(c)); })) { valid = false; reason = "License plate must contain only letters and digits."; }
                    else if (duplicate) { valid = false; reason = "License plate already exists."; }
                    if (!valid) {
                        std::cout << "Invalid license plate! " << reason << "\n";
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
                    if (!priceStr.empty() && isAllDigits(priceStr)) {
                        m.pricePerDayCP = safeStringToInt(priceStr);
                        if (m.pricePerDayCP > 0) break;
                    }
                    std::cout << "Invalid input! Please enter a positive number.\n";
                }
                // MinRenterRating
                while (true) {
                    std::cout << "Minimum renter rating (1-5): ";
                    std::string ratingStr;
                    std::getline(std::cin, ratingStr);
                    if (!ratingStr.empty() && isAllDigits(ratingStr)) {
                        m.minRenterRating = safeStringToInt(ratingStr);
                        if (m.minRenterRating >= 1 && m.minRenterRating <= 5) break;
                    }
                    std::cout << "Invalid input! Please enter a number from 1 to 5.\n";
                }
                // Gán ownerUsername
                m.ownerUsername = username;
                bikes.push_back(m);
                DataManager::saveMotorbikes("motorbikes.csv", bikes);
                std::cout << "Motorbike registered successfully!\n";
                break;
            }
            case 2: {
                // Search and rent a motorbike
                std::vector<User> users = DataManager::loadUsers("users.csv");
                User* currentUser = nullptr;
                for (auto& u : users) if (u.username == username) { currentUser = &u; break; }
                if (!currentUser) { std::cout << "User not found!\n"; break; }
                // Check if user is already renting a motorbike (at any time)
                if (!currentUser->rentingMotorbikeLicense.empty()) {
                    std::cout << "You are already renting a motorbike. You cannot rent another at the same time!\n";
                    break;
                }
                std::string dateStr, city;
                long searchDate = 0;
                // Chọn thành phố hoặc xem tất cả xe trước
                while (true) {
                    std::cout << "Select city: [1] HCMC   [2] Hanoi   [3] View all bikes. Please select (1/2/3): ";
                    std::string cityChoice;
                    std::getline(std::cin, cityChoice);
                    if (cityChoice == "1") { city = "HCMC"; break; }
                    if (cityChoice == "2") { city = "HANOI"; break; }
                    if (cityChoice == "3") { city = "ALL"; break; }
                    std::cout << "Invalid choice! Please enter 1, 2 hoặc 3.\n";
                }
                // Nếu chọn 1 hoặc 2 thì mới hỏi ngày thuê
                if (city == "HCMC" || city == "HANOI") {
                    while (true) {
                        std::cout << "Enter rental date (DD/MM/YYYY): ";
                        std::getline(std::cin, dateStr);
                        bool valid = false;
                        if (dateStr.length() == 10 && dateStr[2] == '/' && dateStr[5] == '/') {
                            std::string dayStr = dateStr.substr(0,2);
                            std::string monthStr = dateStr.substr(3,2);
                            std::string yearStr = dateStr.substr(6,4);
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
                                        std::string ymd = yearStr + monthStr + dayStr;
                                        searchDate = safeStol(ymd, 0);
                                        valid = true;
                                    }
                                }
                            }
                        }
                        if (valid) break;
                        std::cout << "Invalid date format! Please enter DD/MM/YYYY.\n";
                    }
                } else {
                    // Nếu chọn xem tất cả xe thì đặt searchDate = 0 (không lọc theo ngày)
                    searchDate = 0;
                }
                std::vector<Motorbike> bikes = DataManager::loadMotorbikes("motorbikes.csv");
                std::vector<RentalRequest> reqs = DataManager::loadRentalRequests("rental_requests.csv");
                // Lọc xe phù hợp
                std::vector<const Motorbike*> availableBikes;
                for (const auto& m : bikes) {
                    if (city != "ALL" && m.city != city) continue;
                    if (currentUser->rating < m.minRenterRating) continue;
                    if (currentUser->creditPoints < m.pricePerDayCP || currentUser->creditPoints - m.pricePerDayCP < 0) continue;
                    if (m.capacityCC > 50 && (currentUser->licenseNumber.empty() || currentUser->licenseExpiry < searchDate)) continue;
                    // Kiểm tra xe đã được đặt cho ngày này chưa
                    bool booked = false;
                    for (const auto& r : reqs) {
                        if (r.motorbikeLicensePlate == m.licensePlate && r.fromDate == searchDate && r.isAccepted) {
                            booked = true;
                            break;
                        }
                    }
                    if (booked) continue;
                    availableBikes.push_back(&m);
                }
                if (availableBikes.empty()) {
                    std::cout << "No available motorbikes match your criteria.\n";
                    break;
                }
                std::cout << "\n--- Available Motorbikes ---\n";
                std::cout << "+----+------------+---------------+----------+-----+------+---------------+-------+-----------+\n";
                std::cout << "| #  | Brand      | Model         | Color    | CC  | Year | Plate         | Price | MinRating |\n";
                std::cout << "+----+------------+---------------+----------+-----+------+---------------+-------+-----------+\n";
                int idx = 1;
                for (const auto* m : availableBikes) {
                    std::cout << "| " << std::setw(2) << std::right << idx << " "
                              << "| " << std::setw(10) << std::left << m->brand
                              << " | " << std::setw(13) << std::left << m->model
                              << "| " << std::setw(8) << std::left << m->color
                              << "| " << std::setw(3) << std::right << m->capacityCC << " "
                              << "| " << std::setw(4) << std::right << m->year << " "
                              << "| " << std::setw(13) << std::left << m->licensePlate
                              << "| " << std::setw(5) << std::right << m->pricePerDayCP << "  "
                              << "| " << std::setw(9) << std::right << m->minRenterRating << "  |\n";
                    // Hiển thị đánh giá trung bình và 1-2 bình luận gần nhất (nếu có)
                    if (!m->ratings.empty()) {
                        double avg = 0;
                        for (const auto& r : m->ratings) avg += r.score;
                        avg /= m->ratings.size();
                        std::cout << "    Average rating: " << std::fixed << std::setprecision(2) << avg << "/5\n";
                        int showCount = std::min(2, (int)m->ratings.size());
                        for (int i = (int)m->ratings.size() - showCount; i < (int)m->ratings.size(); ++i) {
                            std::cout << "    - [" << m->ratings[i].score << "/5] " << m->ratings[i].comment << " (by " << m->ratings[i].reviewerUsername << ")\n";
                        }
                    } else {
                        std::cout << "    No ratings yet.\n";
                    }
                    idx++;
                }
                std::cout << "+----+------------+---------------+----------+-----+------+---------------+-------+-----------+\n";
                int selIdx = -1;
                while (true) {
                    std::cout << "Enter the number of the motorbike to rent (0 to cancel): ";
                    std::string sel;
                    std::getline(std::cin, sel);
                    if (sel.empty() || sel == "0") {
                        std::cout << "Rental request cancelled.\n";
                        selIdx = -1;
                        break;
                    }
                    if (!isAllDigits(sel)) {
                        std::cout << "Invalid selection. Please enter a valid number.\n";
                        continue;
                    }
                    selIdx = safeStringToInt(sel);
                    if (selIdx < 1 || selIdx > (int)availableBikes.size()) {
                        std::cout << "Invalid selection. Please enter a number from 1 to " << availableBikes.size() << ".\n";
                        continue;
                    }
                    selIdx -= 1; // convert to 0-based index
                    break;
                }
                if (selIdx < 0 || selIdx >= (int)availableBikes.size()) {
                    // User cancelled or invalid, do not proceed
                    break;
                }
                const Motorbike* selected = availableBikes[selIdx];
                // Xác nhận gửi yêu cầu thuê
                std::cout << "You are eligible to rent this motorbike!\n";
                std::cout << "Do you want to send a rental request for this motorbike? (y/n): ";
                std::string confirm;
                std::getline(std::cin, confirm);
                if (confirm != "y" && confirm != "Y") {
                    std::cout << "Rental request cancelled.\n";
                    break;
                }
                // Gửi yêu cầu thuê: lưu vào file rental_requests.csv
                RentalRequest req = {};
                req.renterUsername = username;
                req.motorbikeLicensePlate = selected->licensePlate;
                req.fromDate = searchDate;
                req.toDate = searchDate; // chỉ 1 ngày
                req.isAccepted = false;
                reqs.push_back(req);
                DataManager::saveRentalRequests("rental_requests.csv", reqs);
                // Cập nhật trạng thái user đang thuê xe
                for (auto& u : users) {
                    if (u.username == username) {
                        u.rentingMotorbikeLicense = selected->licensePlate;
                        break;
                    }
                }
                DataManager::saveUsers("users.csv", users);
                std::cout << "Rental request sent successfully!\n";
                break;
            }
            case 3: {
                // Manage profile
                std::vector<User> users = DataManager::loadUsers("users.csv");
                User* currentUser = nullptr;
                for (auto& u : users) if (u.username == username) { currentUser = &u; break; }
                if (!currentUser) { std::cout << "User not found!\n"; break; }
                while (true) {
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
                    std::string opt; std::getline(std::cin, opt);
                    if (opt == "1") { 
                        std::cout << "\n--- Profile ---\n";
                        std::cout << "Username: " << currentUser->username << "\n";
                        std::cout << "Full name: " << currentUser->fullName << "\n";
                        std::cout << "Email: " << currentUser->email << "\n";
                        std::cout << "Phone: " << currentUser->phone << "\n";
                        std::cout << "ID type: " << currentUser->idType << ", ID number: " << currentUser->idNumber << "\n";
                        std::cout << "License: " << currentUser->licenseNumber << ", Expiry: " << currentUser->licenseExpiry << "\n";
                        std::cout << "Credit points: " << currentUser->creditPoints << "\n";
                        std::cout << "Rating: " << currentUser->rating << "\n";
                        std::cout << "\nPress Enter to continue...";
                        std::string dummy; std::getline(std::cin, dummy);
                    } else if (opt == "2") {
                        std::cout << "Update full name (current: " << currentUser->fullName << "): ";
                        std::string newName; std::getline(std::cin, newName);
                        if (!newName.empty()) currentUser->fullName = newName;
                        std::cout << "Update email (current: " << currentUser->email << "): ";
                        std::string newEmail; std::getline(std::cin, newEmail);
                        if (!newEmail.empty()) currentUser->email = newEmail;
                        std::cout << "Update phone (current: " << currentUser->phone << "): ";
                        std::string newPhone; std::getline(std::cin, newPhone);
                        if (!newPhone.empty()) currentUser->phone = newPhone;
                        DataManager::saveUsers("users.csv", users);
                        std::cout << "Profile updated!\n";
                    } else if (opt == "3") {
                        std::cout << "Enter current password: ";
                        std::string oldPw; std::getline(std::cin, oldPw);
                        if (oldPw != currentUser->password) { std::cout << "Wrong password!\n"; continue; }
                        std::cout << "Enter new password: ";
                        std::string newPw; std::getline(std::cin, newPw);
                        std::vector<std::string> reasons = getPasswordWeaknesses(newPw);
                        if (!reasons.empty()) {
                            std::cout << "Password is weak!\n";
                            for (const auto& r : reasons) std::cout << "- " << r << "\n";
                            continue;
                        }
                        currentUser->password = newPw;
                        DataManager::saveUsers("users.csv", users);
                        std::cout << "Password changed!\n";
                    } else if (opt == "4") {
                        std::cout << "Enter amount to deposit (USD): ";
                        std::string amtStr; std::getline(std::cin, amtStr);
                        bool valid = !amtStr.empty();
                        for (char c : amtStr) if (!isdigit(c)) valid = false;
                        if (!valid) { std::cout << "Invalid amount!\n"; continue; }
                        int amt = 0; for (char c : amtStr) amt = amt * 10 + (c - '0');
                        if (amt <= 0) { std::cout << "Amount must be positive!\n"; continue; }
                        std::cout << "Enter password to confirm: ";
                        std::string pw; std::getline(std::cin, pw);
                        if (pw != currentUser->password) { std::cout << "Wrong password!\n"; continue; }
                        currentUser->creditPoints += amt;
                        DepositTransaction tx{amt, std::time(nullptr)};
                        currentUser->depositHistory.push_back(tx);
                        DataManager::saveUsers("users.csv", users);
                        std::cout << "Deposited " << amt << " CP successfully!\n";
                    } else if (opt == "5") {
                        std::cout << "\n--- Deposit History ---\n";
                        if (currentUser->depositHistory.empty()) {
                            std::cout << "No deposit transactions found.\n";
                        } else {
                            for (const auto& tx : currentUser->depositHistory) {
                                char buf[32];
                                std::tm* tm_info = std::localtime(&tx.timestamp);
                                std::strftime(buf, 32, "%d/%m/%Y %H:%M", tm_info);
                                std::cout << buf << ": +" << tx.amount << " CP\n";
                            }
                        }
                        std::cout << "Press Enter to continue...";
                        std::string dummy; std::getline(std::cin, dummy);
                    } else if (opt == "6") {
                        break;
                    } else {
                        std::cout << "Invalid option!\n";
                    }
                }
                break;
            }
            case 4: {
                // View rental history & ratings
                std::vector<RentalRequest> reqs = DataManager::loadRentalRequests("rental_requests.csv");
                std::vector<Motorbike> bikes = DataManager::loadMotorbikes("motorbikes.csv");
                std::vector<User> users = DataManager::loadUsers("users.csv");
                std::cout << "\n--- Rental History ---\n";
                bool found = false;
                bool updated = false;
                for (auto& r : reqs) {
                    if (r.renterUsername == username) {
                        found = true;
                        // Find the motorbike
                        auto bikeIt = std::find_if(bikes.begin(), bikes.end(), [&](const Motorbike& m){ return m.licensePlate == r.motorbikeLicensePlate; });
                        // Find the owner
                        std::string ownerUsername = (bikeIt != bikes.end()) ? bikeIt->ownerUsername : "";
                        auto ownerIt = std::find_if(users.begin(), users.end(), [&](const User& u){ return u.username == ownerUsername; });
                        // Find current user
                        auto userIt = std::find_if(users.begin(), users.end(), [&](const User& u){ return u.username == username; });
                        std::cout << "-----------------------------\n";
                        std::cout << "Motorbike: " << r.motorbikeLicensePlate << (bikeIt != bikes.end() ? (" (" + bikeIt->brand + " " + bikeIt->model + ", " + bikeIt->color + ", " + std::to_string(bikeIt->capacityCC) + "cc, " + std::to_string(bikeIt->year) + ")") : "") << "\n";
                        std::cout << "Owner: " << ownerUsername << "\n";
                        std::cout << "Date: " << r.fromDate << (r.isAccepted ? " (Accepted)" : " (Pending)") << "\n";
                        // Show ratings for this rental
                        if (bikeIt != bikes.end()) {
                            bool hasRating = false;
                            for (const auto& rate : bikeIt->ratings) {
                                if (rate.reviewerUsername == username && rate.timestamp == r.toDate) {
                                    std::cout << "Your rating for this bike: [" << rate.score << "/5] " << rate.comment << "\n";
                                    hasRating = true;
                                }
                            }
                            if (!hasRating) std::cout << "You have not rated this bike yet.\n";
                        }
                        if (ownerIt != users.end()) {
                            bool hasOwnerRating = false;
                            for (const auto& rate : ownerIt->ratings) {
                                if (rate.reviewerUsername == username && rate.timestamp == r.toDate) {
                                    std::cout << "Your rating for this owner: [" << rate.score << "/5] " << rate.comment << "\n";
                                    hasOwnerRating = true;
                                }
                            }
                            if (!hasOwnerRating) std::cout << "You have not rated this owner yet.\n";
                        }
                        // Prompt for rating if not yet rated and rental is accepted
                        if (r.isAccepted) {
                            bool alreadyRated = false;
                            if (bikeIt != bikes.end()) {
                                for (const auto& rate : bikeIt->ratings) {
                                    if (rate.reviewerUsername == username && rate.timestamp == r.toDate) {
                                        alreadyRated = true;
                                        break;
                                    }
                                }
                            }
                            if (!alreadyRated && bikeIt != bikes.end() && userIt != users.end()) {
                                std::cout << "You have not rated this rental yet. Would you like to rate now? (y/n): ";
                                std::string ans; std::getline(std::cin, ans);
                                if (ans == "y" || ans == "Y") {
                                    // Rate the motorbike
                                    int score = 0;
                                    while (true) {
                                        std::cout << "Rate the motorbike (1-5): ";
                                        std::string s; std::getline(std::cin, s);
                                        score = safeStoi(s);
                                        if (score >= 1 && score <= 5) break;
                                        std::cout << "Invalid score!\n";
                                    }
                                    std::cout << "Comment for the motorbike: ";
                                    std::string comment; std::getline(std::cin, comment);
                                    Rating bikeRating{score, comment, username, r.toDate};
                                    bikeIt->ratings.push_back(bikeRating);
                                    // Rate the owner
                                    if (ownerIt != users.end()) {
                                        int ownerScore = 0;
                                        while (true) {
                                            std::cout << "Rate the owner (1-5): ";
                                            std::string s; std::getline(std::cin, s);
                                            if (!s.empty() && std::all_of(s.begin(), s.end(), ::isdigit)) {
                                                ownerScore = safeStoi(s);
                                                if (ownerScore >= 1 && ownerScore <= 5) break;
                                            }
                                            std::cout << "Invalid score!\n";
                                        }
                                        std::cout << "Comment for the owner: ";
                                        std::string ownerComment; std::getline(std::cin, ownerComment);
                                        Rating ownerRating{ownerScore, ownerComment, username, r.toDate};
                                        ownerIt->ratings.push_back(ownerRating);
                                        // Update owner's average rating
                                        int sum = 0;
                                        for (const auto& rate : ownerIt->ratings) sum += rate.score;
                                        ownerIt->rating = ownerIt->ratings.empty() ? 3 : (sum / (int)ownerIt->ratings.size());
                                    }
                                    updated = true;
                                    std::cout << "Thank you for rating!\n";
                                }
                            }
                        }
                    }
                }
                if (updated) {
                    DataManager::saveMotorbikes("motorbikes.csv", bikes);
                    DataManager::saveUsers("users.csv", users);
                }
                if (!found) std::cout << "No rental history found.\n";
                break;
            }
            case 5: {
                // Unregister your motorbike
                std::vector<Motorbike> bikes = DataManager::loadMotorbikes("motorbikes.csv");
                std::vector<RentalRequest> reqs = DataManager::loadRentalRequests("rental_requests.csv");
                // Find the user's motorbike
                auto it = std::find_if(bikes.begin(), bikes.end(), [&](const Motorbike& m){ return m.ownerUsername == username; });
                if (it == bikes.end()) {
                    std::cout << "You have not registered any motorbike.\n";
                    break;
                }
                // Check for accepted bookings
                bool hasAccepted = false;
                for (const auto& r : reqs) {
                    if (r.motorbikeLicensePlate == it->licensePlate && r.isAccepted) {
                        hasAccepted = true;
                        break;
                    }
                }
                if (hasAccepted) {
                    std::cout << "Cannot unregister: your motorbike has accepted bookings.\n";
                    break;
                }
                std::cout << "Are you sure you want to unregister your motorbike with license plate [" << it->licensePlate << "]? (y/n): ";
                std::string ans; std::getline(std::cin, ans);
                if (ans != "y" && ans != "Y") {
                    std::cout << "Unregister cancelled.\n";
                    break;
                }
                bikes.erase(it);
                DataManager::saveMotorbikes("motorbikes.csv", bikes);
                std::cout << "Your motorbike has been unregistered.\n";
                break;
            }
            case 6: {
                // Approve rental requests (for owners)
                std::vector<Motorbike> bikes = DataManager::loadMotorbikes("motorbikes.csv");
                std::vector<RentalRequest> reqs = DataManager::loadRentalRequests("rental_requests.csv");
                // Find the user's motorbike
                auto it = std::find_if(bikes.begin(), bikes.end(), [&](const Motorbike& m){ return m.ownerUsername == username; });
                if (it == bikes.end()) {
                    std::cout << "You have not registered any motorbike.\n";
                    break;
                }
                std::string plate = it->licensePlate;
                // List pending requests for this motorbike
                std::vector<size_t> pendingIdx;
                std::cout << "\n--- Pending Rental Requests for Your Motorbike ---\n";
                for (size_t i = 0; i < reqs.size(); ++i) {
                    if (reqs[i].motorbikeLicensePlate == plate && !reqs[i].isAccepted) {
                        pendingIdx.push_back(i);
                        std::cout << pendingIdx.size() << ". Renter: " << reqs[i].renterUsername << ", Date: " << reqs[i].fromDate << "\n";
                    }
                }
                if (pendingIdx.empty()) {
                    std::cout << "No pending requests.\n";
                    break;
                }
                int idx = -1;
                while (true) {
                    std::cout << "Enter the number of the request to approve (or 0 to cancel): ";
                    std::string sel; std::getline(std::cin, sel);
                    if (sel == "0" || sel.empty()) {
                        idx = -1;
                        break;
                    }
                    if (!isAllDigits(sel)) {
                        std::cout << "Invalid selection. Please enter a valid number.\n";
                        continue;
                    }
                    idx = safeStoi(sel, -1) - 1;
                    if (idx < 0 || idx >= (int)pendingIdx.size()) {
                        std::cout << "Invalid selection. Please enter a valid number.\n";
                        continue;
                    }
                    break;
                }
                if (idx < 0 || idx >= (int)pendingIdx.size()) break;
                size_t reqIdx = pendingIdx[idx];
                std::cout << "Approve (a) or reject (r) this request? (a/r): ";
                std::string act; std::getline(std::cin, act);
                if (act == "a" || act == "A") {
                    reqs[reqIdx].isAccepted = true;
                    std::cout << "Request approved.\n";
                } else if (act == "r" || act == "R") {
                    reqs.erase(reqs.begin() + reqIdx);
                    std::cout << "Request rejected and removed.\n";
                } else {
                    std::cout << "Invalid action.\n";
                }
                DataManager::saveRentalRequests("rental_requests.csv", reqs);
                break;
            }
            case 7: {
                // Cancel/Return rented motorbike
                std::vector<User> users = DataManager::loadUsers("users.csv");
                User* currentUser = nullptr;
                for (auto& u : users) if (u.username == username) { currentUser = &u; break; }
                if (!currentUser) { std::cout << "User not found!\n"; break; }
                if (currentUser->rentingMotorbikeLicense.empty()) {
                    std::cout << "You are not currently renting any motorbike.\n";
                    break;
                }
                std::cout << "Are you sure you want to cancel/return the motorbike with license plate [" << currentUser->rentingMotorbikeLicense << "]? (y/n): ";
                std::string ans; std::getline(std::cin, ans);
                if (ans != "y" && ans != "Y") {
                    std::cout << "Cancel/Return operation aborted.\n";
                    break;
                }
                // Xóa trạng thái đang thuê của user
                std::string plate = currentUser->rentingMotorbikeLicense;
                currentUser->rentingMotorbikeLicense = "";
                DataManager::saveUsers("users.csv", users);
                // Xóa các yêu cầu thuê chưa được duyệt (isAccepted == false) của user cho xe này
                std::vector<RentalRequest> reqs = DataManager::loadRentalRequests("rental_requests.csv");
                for (auto it = reqs.begin(); it != reqs.end(); ) {
                    if (it->renterUsername == username && it->motorbikeLicensePlate == plate && !it->isAccepted) {
                        it = reqs.erase(it);
                    } else {
                        ++it;
                    }
                }
                DataManager::saveRentalRequests("rental_requests.csv", reqs);
                std::cout << "You have successfully cancelled/returned the motorbike [" << plate << "].\n";
                break;
            }
            case 8: {
                // Logout
                std::cout << "Logging out...\n";
                break;
            }
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 8);
}
