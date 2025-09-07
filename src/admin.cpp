#include "admin.h"
#include <iostream>

void adminMenuLoop() {
    int choice = 0;
    do {
        std::cout << std::endl;
        std::cout << "===============================================|" << std::endl;
        std::cout << "|                ADMIN MENU                    |" << std::endl;
        std::cout << "===============================================|" << std::endl;
        std::cout << "| 1. View all users                            |" << std::endl;
        std::cout << "| 2. View all motorbikes                       |" << std::endl;
        std::cout << "| 3. Approve rental requests                   |" << std::endl;
        std::cout << "| 4. Logout                                    |" << std::endl;
        std::cout << "===============================================|" << std::endl;
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
            case 1:
                std::cout << "[Admin] View all users (placeholder)\n";
                break;
            case 2:
                std::cout << "[Admin] View all motorbikes (placeholder)\n";
                break;
            case 3:
                std::cout << "[Admin] Approve rental requests (placeholder)\n";
                break;
            case 4:
                std::cout << "Logging out...\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 4);
}
