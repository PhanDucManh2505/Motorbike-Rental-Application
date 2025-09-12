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

// Enum for user roles
enum class UserRole { Guest, Member, Admin };

// Rating
struct Rating {
    int score; // 1-5
    std::string comment;
    std::string reviewerUsername;
    std::time_t timestamp;
};

// Information about an EBike
struct EBike {
    std::string brand;
    std::string model;
    std::string color;
    int capacityCC;
    int year;
    std::string licensePlate;
    std::string ownerUsername;
    std::string city; // HCMC or HANOI
    int pricePerDayCP;
    int minRenterRating;
    std::vector<Rating> ratings;
    bool isAvailable;
    std::vector<std::pair<std::time_t, std::time_t>> bookedPeriods;
};
// backward compatibility: some files still use Motorbike identifier
using Motorbike = EBike;

// Rental request
struct RentalRequest {
    std::string renterUsername;
    std::string ebikeLicensePlate; // primary field
    std::string motorbikeLicensePlate; // backward compatibility
    std::time_t fromDate;
    std::time_t toDate;
    bool isAccepted;
    int rating = 0; // 1-5 (if rated) else 0 if not rated
    std::string comment;
};

//  User information
struct User {
    std::string username;
    std::string password;
    std::string fullName;
    std::string email;
    std::string phone;
    std::string idType; // e.g., CMND, CCCD, Passport
    std::string idNumber;
    std::string licenseNumber;
    std::time_t licenseExpiry; // timestamp, 0 if no license
    UserRole role;
    int creditPoints;
    int rating; // average rating from 1-5
    std::vector<Rating> ratings;
    std::string ownedEbikeLicense; // if have (was ownedMotorbikeLicense)
    std::string rentingEbikeLicense; // if in renting (was rentingMotorbikeLicense)
    std::string ownedMotorbikeLicense; // backward compatibility
    std::string rentingMotorbikeLicense; // backward compatibility
    std::vector<DepositTransaction> depositHistory;
};

#endif // MODELS_H
