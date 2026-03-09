#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

class Appliance {
private:
    string name;
    double powerRating;
    double usageHours;

public:
    Appliance(string n, double p, double h)
        : name(n), powerRating(p), usageHours(h) {}

    string getName() const { return name; }
    double getPower() const { return powerRating; }
    double getHours() const { return usageHours; }

    double calculateEnergy() const {
        return (powerRating * usageHours) / 1000.0;
    }
};

// Global appliance list
vector<Appliance> appliances;
const string FILE_NAME = "appliances.txt";

// Save appliances to file
void saveToFile() {
    ofstream file(FILE_NAME);

    for (const Appliance& a : appliances) {
        file << a.getName() << "," << a.getPower() << "," << a.getHours() << endl;
    }

    file.close();
    cout << "Data saved successfully.\n";
}

// Load appliances from file
void loadFromFile() {
    ifstream file(FILE_NAME);
    if (!file) return; // if file doesn't exist, skip

    string line;
    while (getline(file, line)) {
        size_t pos1 = line.find(",");
        size_t pos2 = line.find(",", pos1 + 1);

        string name = line.substr(0, pos1);
        double power = stod(line.substr(pos1 + 1, pos2 - pos1 - 1));
        double hours = stod(line.substr(pos2 + 1));

        appliances.push_back(Appliance(name, power, hours));
    }

    file.close();
    cout << "Data loaded successfully.\n";
}

// Calculate total energy
double calculateTotalEnergy() {
    double totalEnergy = 0.0;
    for (const Appliance& a : appliances) {
        totalEnergy += a.calculateEnergy();
    }
    return totalEnergy;
}

// Register appliance
void registerAppliance() {
    string name;
    double power;
    double hours;

    cin.ignore(); // clear input buffer
    cout << "\n--- Register New Appliance ---\n";

    cout << "Enter appliance name: ";
    getline(cin, name);
    while (name.empty()) {
        cout << "Name cannot be empty. Enter again: ";
        getline(cin, name);
    }

    cout << "Enter power rating (Watts): ";
    cin >> power;
    while (cin.fail() || power <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Enter power > 0: ";
        cin >> power;
    }

    cout << "Enter daily usage hours (0 - 24): ";
    cin >> hours;
    while (cin.fail() || hours < 0 || hours > 24) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Enter hours (0 - 24): ";
        cin >> hours;
    }

    appliances.push_back(Appliance(name, power, hours));
    saveToFile();

    cout << "Appliance registered successfully!\n";
}

// View appliances
void viewAppliances() {
    if (appliances.empty()) {
        cout << "\nNo appliances registered.\n";
        return;
    }

    cout << "\n--- Registered Appliances ---\n";

    cout << left
         << setw(20) << "Name"
         << setw(12) << "Power(W)"
         << setw(12) << "Hours"
         << setw(15) << "Energy(kWh)" << endl;

    cout << "----------------------------------------------\n";

    for (const Appliance& a : appliances) {
        cout << left
             << setw(20) << a.getName()
             << setw(12) << a.getPower()
             << setw(12) << a.getHours()
             << setw(15) << fixed << setprecision(2)
             << a.calculateEnergy()
             << endl;
    }
}

// Calculate billing
double calculateBilling(double tariff) {
    return calculateTotalEnergy() * tariff;
}

// Perform billing calculation
void performBillingCalculation() {
    if (appliances.empty()) {
        cout << "\nNo appliances registered.\n";
        return;
    }

    double tariff;
    cout << "\nEnter tariff per kWh: ";
    cin >> tariff;
    while (cin.fail() || tariff <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid tariff. Enter value > 0: ";
        cin >> tariff;
    }

    double totalEnergy = calculateTotalEnergy();
    double totalCost = calculateBilling(tariff);

    cout << fixed << setprecision(2);
    cout << "\n------ BILLING SUMMARY ------\n";
    cout << "Total Energy Consumed : " << totalEnergy << " kWh\n";
    cout << "Tariff Rate           : " << tariff << " per kWh\n";
    cout << "Total Electricity Cost: " << totalCost << endl;
    cout << "-----------------------------\n";
}

// Main menu
int main() {
    loadFromFile();

    int choice;
    while (true) {
        cout << "\n--- ENERGY TRACKER MENU ---\n";
        cout << "1. Register Appliance\n";
        cout << "2. View Registered Appliances\n";
        cout << "3. Calculate Total Energy\n";
        cout << "4. Calculate Billing\n";
        cout << "5. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
            registerAppliance();
            break;
        case 2:
            viewAppliances();
            break;
        case 3:
            cout << fixed << setprecision(2);
            cout << "\nTotal Energy: " << calculateTotalEnergy() << " kWh\n";
            break;
        case 4:
            performBillingCalculation();
            break;
        case 5:
            cout << "\nGoodbye\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
