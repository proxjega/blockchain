#include "../include/main.h"
#include "../include/user.h"
#include "../include/transaction.h"
#include "../include/blockchain.h"
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

string GetCurrentTimeStamp();
int main() {
    cout << GetCurrentTimeStamp() << "\n";  
    User Satoshi("Satoshi Nakamoto");
    ;

   
    // generate users
    std::unordered_map<string, User> users;
    users.reserve(1000);
    for (int i = 0; i < 1000; i++) {
        string name = "JohnSmith" + std::to_string(i);
        User user(name);
        users.insert({user.getKey(), user});
    }
    // vector for fast random access (for transactions generation)
    vector<User> usersVector;
    usersVector.reserve(1000);
    for (auto it = users.begin(); it!=users.end(); it++) {
        usersVector.push_back(it->second);
    }

    Blockchain Btc(Satoshi.getKey());
    Btc.getLastBlock().CoutBlock();
    //generate transactions
    Btc.GenerateMemPool(usersVector);
    // for (auto transaction : Btc.getMemPool()) {
    //     cout << "TX: " << transaction.getID() <<" from:" << transaction.getSender() << 
    //     " to:" << transaction.getReceiver() << " " << transaction.getAmount() << "\n hash: " << transaction.getHash() << "\n";
    // }


}