// Chuyển time_t/timestamp sang chuỗi ngày giờ dd/mm/yyyy HH:MM
#include <string>
#include <ctime>
std::string formatTimestamp(time_t t);
#include <ctime>
#include <string>
#pragma once

// Chuyển "yyyy-mm-dd" thành time_t, trả về -1 nếu lỗi
time_t parseDate(const std::string& dateStr);
// Kiểm tra định dạng yyyy-mm-dd và ngày hợp lệ
bool isValidDate(const std::string& dateStr);
#pragma once
#include <string>
#include <ctime>
#include <vector>

std::string trim(const std::string& s);
bool isAllDigits(const std::string& s);
std::string toLower(const std::string& s);
int safeStringToInt(const std::string& s);
int safeStoi(const std::string& s, int defaultVal = 0);
long safeStol(const std::string& s, long defaultVal = 0);
bool isValidPhone(const std::string& s);
std::vector<std::string> getPasswordWeaknesses(const std::string& pw);
