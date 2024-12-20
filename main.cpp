#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <map>
#include <string>
#include <set>
#include <sstream>
#include <cmath>

using namespace std;

class QuantumCoinTracker {
private:
    double cryptoAmount;
    bool mining;
    set<string> redeemedCodes;
    map<string, double> codeValues;

    string generateCode(double crypto) {
        ostringstream code;
        code << "QUANTUMCOIN:" << fixed << setprecision(4) << crypto << ":" << chrono::system_clock::now().time_since_epoch().count();
        return code.str();
    }

    pair<double, bool> parseCode(const string& code) {
        istringstream stream(code);
        string prefix;
        double amount;
        long long timestamp;

        getline(stream, prefix, ':');
        if (prefix != "QUANTUMCOIN") return {0.0, false};

        stream >> amount;
        if (stream.fail()) return {0.0, false};

        stream.ignore(1, ':');
        stream >> timestamp;
        if (stream.fail()) return {0.0, false};

        return {amount, true};
    }

    void updateUSDValue() {
        double conversionRate = 0.000015 / 0.0001;
        double usdValue = cryptoAmount * conversionRate;
        cout << "\nProjected Value: $" << fixed << setprecision(5) << usdValue << " USD\n";
    }

public:
    QuantumCoinTracker() : cryptoAmount(0.0), mining(false) {}

    void startMining() {
        mining = true;
        thread([this]() {
            while (mining) {
                this_thread::sleep_for(chrono::seconds(1));
                cryptoAmount += 0.0001;
                cout << "\rMined Quantum Coin: " << fixed << setprecision(4) << cryptoAmount;
                updateUSDValue();
            }
        }).detach();
    }

    void stopMining() {
        mining = false;
    }

    void sellCrypto(double amount) {
        if (amount <= 0 || amount > cryptoAmount) {
            cout << "Invalid amount to sell.\n";
            return;
        }

        cryptoAmount -= amount;
        string code = generateCode(amount);
        codeValues[code] = amount;
        cout << "Sold " << fixed << setprecision(4) << amount << " Quantum Coin. Your code: " << code << "\n";
    }

    void redeemCrypto(const string& code) {
        if (redeemedCodes.find(code) != redeemedCodes.end()) {
            cout << "Invalid or already redeemed code.\n";
            return;
        }

        auto [amount, valid] = parseCode(code);
        if (!valid) {
            cout << "Invalid code format.\n";
            return;
        }

        cryptoAmount += amount;
        redeemedCodes.insert(code);
        cout << "Successfully redeemed " << fixed << setprecision(4) << amount << " Quantum Coin!\n";
        updateUSDValue();
    }
};

int main() {
    QuantumCoinTracker tracker;
    int choice;

    while (true) {
        cout << "\n--- Quantum Coin Tracker ---\n";
        cout << "1. Start Mining\n";
        cout << "2. Stop Mining\n";
        cout << "3. Sell Quantum Coin\n";
        cout << "4. Redeem Quantum Coin\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                tracker.startMining();
                break;
            case 2:
                tracker.stopMining();
                break;
            case 3: {
                double amount;
                cout << "Enter amount to sell: ";
                cin >> amount;
                tracker.sellCrypto(amount);
                break;
            }
            case 4: {
                string code;
                cout << "Enter redemption code: ";
                cin.ignore();
                getline(cin, code);
                tracker.redeemCrypto(code);
                break;
            }
            case 5:
                tracker.stopMining();
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}
