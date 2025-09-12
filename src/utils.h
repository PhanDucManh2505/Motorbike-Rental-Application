// Change time_t/timestamp to dd/mm/yyyy HH:MM
#include <string>
#include <ctime>
std::string formatTimestamp(time_t t);
#include <ctime>
#include <string>
#pragma once

// Change "yyyy-mm-dd" to time_t, return -1 if invalid
time_t parseDate(const std::string& dateStr);
// Check if yyyy-mm-dd format is valid
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
