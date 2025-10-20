#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "../include/blockchain.h"
#include "../include/transaction.h"
#include "../include/user.h"

using std::cout;
using std::unordered_map;
using std::vector;

string HashFunction(const string &input);


Blockchain::Blockchain() {
    // generate users and transactions
    GenerateUsers();
    GenerateMemPool();
    VerifyMemPool();

    //satoshi
    User Satoshi("Satoshi Nakamoto", "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX", 0);
    this->users.insert({Satoshi.getKey(), Satoshi});

    // add genesis block
    Block genesisBlock(Satoshi);
    // this->ExecuteTransactions(genesisBlock, users);
    blockList.push_back(genesisBlock);
}

void Blockchain::GenerateMemPool() {
    
    // vector for fast random access
    vector<User> usersVector;
    usersVector.reserve(1000);
    for (auto it = users.begin(); it!=users.end(); it++) {
        usersVector.push_back(it->second);
    }
    
    //generate mempool
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> userDistribution(0, usersVector.size() - 1);
    std::uniform_int_distribution<int> amountDistribution(0, 100);
    memPool.reserve(10000);
    for (int i = 1; i < 10000; i++) {
        User user1 = usersVector.at(userDistribution(mt));
        User user2 = usersVector.at(userDistribution(mt));
        Transaction transaction(i, user1.getKey(), user2.getKey(), amountDistribution(mt));
        memPool.insert({transaction.getHash(), transaction});
    }
    
    //create sorted transaction hashes vector (transactions with biggest sent amount will be processed first)
    for (auto transaction : memPool) {
        sortedTransactionHashes.push_back(transaction.first);
    }

    std::sort(sortedTransactionHashes.begin(), sortedTransactionHashes.end(), [this](const string& firstHash, const string& secondHash) -> bool{
        return memPool.at(firstHash).getAmount() < memPool.at(secondHash).getAmount();
    });
    assert( memPool.size() == sortedTransactionHashes.size());
}

void Blockchain::GenerateUsers(){
    users.reserve(1000);
    for (int i = 0; i < 1000; i++) {
        string name = "JohnSmith" + std::to_string(i);
        User user(name);
        users.insert({user.getKey(), user});
    }
}

void Blockchain::validateAndAddBlock(Block &BlockToAdd){
    bool add = true;
    // validate transactions
    for (auto Transaction : BlockToAdd.getTransactions()) {
        if(Transaction.getAmount() > users.at(Transaction.getSender()).getBalance()) {
            memPool.erase(Transaction.getHash());
            continue;
            add = false;
        }
        if (Transaction.getHash()!= 
        HashFunction(std::to_string(Transaction.getID()) + Transaction.getSender() + Transaction.getReceiver() + std::to_string(Transaction.getAmount()) )) {
            memPool.erase(Transaction.getHash());
            continue;
            add = false;
        }
    }
    

    if (add == false) {
        cout << "Block denied\n";
        return;
    }
}

Blockchain::~Blockchain(){
    blockList.clear();
    memPool.clear();
    users.clear();
}