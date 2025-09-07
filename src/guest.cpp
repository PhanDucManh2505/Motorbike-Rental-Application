#include "member.h"
#include "guest.h"
#include "models.h"
#include "data_manager.h"
#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>

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
    int choice = 0;
    do {
        showGuestMenu();
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
            case 1:
                registerMember();
                // Làm sạch buffer sau khi đăng ký xong
                std::cin.clear();
                if (std::cin.rdbuf()->in_avail() > 0) std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            case 2: {
                std::vector<Motorbike> bikes = DataManager::loadMotorbikes("motorbikes.csv");
                if (bikes.empty()) {
                    std::cout << "No motorbikes available.\n";
                } else {
                    std::cout << "\n[You are viewing as a guest. Please register or login to see more details or rent a motorbike.]\n";
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
