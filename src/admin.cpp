#include "utils.h"
#include "admin.h"
#include "admin_login.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "data_manager.h"
#include "models.h"

void adminMenuLoop() {
    int choice = 0;
    bool running = true;
    while (running) {
    std::cout << std::endl;
    std::cout << "+===============================================+" << std::endl;
    std::cout << "|                ADMIN MENU                     |" << std::endl;
    std::cout << "+===============================================+" << std::endl;
    std::cout << "| 1. View all users                             |" << std::endl;
    std::cout << "| 2. View all motorbikes                        |" << std::endl;
    std::cout << "| 3. Logout                                     |" << std::endl;
    std::cout << "+===============================================+" << std::endl;
    std::cout << "Please select an option: ";
        std::string input;
        std::getline(std::cin, input);
        // Xóa khoảng trắng đầu/cuối
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);
        // Kiểm tra input là số hợp lệ tuyệt đối
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
        // Đảm bảo input không rỗng và chỉ toàn số, mới chuyển đổi
        choice = 0;
        for (char c : input) {
            choice = choice * 10 + (c - '0');
        }
    switch (choice) {
            case 1: {
                // Hiển thị tất cả user
                auto users = DataManager::loadUsers("users.csv");
                std::cout << "\n========================================== ALL USERS ====================================================\n";
                std::cout << "| No | Username      | Full Name           | Email                | Phone        | Role   | CP   | Rating |\n";
                std::cout << "-----------------------------------------------------------------------------------------------------\n";
                int idx = 1;
                std::vector<User> deletableUsers;
                for (const auto& u : users) {
                    if (u.role != UserRole::Admin) deletableUsers.push_back(u);
                }
                for (const auto& u : deletableUsers) {
                    std::string roleStr = (u.role == UserRole::Admin ? "Admin" : (u.role == UserRole::Member ? "Member" : "Guest"));
                    std::cout << "| " << std::setw(2) << std::left << idx
                              << " | " << std::setw(12) << std::left << u.username
                              << "| " << std::setw(19) << std::left << u.fullName
                              << "| " << std::setw(21) << std::left << u.email
                              << "| " << std::setw(12) << std::left << u.phone
                              << "| " << std::setw(7) << std::left << roleStr
                              << "| " << std::setw(5) << std::left << u.creditPoints
                              << "| " << std::setw(7) << std::left << u.rating << "|" << std::endl;
                    ++idx;
                }
                std::cout << "-----------------------------------------------------------------------------------------------------\n";
                                if (deletableUsers.empty()) {
                                    std::cout << "No deletable users found.\n";
                                    std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy);
                                    break;
                                }
                                std::cout << "Enter the number of the account to delete (or 0 to cancel): ";
                                std::string delInput; std::getline(std::cin, delInput);
                                int delIdx = 0;
                                try { delIdx = std::stoi(delInput); } catch (...) { delIdx = 0; }
                                if (delIdx > 0 && delIdx <= (int)deletableUsers.size()) {
                                    std::string delUser = deletableUsers[delIdx-1].username;
                                    // Kiểm tra user có giao dịch đang diễn ra không
                                    auto reqs = DataManager::loadRentalRequests("rental_requests.csv");
                                    bool hasActive = false;
                                    for (const auto& r : reqs) {
                                        if ((r.renterUsername == delUser && r.isAccepted) || (r.motorbikeLicensePlate == delUser && r.isAccepted)) {
                                            hasActive = true; break;
                                        }
                                    }
                                    if (hasActive) {
                                        std::cout << "Cannot delete: user has active rental transactions!\n";
                                        std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy);
                                        break;
                                    }
                                    auto it = std::find_if(users.begin(), users.end(), [&](const User& u){ return u.username == delUser; });
                                    if (it != users.end()) users.erase(it);
                                    DataManager::saveUsers("users.csv", users);
                                    // Xóa motorbike thuộc user này
                                    auto bikes = DataManager::loadMotorbikes("motorbikes.csv");
                                    bikes.erase(std::remove_if(bikes.begin(), bikes.end(), [&](const Motorbike& m){ return m.ownerUsername == delUser; }), bikes.end());
                                    DataManager::saveMotorbikes("motorbikes.csv", bikes);
                                    // Xóa rental request liên quan
                                    reqs.erase(std::remove_if(reqs.begin(), reqs.end(), [&](const RentalRequest& r){ return r.renterUsername == delUser || r.motorbikeLicensePlate == delUser; }), reqs.end());
                                    DataManager::saveRentalRequests("rental_requests.csv", reqs);
                                    std::cout << "Account '" << delUser << "' and all related data deleted!\n";
                                } else if (delIdx != 0) {
                                    std::cout << "Invalid selection.\n";
                                }
                                std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy);
                break;
            }
            case 2: {
                // Hiển thị tất cả motorbike
                auto bikes = DataManager::loadMotorbikes("motorbikes.csv");
                std::cout << "\n======================================== ALL MOTORBIKES ================================================\n";
                std::cout << "| License Plate | Brand      | Model      | Color   | CC   | Year | Owner        | City   | Price      | MinRating |\n";
                std::cout << "------------------------------------------------------------------------------------------------------\n";
                for (const auto& m : bikes) {
                    std::cout << "| " << std::setw(13) << std::left << m.licensePlate
                              << "| " << std::setw(11) << std::left << m.brand
                              << "| " << std::setw(11) << std::left << m.model
                              << "| " << std::setw(8) << std::left << m.color
                              << "| " << std::setw(5) << std::left << (std::to_string(m.capacityCC) + "cc")
                              << "| " << std::setw(5) << std::left << m.year
                              << "| " << std::setw(13) << std::left << m.ownerUsername
                              << "| " << std::setw(7) << std::left << m.city
                              << "| " << std::setw(10) << std::left << (std::to_string(m.pricePerDayCP) + " CP/day")
                              << "| " << std::setw(9) << std::left << m.minRenterRating << "|" << std::endl;
                }
                std::cout << "------------------------------------------------------------------------------------------------------\n";
                std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy);
                break;
            }
            case 3:
                std::cout << "Logging out...\n";
                running = false;
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}
