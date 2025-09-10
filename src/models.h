#include <ctime>

#ifndef DEPOSIT_TRANSACTION_STRUCT
#define DEPOSIT_TRANSACTION_STRUCT
// Lịch sử nạp CP
struct DepositTransaction {
    int amount;
    std::time_t timestamp;
};
#endif
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

// Thông tin EBike
struct EBike {
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
// backward compatibility: some files still use Motorbike identifier
using Motorbike = EBike;

// Yêu cầu thuê EBike
struct RentalRequest {
    std::string renterUsername;
    std::string ebikeLicensePlate; // primary field
    std::string motorbikeLicensePlate; // backward compatibility
    std::time_t fromDate;
    std::time_t toDate;
    bool isAccepted;
    int rating = 0; // 1-5, 0 nếu chưa đánh giá
    std::string comment;
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
    std::string ownedEbikeLicense; // nếu có (was ownedMotorbikeLicense)
    std::string rentingEbikeLicense; // nếu đang thuê (was rentingMotorbikeLicense)
    std::string ownedMotorbikeLicense; // backward compatibility
    std::string rentingMotorbikeLicense; // backward compatibility
    std::vector<DepositTransaction> depositHistory;
};

#endif // MODELS_H
