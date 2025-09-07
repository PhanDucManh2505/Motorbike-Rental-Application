#include <iostream>
#include "guest.h"
#include "member.h"
#include <regex>
#include <iostream>
#include <iomanip>
#include "admin.h"
#include "models.h"
#include "data_manager.h"
#include <vector>
#include <limits>

void showMainMenu();
int main() {
    int choice = 0;
    do {
        showMainMenu();
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
                guestMenuLoop();
                break;
            case 2: {
                std::vector<User> users = DataManager::loadUsers("users.csv");
                std::string username, password;
                bool loginSuccess = false;
                std::cout << "\n--- Member Login ---\n";
                std::cout << "Username: ";
                std::getline(std::cin, username);
                std::cout << "Password: ";
                std::getline(std::cin, password);
                for (const auto& u : users) {
                    if (u.username == username && u.password == password && u.role == UserRole::Member) {
                        loginSuccess = true;
                        break;
                    }
                }
                if (loginSuccess) {
                    std::cout << "Login successful!\n";
                    memberMenuLoop(username);
                } else {
                    std::cout << "Login failed! Invalid username or password, or not a member.\n";
                }
                break;
            }
            case 3:
                adminMenuLoop();
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


// Function prototype
void registerMember();




// ...existing code...

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









