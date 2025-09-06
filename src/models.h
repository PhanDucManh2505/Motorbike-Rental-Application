#ifndef MODELS_H
#define MODELS_H
#include <string>
#include <vector>
#include <ctime>

// Enum cho loại người dùng
enum class UserRole { Guest, Member, Admin };

// Đánh giá (rating)
struct Rating {
    int score; // 1-5
    std::string comment;
    std::string reviewerUsername;
    std::time_t timestamp;
};

// Thông tin xe máy điện
struct Motorbike {
    std::string brand;
    std::string model;
    std::string color;
    int capacityCC;
    int year;
    std::string licensePlate;
    std::string ownerUsername;
    std::string city; // HCMC hoặc Hanoi
    int pricePerDayCP;
    int minRenterRating;
    std::vector<Rating> ratings;
    bool isAvailable;
    std::vector<std::pair<std::time_t, std::time_t>> bookedPeriods;
};

// Yêu cầu thuê xe
struct RentalRequest {
    std::string renterUsername;
    std::string motorbikeLicensePlate;
    std::time_t fromDate;
    std::time_t toDate;
    bool isAccepted;
};

// Thông tin người dùng
struct User {
    std::string username;
    std::string password;
    std::string fullName;
    std::string email;
    std::string phone;
    std::string idType; // CCCD/Hộ chiếu
    std::string idNumber;
    std::string licenseNumber;
    std::time_t licenseExpiry; // 0 nếu không có
    UserRole role;
    int creditPoints;
    int rating; // trung bình
    std::vector<Rating> ratings;
    std::string ownedMotorbikeLicense; // nếu có
    std::string rentingMotorbikeLicense; // nếu đang thuê
};

#endif // MODELS_H
