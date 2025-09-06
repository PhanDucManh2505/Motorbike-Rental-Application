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
        while (std::getline(file, line)) {
            auto tokens = split(line, ',');
            if (tokens.size() < 12) continue;
            User u;
            u.username = tokens[0];
            u.password = tokens[1];
            u.fullName = tokens[2];
            u.email = tokens[3];
            u.phone = tokens[4];
            u.idType = tokens[5];
            u.idNumber = tokens[6];
            u.licenseNumber = tokens[7];
            u.licenseExpiry = std::stol(tokens[8]);
            u.role = static_cast<UserRole>(std::stoi(tokens[9]));
            u.creditPoints = std::stoi(tokens[10]);
            u.rating = std::stoi(tokens[11]);
            if (tokens.size() > 12) u.ownedMotorbikeLicense = tokens[12];
            if (tokens.size() > 13) u.rentingMotorbikeLicense = tokens[13];
            users.push_back(u);
        }
        return users;
    }

    void saveUsers(const std::string& filename, const std::vector<User>& users) {
        std::ofstream file(filename);
        for (const auto& u : users) {
            file << u.username << ',' << u.password << ',' << u.fullName << ',' << u.email << ','
                 << u.phone << ',' << u.idType << ',' << u.idNumber << ',' << u.licenseNumber << ','
                 << u.licenseExpiry << ',' << static_cast<int>(u.role) << ',' << u.creditPoints << ','
                 << u.rating << ',' << u.ownedMotorbikeLicense << ',' << u.rentingMotorbikeLicense << '\n';
        }
    }

    std::vector<Motorbike> loadMotorbikes(const std::string& filename) {
        std::vector<Motorbike> bikes;
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            auto tokens = split(line, ',');
            if (tokens.size() < 10) continue;
            Motorbike m;
            m.brand = tokens[0];
            m.model = tokens[1];
            m.color = tokens[2];
            m.capacityCC = std::stoi(tokens[3]);
            m.year = std::stoi(tokens[4]);
            m.licensePlate = tokens[5];
            m.ownerUsername = tokens[6];
            m.city = tokens[7];
            m.pricePerDayCP = std::stoi(tokens[8]);
            m.minRenterRating = std::stoi(tokens[9]);
            // ratings, isAvailable, bookedPeriods: not loaded for simplicity
            bikes.push_back(m);
        }
        return bikes;
    }

    void saveMotorbikes(const std::string& filename, const std::vector<Motorbike>& bikes) {
        std::ofstream file(filename);
        for (const auto& m : bikes) {
            file << m.brand << ',' << m.model << ',' << m.color << ',' << m.capacityCC << ','
                 << m.year << ',' << m.licensePlate << ',' << m.ownerUsername << ',' << m.city << ','
                 << m.pricePerDayCP << ',' << m.minRenterRating << '\n';
        }
    }

    std::vector<RentalRequest> loadRentalRequests(const std::string& filename) {
        std::vector<RentalRequest> reqs;
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            auto tokens = split(line, ',');
            if (tokens.size() < 5) continue;
            RentalRequest r;
            r.renterUsername = tokens[0];
            r.motorbikeLicensePlate = tokens[1];
            r.fromDate = std::stol(tokens[2]);
            r.toDate = std::stol(tokens[3]);
            r.isAccepted = (tokens[4] == "1");
            reqs.push_back(r);
        }
        return reqs;
    }

    void saveRentalRequests(const std::string& filename, const std::vector<RentalRequest>& reqs) {
        std::ofstream file(filename);
        for (const auto& r : reqs) {
            file << r.renterUsername << ',' << r.motorbikeLicensePlate << ',' << r.fromDate << ','
                 << r.toDate << ',' << (r.isAccepted ? "1" : "0") << '\n';
        }
    }
}
