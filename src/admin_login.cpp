#include "admin_login.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<AdminAccount> loadAdminAccounts(const std::string& filename) {
    std::vector<AdminAccount> admins;
    std::ifstream ifs(filename);
    std::string line;
    getline(ifs, line); // skip header
    while (getline(ifs, line)) {
        std::stringstream ss(line);
        std::string token;
        AdminAccount acc;
        getline(ss, acc.username, ',');
        getline(ss, acc.password, ',');
        getline(ss, acc.fullName, ',');
        getline(ss, acc.email, ',');
        getline(ss, acc.phone, ',');
        if (!acc.username.empty()) admins.push_back(acc);
    }
    return admins;
}

bool adminLogin(AdminAccount& outAdmin) {
    std::string username, password;
    std::cout << "\n--- Admin Login ---\n";
    std::cout << "Username: "; std::getline(std::cin, username);
    std::cout << "Password: "; std::getline(std::cin, password);
    auto admins = loadAdminAccounts("admin.csv");
    for (const auto& acc : admins) {
        if (acc.username == username && acc.password == password) {
            outAdmin = acc;
            std::cout << "Login successful! Welcome, " << acc.fullName << "\n";
            return true;
        }
    }
    std::cout << "Login failed!\n";
    return false;
}
