#include "../include/main.h"
#include "../include/user.h"
#include "../include/transaction.h"
#include "../include/blockchain.h"
#include <string>

string GetCurrentTimeStamp();
int main() {
    cout << GetCurrentTimeStamp() << "\n";  
    User Satoshi("Satoshi Nakamoto");
    Satoshi.getKey();
    vector<User> users;
    for (int i = 0; i < 1000; i++) {
        string name = "JohnSmith" + std::to_string(i);
        User user(name);
        users.push_back(user);
    }
    for (auto user : users) {
        cout << "User:" << user.getName() << " " << user.getKey() << " " << user.getBalance() << "\n";
    }
}