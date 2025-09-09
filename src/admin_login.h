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

// Đọc danh sách admin từ file admin.csv
std::vector<AdminAccount> loadAdminAccounts(const std::string& filename);

// Đăng nhập admin, trả về true nếu thành công và trả về thông tin admin
bool adminLogin(AdminAccount& outAdmin);

void adminMenuLoop();
