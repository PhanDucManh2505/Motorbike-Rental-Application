#include "data_manager.h"
#include <fstream>
#include <sstream>

namespace DataManager {
    // Helper: split string by delimiter
    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            tokens.push_back(item);
        }
        return tokens;
    }

    std::vector<User> loadUsers(const std::string& filename) {
        std::vector<User> users;
        std::ifstream file(filename);
        std::string line;
        bool isFirstLine = true;
        while (std::getline(file, line)) {
            if (isFirstLine) { isFirstLine = false; continue; } // Skip header
            auto tokens = split(line, ',');
            if (tokens.size() < 12) continue;
            // Kiểm tra các trường số hợp lệ trước khi chuyển đổi
            bool valid = true;
            for (int idx : {8,9,10,11}) {
                if (tokens[idx].empty() || tokens[idx].find_first_not_of("0123456789") != std::string::npos) {
                    valid = false;
                    break;
                }
            }
            if (!valid) continue;
            User u;
            u.username = tokens[0];
            u.password = tokens[1];
            u.fullName = tokens[2];
            u.email = tokens[3];
            u.phone = tokens[4];
            u.idType = tokens[5];
            u.idNumber = tokens[6];
            u.licenseNumber = tokens[7];
            u.licenseExpiry = tokens[8].empty() || tokens[8].find_first_not_of("0123456789") != std::string::npos ? 0 : std::stol(tokens[8]);
            u.role = tokens[9].empty() || tokens[9].find_first_not_of("0123456789") != std::string::npos ? UserRole::Member : static_cast<UserRole>(std::stoi(tokens[9]));
            u.creditPoints = tokens[10].empty() || tokens[10].find_first_not_of("0123456789") != std::string::npos ? 0 : std::stoi(tokens[10]);
            u.rating = tokens[11].empty() || tokens[11].find_first_not_of("0123456789") != std::string::npos ? 0 : std::stoi(tokens[11]);
            if (tokens.size() > 12) u.ownedEbikeLicense = tokens[12];
            if (tokens.size() > 13) u.rentingEbikeLicense = tokens[13];
            users.push_back(u);
        }
        return users;
    }

    void saveUsers(const std::string& filename, const std::vector<User>& users) {
        std::ofstream file(filename);
        // Ghi dòng tiêu đề (header)
        file << "username,password,fullName,email,phone,idType,idNumber,licenseNumber,licenseExpiry,role,creditPoints,rating,ownedEbikeLicense,rentingEbikeLicense\n";
        for (const auto& u : users) {
            file << u.username << ',' << u.password << ',' << u.fullName << ',' << u.email << ','
                 << u.phone << ',' << u.idType << ',' << u.idNumber << ',' << u.licenseNumber << ','
                 << u.licenseExpiry << ',' << static_cast<int>(u.role) << ',' << u.creditPoints << ','
                 << u.rating << ',' << u.ownedEbikeLicense << ',' << u.rentingEbikeLicense << '\n';
        }
    }

    std::vector<EBike> loadMotorbikes(const std::string& filename) {
        std::vector<EBike> bikes;
        std::ifstream file(filename);
        std::string line;
        bool isFirstLine = true;
        while (std::getline(file, line)) {
            if (isFirstLine) { isFirstLine = false; continue; } // Skip header
            auto tokens = split(line, ',');
            if (tokens.size() < 10) continue;
            // Kiểm tra các trường số hợp lệ trước khi chuyển đổi
            bool valid = true;
            for (int idx : {3,4,8,9}) {
                if (tokens[idx].empty() || tokens[idx].find_first_not_of("0123456789") != std::string::npos) {
                    valid = false;
                    break;
                }
            }
            if (!valid) continue;
            EBike m;
            m.brand = tokens[0];
            m.model = tokens[1];
            m.color = tokens[2];
            m.capacityCC = tokens[3].empty() || tokens[3].find_first_not_of("0123456789") != std::string::npos ? 0 : std::stoi(tokens[3]);
            m.year = tokens[4].empty() || tokens[4].find_first_not_of("0123456789") != std::string::npos ? 0 : std::stoi(tokens[4]);
            m.licensePlate = tokens[5];
            m.ownerUsername = tokens[6];
            m.city = tokens[7];
            m.pricePerDayCP = tokens[8].empty() || tokens[8].find_first_not_of("0123456789") != std::string::npos ? 0 : std::stoi(tokens[8]);
            m.minRenterRating = tokens[9].empty() || tokens[9].find_first_not_of("0123456789") != std::string::npos ? 0 : std::stoi(tokens[9]);
            // ratings, isAvailable, bookedPeriods: not loaded for simplicity
            bikes.push_back(m);
        }
        return bikes;
    }

    void saveMotorbikes(const std::string& filename, const std::vector<EBike>& bikes) {
        std::ofstream file(filename);
        // Write header
        file << "Brand,Model,Color,CapacityCC,Year,LicensePlate,OwnerUsername,City,PricePerDayCP,MinRenterRating\n";
        for (const auto& m : bikes) {
            file << m.brand << ',' << m.model << ',' << m.color << ',' << m.capacityCC << ','
                 << m.year << ',' << m.licensePlate << ',' << m.ownerUsername << ',' << m.city << ','
                 << m.pricePerDayCP << ',' << m.minRenterRating << '\n';
        }
    }

    // Backwards-compatible wrappers named loadEbikes/saveEbikes
    std::vector<EBike> loadEbikes(const std::string& filename) {
        return loadMotorbikes(filename);
    }
    void saveEbikes(const std::string& filename, const std::vector<EBike>& bikes) {
        saveMotorbikes(filename, bikes);
    }

    std::vector<RentalRequest> loadRentalRequests(const std::string& filename) {
        std::vector<RentalRequest> reqs;
        std::ifstream file(filename);
        std::string line;
        bool isFirstLine = true;
        while (std::getline(file, line)) {
            if (isFirstLine) { isFirstLine = false; continue; }
            auto tokens = split(line, ',');
            if (tokens.size() < 5) continue;
            RentalRequest r;
            r.renterUsername = tokens[0];
            r.ebikeLicensePlate = tokens[1];
            r.motorbikeLicensePlate = r.ebikeLicensePlate; // compatibility
            r.fromDate = tokens[2].empty() || tokens[2].find_first_not_of("0123456789") != std::string::npos ? 0 : std::stol(tokens[2]);
            r.toDate = tokens[3].empty() || tokens[3].find_first_not_of("0123456789") != std::string::npos ? 0 : std::stol(tokens[3]);
            r.isAccepted = (tokens[4] == "1");
            r.rating = (tokens.size() > 5 && !tokens[5].empty() && tokens[5].find_first_not_of("0123456789") == std::string::npos) ? std::stoi(tokens[5]) : 0;
            r.comment = (tokens.size() > 6) ? tokens[6] : "";
            reqs.push_back(r);
        }
        return reqs;
    }

    void saveRentalRequests(const std::string& filename, const std::vector<RentalRequest>& reqs) {
        std::ofstream file(filename);
        // Ghi header mới
        file << "renterUsername,ebikeLicensePlate,fromDate,toDate,isAccepted,rating,comment\n";
        for (const auto& r : reqs) {
            file << r.renterUsername << ',' << r.ebikeLicensePlate << ',' << r.fromDate << ','
                 << r.toDate << ',' << (r.isAccepted ? "1" : "0") << ',' << r.rating << ',' << r.comment << '\n';
        }
    }
}
