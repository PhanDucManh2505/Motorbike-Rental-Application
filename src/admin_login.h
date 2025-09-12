#pragma once
#include <string>
#include <vector>
#include "models.h"

struct AdminAccount {
    std::string username;
    std::string password;
    std::string fullName;
    std::string email;
    std::string phone;
};

// Read admin accounts from admins.csv
std::vector<AdminAccount> loadAdminAccounts(const std::string& filename);

// login as admin and return the logged-in admin account
bool adminLogin(AdminAccount& outAdmin);

void adminMenuLoop();
