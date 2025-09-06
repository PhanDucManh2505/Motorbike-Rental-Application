#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H
#include "models.h"
#include <vector>
#include <string>

namespace DataManager {
    // Load all users from file
    std::vector<User> loadUsers(const std::string& filename);
    // Save all users to file
    void saveUsers(const std::string& filename, const std::vector<User>& users);

    // Load all motorbikes from file
    std::vector<Motorbike> loadMotorbikes(const std::string& filename);
    // Save all motorbikes to file
    void saveMotorbikes(const std::string& filename, const std::vector<Motorbike>& motorbikes);

    // Load all rental requests from file
    std::vector<RentalRequest> loadRentalRequests(const std::string& filename);
    // Save all rental requests to file
    void saveRentalRequests(const std::string& filename, const std::vector<RentalRequest>& requests);
}

#endif // DATA_MANAGER_H
