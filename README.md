# Motorbike Rental Application

## Introduction
This is a command-line application for managing electric motorbike rentals, supporting three user roles: Guest, Member, and Admin. Data is stored in CSV files. The app provides features for searching, renting, managing, and rating motorbikes.

## Main Features

### Guest
- View a list of available motorbikes with limited information (brand, model, capacity, location)
- Cannot view ratings/comments
- Cannot send rental requests

### Member
- Register/login
- Register an electric motorbike for rent (one per member)
- Search, filter, and request to rent if eligible
- Top up Credit Points (CP) with password authentication
- Rate motorbikes and owners after rental
- Manage personal profile, change password
- View rental history and rating statistics

### Admin
- View all member profiles and all motorbikes without restrictions
- Manage and view statistics of users and motorbikes

## Rules & Constraints
- Credit point balance must never be negative
- Each member can register only one motorbike and rent only one at a time
- Rental requests cannot be cancelled once accepted
- Members without a valid license cannot rent motorbikes above 50cc

## Folder Structure
```
src/
	admin.cpp, admin.h, admin_login.cpp, admin_login.h
	data_manager.cpp, data_manager.h
	guest.cpp, guest.h
	main.cpp
	member.cpp, member.h
	models.h
	utils.cpp, utils.h
	*.csv (users, motorbikes, rental_requests, ...)
task/
	summary.txt, SED-APT 2025B - Group Project.pdf
README.md
```

## Usage
1. **Build:**
	 - Requires C++17 or later (g++, clang++)
	 - Example build command:
		 ```
		 g++ -o main.exe main.cpp data_manager.cpp admin.cpp member.cpp guest.cpp utils.cpp admin_login.cpp

		 ```
2. **Run:**
	 - Open terminal in the project folder and run:
		 ```
		 ./motorbike_app
		 ```
	 - (On Windows: `motorbike_app.exe`)

## Data Storage
- Data is stored in CSV files: `users.csv`, `motorbikes.csv`, `rental_requests.csv`, ...
- Data is automatically loaded on startup and saved on exit.

## Contribution
For contributions, feedback, or bug reports, please use GitHub Issues or contact the development team.

## Team Information
- Project: SED-APT 2025B - Motorbike Rental Application
- Members: ... (add team names if needed)