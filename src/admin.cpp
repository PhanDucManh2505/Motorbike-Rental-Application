#include "utils.h"
#include "admin.h"
#include "admin_login.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include "data_manager.h"
#include "models.h"

// Additions
#include <limits>
using namespace std;

// Gets a valid integer choice from the user within the specified range
int getChoice(int min, int max){
    int n;
    while(true){
        cin >> n;
        if(cin.fail() || n < min || n > max){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return n;
        }
    }
}

void adminMenuLoop() {
    int choice = 0;
    bool running = true;
    while (running) {
    std::cout << std::endl;
    std::cout << "+===============================================+" << std::endl;
    std::cout << "|                ADMIN MENU                     |" << std::endl;
    std::cout << "+===============================================+" << std::endl;
    std::cout << "| 1. View all users                             |" << std::endl;
    std::cout << "| 2. View all EBikes                            |" << std::endl;
    std::cout << "| 3. Delete EBike                               |" << std::endl;
    std::cout << "| 4. Logout                                     |" << std::endl;
    std::cout << "+===============================================+" << std::endl;
    std::cout << "Please select an option: ";
        
        /*std::string input;
        std::getline(std::cin, input);
        // Delet leading/trailing whitespace
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);
        // Check if input is all digits
        bool valid = !input.empty();
        for (char c : input) {
            if (!isdigit(c)) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "Invalid option. Please enter a number.\n";
            continue;
        }
        // Make sure to convert string to int safely
        choice = 0;
        for (char c : input) {
            choice = choice * 10 + (c - '0');
        }
            */
        choice = getChoice(1, 4);
    switch (choice) {
            case 1: {
                // Display all users from users.csv
                auto users = DataManager::loadUsers("users.csv");
                std::cout << "\n============================================ ALL USERS ================================================\n";
                std::cout << "| No | Username    | Full Name          | Email                | Phone       | Role   | CP   | Rating |\n";
                std::cout << "-------------------------------------------------------------------------------------------------------\n";
                int idx = 1;
                std::vector<User> deletableUsers;
                for (const auto& u : users) {
                    if (u.role != UserRole::Admin) deletableUsers.push_back(u);
                }
                for (const auto& u : deletableUsers) {
                    std::string roleStr = (u.role == UserRole::Admin ? "Admin" : (u.role == UserRole::Member ? "Member" : "Guest"));
                    std::cout << "| " << std::setw(2) << std::left << idx
                              << " | " << std::setw(12) << std::left << u.username
                              << "| " << std::setw(19) << std::left << u.fullName
                              << "| " << std::setw(21) << std::left << u.email
                              << "| " << std::setw(12) << std::left << u.phone
                              << "| " << std::setw(7) << std::left << roleStr
                              << "| " << std::setw(5) << std::left << u.creditPoints
                              << "| " << std::setw(7) << std::left << u.rating << "|" << std::endl;
                    ++idx;
                }
                std::cout << "-------------------------------------------------------------------------------------------------------\n";
                                if (deletableUsers.empty()) {
                                    std::cout << "No deletable users found.\n";
                                    std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy);
                                    break;
                                }
                                std::cout << "Enter the number of the account to delete (or 0 to cancel): ";
                                std::string delInput; std::getline(std::cin, delInput);
                                int delIdx = 0;
                                try { delIdx = std::stoi(delInput); } catch (...) { delIdx = 0; }
                                if (delIdx > 0 && delIdx <= (int)deletableUsers.size()) {
                                    std::string delUser = deletableUsers[delIdx-1].username;
                                    // Check if user has active rental requests
                                    auto reqs = DataManager::loadRentalRequests("rental_requests.csv");
                                    bool hasActive = false;
                                    for (const auto& r : reqs) {
                                        if ((r.renterUsername == delUser && r.isAccepted) || (r.ebikeLicensePlate == delUser && r.isAccepted) || (r.motorbikeLicensePlate == delUser && r.isAccepted)) {
                                            hasActive = true; break;
                                        }
                                    }
                                    if (hasActive) {
                                        std::cout << "Cannot delete: user has active rental transactions!\n";
                                        std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy);
                                        break;
                                    }
                                    auto it = std::find_if(users.begin(), users.end(), [&](const User& u){ return u.username == delUser; });
                                    if (it != users.end()) users.erase(it);
                                    DataManager::saveUsers("users.csv", users);
                                    // Delete user's EBikes
                                    auto bikes = DataManager::loadMotorbikes("ebikes.csv");
                                    bikes.erase(std::remove_if(bikes.begin(), bikes.end(), [&](const EBike& m){ return m.ownerUsername == delUser; }), bikes.end());
                                    DataManager::saveMotorbikes("ebikes.csv", bikes);
                                    // Delete related rental requests
                                    reqs.erase(std::remove_if(reqs.begin(), reqs.end(), [&](const RentalRequest& r){ return r.renterUsername == delUser || r.ebikeLicensePlate == delUser || r.motorbikeLicensePlate == delUser; }), reqs.end());
                                    DataManager::saveRentalRequests("rental_requests.csv", reqs);
                                    std::cout << "Account '" << delUser << "' and all related data deleted!\n";
                                } else if (delIdx != 0) {
                                    std::cout << "Invalid selection.\n";
                                }
                                std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy);
                break;
            }
            case 2: {
                // Display all EBikes from ebikes.csv
                auto bikes = DataManager::loadMotorbikes("ebikes.csv");
                std::cout << "\n======================================== ALL EBIKES =============================================================\n";
                std::cout << "| License Plate| Brand      | Model      | Color   | CC   | Year | Owner        | City   | Price     | MinRating|\n";
                std::cout << "-----------------------------------------------------------------------------------------------------------------\n";
                for (const auto& m : bikes) {
                    std::cout << "| " << std::setw(13) << std::left << m.licensePlate
                              << "| " << std::setw(11) << std::left << m.brand
                              << "| " << std::setw(11) << std::left << m.model
                              << "| " << std::setw(8) << std::left << m.color
                              << "| " << std::setw(5) << std::left << (std::to_string(m.capacityCC) + "cc")
                              << "| " << std::setw(5) << std::left << m.year
                              << "| " << std::setw(13) << std::left << m.ownerUsername
                              << "| " << std::setw(7) << std::left << m.city
                              << "| " << std::setw(10) << std::left << (std::to_string(m.pricePerDayCP) + " CP/day")
                              << "| " << std::setw(9) << std::left << m.minRenterRating << "|" << std::endl;
                }
                std::cout << "-----------------------------------------------------------------------------------------------------------------\n";
                std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy);
                break;
            }
            case 3: {
                // Delete EBike
                auto bikes = DataManager::loadMotorbikes("ebikes.csv");
                if (bikes.empty()) { std::cout << "No EBikes to delete.\n"; std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy); break; }
                std::cout << "\nListing EBikes:\n";
                int i = 1;
                for (const auto& m : bikes) {
                    std::cout << i << ". " << m.licensePlate << " - " << m.brand << " " << m.model << " (Owner: " << m.ownerUsername << ")\n";
                    ++i;
                }
                std::cout << "Enter the number of the EBike to delete (or 0 to cancel): "; std::string sel; std::getline(std::cin, sel);
                int s = 0; try { s = std::stoi(sel); } catch (...) { s = 0; }
                if (s <= 0 || s > (int)bikes.size()) { std::cout << "Cancelled or invalid selection.\n"; std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy); break; }
                std::string lp = bikes[s-1].licensePlate;
                // check active rental requests
                auto reqs = DataManager::loadRentalRequests("rental_requests.csv");
                bool hasActive = false;
                for (const auto& r : reqs) {
                    if ((r.ebikeLicensePlate == lp || r.motorbikeLicensePlate == lp) && r.isAccepted) { hasActive = true; break; }
                }
                if (hasActive) {
                    // Archive the EBike into Rental.csv instead of deleting outright
                    bool needHeader = false;
                    std::ifstream ifs("Rental.csv");
                    if (!ifs.good()) needHeader = true;
                    ifs.close();
                    std::ofstream ofs("Rental.csv", std::ios::app);
                    if (needHeader) ofs << "Brand,Model,Color,CapacityCC,Year,LicensePlate,OwnerUsername,City,PricePerDayCP,MinRenterRating\n";
                    const auto& bike = bikes[s-1];
                    ofs << bike.brand << ',' << bike.model << ',' << bike.color << ',' << bike.capacityCC << ','
                        << bike.year << ',' << bike.licensePlate << ',' << bike.ownerUsername << ',' << bike.city << ','
                        << bike.pricePerDayCP << ',' << bike.minRenterRating << '\n';
                    ofs.close();
                    // Remove from active ebikes list and save
                    bikes.erase(bikes.begin() + (s-1));
                    DataManager::saveMotorbikes("ebikes.csv", bikes);
                    std::cout << "EBike has active rentals — its data was archived to Rental.csv and removed from active list.\n";
                    std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy);
                    break;
                }
                // No active rentals: safe to delete and remove related requests
                // remove bike
                bikes.erase(bikes.begin() + (s-1));
                DataManager::saveMotorbikes("ebikes.csv", bikes);
                // remove related rental requests
                reqs.erase(std::remove_if(reqs.begin(), reqs.end(), [&](const RentalRequest& r){ return r.ebikeLicensePlate == lp || r.motorbikeLicensePlate == lp; }), reqs.end());
                DataManager::saveRentalRequests("rental_requests.csv", reqs);
                std::cout << "EBike '" << lp << "' deleted.\n";
                std::cout << "Press Enter to continue..."; std::string dummy; std::getline(std::cin, dummy);
                break;
            }
            case 4: {
                std::cout << "Logging out...\n";
                running = false;
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
    }
}
