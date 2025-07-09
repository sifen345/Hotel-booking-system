#include <iostream>
#include <fstream>
#include <string>
#include <cctype>   // for isalpha
#include <ctime>    // for date/time calculation
using namespace std;

struct Customer {
    string name;
    string address;
    string phone;
    string roomType;
    int days;
    bool swimming;
    bool spa;
};

class Hotel {
private:
    Customer rooms[201];  // Rooms 1 to 200
    bool isBooked[201];

public:
    Hotel();
    void loadData();
    void saveData();
    void hotelInfo();
    void mainMenu();
    void bookRoom();
    void showCustomer();
    void showAllRooms();
    void checkout();
    float calculateBill(Customer c);
};

Hotel::Hotel() {
    for (int i = 1; i <= 200; i++) {
        isBooked[i] = false;
    }
}

void Hotel::loadData() {
    ifstream fin("hotel_data.txt");
    if (!fin) return;

    int roomNo;
    while (fin >> roomNo) {
        fin >> rooms[roomNo].name >> rooms[roomNo].address >> rooms[roomNo].phone;
        fin >> rooms[roomNo].days >> rooms[roomNo].roomType;
        fin >> rooms[roomNo].swimming >> rooms[roomNo].spa;
        isBooked[roomNo] = true;
    }
    fin.close();
}

void Hotel::saveData() {
    ofstream fout("hotel_data.txt");
    for (int i = 1; i <= 200; i++) {
        if (isBooked[i]) {
            fout << i << " "
                 << rooms[i].name << " "
                 << rooms[i].address << " "
                 << rooms[i].phone << " "
                 << rooms[i].days << " "
                 << rooms[i].roomType << " "
                 << rooms[i].swimming << " "
                 << rooms[i].spa << endl;
        }
    }
    fout.close();
}

void Hotel::hotelInfo() {
    cout << "==============================================" << endl;
    cout << "         WELCOME TO ROYAL BLUE HOTEL" << endl;
    cout << "==============================================" << endl;
    cout << "We have 200 rooms (AC & Non-AC)" << endl;
    cout << "AC: 2000 Birr/day | Non-AC: 1000 Birr/day" << endl;
    cout << "Swimming: 500 Birr/day | Spa: 800 Birr/day" << endl;
    cout << "10% discount + 5% tax on total bill" << endl;
    cout << "==============================================" << endl;
}

void Hotel::mainMenu() {
    loadData();

    string user, pass;
    cout << "Login" << endl;
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    if (user != "admin" || pass != "1234") {
        cout << "Invalid login. Exiting." << endl;
        return;
    }

    int choice;
    do {
        cout << endl;
        cout << "=============== MAIN MENU ====================" << endl;
        cout << "1. Book a Room" << endl;
        cout << "2. View Customer Record" << endl;
        cout << "3. View Room Status" << endl;
        cout << "4. Check Out" << endl;
        cout << "5. Exit" << endl;
        cout << "==============================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) bookRoom();
        else if (choice == 2) showCustomer();
        else if (choice == 3) showAllRooms();
        else if (choice == 4) checkout();
        else if (choice == 5) {
            saveData();
            cout << "Goodbye!" << endl;
        }
        else cout << "Invalid choice." << endl;

    } while (choice != 5);
}

void Hotel::bookRoom() {
    int roomNo;
    cout << "Enter Room Number to book (1-200): ";
    cin >> roomNo;

    if (roomNo < 1 || roomNo > 200) {
        cout << "Invalid room number." << endl;
        return;
    }
    if (isBooked[roomNo]) {
        cout << "Room already booked by " << rooms[roomNo].name << "." << endl;
        return;
    }

    Customer c;

    // Name validation - only letters and spaces
    do {
        cout << "Enter Name (letters and spaces only): ";
        cin.ignore(); getline(cin, c.name);
        bool valid = true;
        for (char ch : c.name) {
            if (!isalpha(ch) && ch != ' ') {
                valid = false;
                break;
            }
        }
        if (!valid || c.name.empty()) {
            cout << "Invalid name. Please use letters and spaces only." << endl;
            c.name = "";
        }
    } while (c.name == "");

    // Address validation - only letters and spaces
    do {
        cout << "Enter Address (letters and spaces only): ";
        getline(cin, c.address);
        bool valid = true;
        for (char ch : c.address) {
            if (!isalpha(ch) && ch != ' ') {
                valid = false;
                break;
            }
        }
        if (!valid || c.address.empty()) {
            cout << "Invalid address. Please use letters and spaces only." << endl;
            c.address = "";
        }
    } while (c.address == "");

    // Phone validation: must start with +251 and be 13 chars total
    do {
        cout << "Enter Phone (+251xxxxxxxxx): ";
        getline(cin, c.phone);
        if (c.phone.length() != 13 || c.phone.substr(0, 4) != "+251") {
            cout << "Invalid phone. Must start with +251 and be exactly 13 characters." << endl;
            c.phone = "";
        }
    } while (c.phone == "");

    // Date input with validation and clear error messages
    while (true) {
        string startDate, endDate;
        tm start = {}, end = {};

        cout << "Enter Start Date (YYYY-MM-DD): ";
        cin >> startDate;
        cout << "Enter End Date (YYYY-MM-DD): ";
        cin >> endDate;

        // Parse start date
        if (sscanf(startDate.c_str(), "%d-%d-%d", &start.tm_year, &start.tm_mon, &start.tm_mday) != 3) {
            cout << "Invalid start date format. Use YYYY-MM-DD." << endl;
            continue;
        }
        // Parse end date
        if (sscanf(endDate.c_str(), "%d-%d-%d", &end.tm_year, &end.tm_mon, &end.tm_mday) != 3) {
            cout << "Invalid end date format. Use YYYY-MM-DD." << endl;
            continue;
        }

        start.tm_year -= 1900;
        start.tm_mon -= 1;
        start.tm_hour = 0; start.tm_min = 0; start.tm_sec = 0;

        end.tm_year -= 1900;
        end.tm_mon -= 1;
        end.tm_hour = 0; end.tm_min = 0; end.tm_sec = 0;

        time_t t_start = mktime(&start);
        time_t t_end = mktime(&end);

        if (t_start == -1 || t_end == -1) {
            cout << "Error parsing dates. Please try again." << endl;
            continue;
        }

        // Get current date at midnight
        time_t now = time(nullptr);
        tm *today = localtime(&now);
        today->tm_hour = 0; today->tm_min = 0; today->tm_sec = 0;
        time_t t_today = mktime(today);

        if (difftime(t_start, t_today) < 0) {
            cout << "Error: Start date " << startDate << " cannot be before today." << endl;
            continue;
        }

        if (difftime(t_end, t_start) < 0) {
            cout << "Error: End date cannot be before start date." << endl;
            continue;
        }

        double diff = difftime(t_end, t_start) / (60*60*24);
        c.days = (int)diff;
        if (c.days < 1) c.days = 1;

        break;
    }

    cout << "Enter Room Type (AC / Non-AC): ";
    cin >> c.roomType;
    cout << "Want Swimming Pool (500 Birr/day)? (1=Yes 0=No): ";
    cin >> c.swimming;
    cout << "Want Spa (800 Birr/day)? (1=Yes 0=No): ";
    cin >> c.spa;

    rooms[roomNo] = c;
    isBooked[roomNo] = true;
    cout << "Room booked successfully for " << c.days << " days!" << endl;
}

void Hotel::showCustomer() {
    int roomNo;
    cout << "Enter Room Number: ";
    cin >> roomNo;

    if (roomNo < 1 || roomNo > 200) {
        cout << "Invalid room number." << endl;
        return;
    }

    if (isBooked[roomNo]) {
        Customer c = rooms[roomNo];
        cout << "----------------------------------------------" << endl;
        cout << "Room: " << roomNo << endl;
        cout << "Name: " << c.name << endl;
        cout << "Address: " << c.address << endl;
        cout << "Phone: " << c.phone << endl;
        cout << "Days: " << c.days << endl;
        cout << "Room Type: " << c.roomType << endl;
        cout << "Swimming: " << (c.swimming ? "Yes" : "No") << endl;
        cout << "Spa: " << (c.spa ? "Yes" : "No") << endl;
        cout << "----------------------------------------------" << endl;
    } else {
        cout << "Room is not booked." << endl;
    }
}

void Hotel::showAllRooms() {
    cout << "============== ROOM STATUS ====================" << endl;
    for (int i = 1; i <= 200; i++) {
        if (isBooked[i]) {
            Customer c = rooms[i];
            cout << "Room " << i << " - Booked by " << c.name
                 << " (" << c.roomType << ", " << c.days << " days";
            if (c.swimming) cout << ", Swimming";
            if (c.spa) cout << ", Spa";
            cout << ")" << endl;
        } else {
            cout << "Room " << i << " - Free" << endl;
        }
    }
}

float Hotel::calculateBill(Customer c) {
    float roomRate = (c.roomType == "AC" || c.roomType == "ac") ? 2000 : 1000;
    float swimCost = c.swimming ? 500 * c.days : 0;
    float spaCost = c.spa ? 800 * c.days : 0;
    float total = (roomRate * c.days) + swimCost + spaCost;
    float discount = total * 0.10;
    float tax = total * 0.05;
    return total - discount + tax;
}

void Hotel::checkout() {
    int roomNo;
    cout << "Enter Room Number to check out: ";
    cin >> roomNo;

    if (roomNo < 1 || roomNo > 200) {
        cout << "Invalid room number." << endl;
        return;
    }

    if (isBooked[roomNo]) {
        Customer c = rooms[roomNo];
        float bill = calculateBill(c);
        cout << "==============================================" << endl;
        cout << "Check Out Summary" << endl;
        cout << "Total bill for " << c.days << " days";
        if (c.swimming) cout << " + Swimming";
        if (c.spa) cout << " + Spa";
        cout << " = " << bill << " Birr" << endl;
        cout << "==============================================" << endl;
        isBooked[roomNo] = false;
    } else {
        cout << "Room is already free." << endl;
    }
}

int main() {
    Hotel h;
    h.hotelInfo();
    h.mainMenu();
    return 0;
}
