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

    // Load all EBikes from file (function name kept as loadMotorbikes for compatibility)
    std::vector<EBike> loadMotorbikes(const std::string& filename);
    // Save all EBikes to file
    void saveMotorbikes(const std::string& filename, const std::vector<EBike>& motorbikes);

    // Load all rental requests from file
    std::vector<RentalRequest> loadRentalRequests(const std::string& filename);
    // Save all rental requests to file
    void saveRentalRequests(const std::string& filename, const std::vector<RentalRequest>& requests);
}

#endif // DATA_MANAGER_H
