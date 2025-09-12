#include <iostream>
#include "guest.h"
#include <regex>
#include <iostream>
#include <iomanip>
#include "admin.h"
#include "admin_login.h"
#include "models.h"
#include "data_manager.h"
#include <vector>
#include <limits>

void showMainMenu();
void memberMenuLoop(); // Prototype declaration for memberMenuLoop
int main() {
    int choice = 0;
    do {
        showMainMenu();
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
        switch (choice) {
            case 1:
                guestMenuLoop();
                break;
            case 2:
                memberMenuLoop();
                break;
            case 3: {
                AdminAccount admin;
                if (adminLogin(admin)) {
                    adminMenuLoop();
                }
                break;
            }
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
    std::cout << "                                    ,-~ |\n";
    std::cout << "       ________________          o==]___|\n";
    std::cout << "      |                |            \\ \\\n";
    std::cout << "      |________________|            /\\ \\\n";
    std::cout << " __  /  _,-----._      )           |  \\ \\\\\n";
    std::cout << "|_||/_-~         `.   /()          |  /|]_|_____\n";
    std::cout << "  |//              \\ |              \\/ /_-~     ~-_\n";
    std::cout << "  //________________||              / //___________\\\n";
    std::cout << " //__|______________| \\____________/ //___/-\\ \\~-_\n";
    std::cout << "((_________________/_-o___________/_//___/  /,\\\\  \\\n";
    std::cout << " |__/(  ((====)o===--~~                 (  ( (o/)  )\n";
    std::cout << "      \\  ``==' /                         \\  `--'  /\n";
    std::cout << "       `-.__,-'                            `-.__,-'\n";
    std::cout << "\n";
    
    std::cout << "==========================================\n";
    std::cout << "|      EBIKE RENTAL APPLICATION           |\n";
    std::cout << "==========================================\n";
    std::cout << "| 1. Guest                                |\n";
    std::cout << "| 2. Member                               |\n";
    std::cout << "| 3. Admin                                |\n";
    std::cout << "| 4. Exit                                 |\n";
    std::cout << "==========================================\n";
    std::cout << "Please select an option: ";
}










