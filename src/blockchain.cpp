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


Blockchain::Blockchain(const string &satoshisKey) {
    // generate users and transactions
    GenerateUsers();
    GenerateMemPool();

    // add genesis block
    Block genesisBlock(satoshisKey);
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

Blockchain::~Blockchain(){
    blockList.clear();
    memPool.clear();
    users.clear();
}