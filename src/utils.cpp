// Chuyển time_t/timestamp sang chuỗi ngày giờ dd/mm/yyyy HH:MM
#include <string>
#include <ctime>
std::string formatTimestamp(time_t t) {
    char buf[20];
    tm* tm_info = localtime(&t);
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", tm_info);
    return std::string(buf);
}
#include <ctime>
#include <sstream>

bool isValidDate(const std::string& dateStr) {
    if (dateStr.size() != 10 || dateStr[4] != '-' || dateStr[7] != '-') return false;
    int y, m, d;
    if (sscanf(dateStr.c_str(), "%d-%d-%d", &y, &m, &d) != 3) return false;
    if (y < 1990 || y > 2100 || m < 1 || m > 12 || d < 1 || d > 31) return false;
    static const int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    int dim = daysInMonth[m-1];
    if (m == 2 && ((y%4==0 && y%100!=0) || (y%400==0))) dim = 29;
    if (d > dim) return false;
    return true;
}

time_t parseDate(const std::string& dateStr) {
    if (!isValidDate(dateStr)) return -1;
    std::tm tm = {};
    sscanf(dateStr.c_str(), "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_hour = 0; tm.tm_min = 0; tm.tm_sec = 0;
    return mktime(&tm);
}
// ...existing code...
#include <algorithm>
std::string toLower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}
#include "utils.h"
#include <algorithm>
#include <cctype>

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

int safeStoi(const std::string& s, int defaultVal) {
    if (s.empty() || !std::all_of(s.begin(), s.end(), ::isdigit)){
        return defaultVal;
    }
    int val = 0;
    for (char c : s) val = val * 10 + (c - '0');
    return val;
}

long safeStol(const std::string& s, long defaultVal) {
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
