#include <algorithm>
#include <cassert>
#include <format>
#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "../include/blockchain.h"
#include "../include/transaction.h"
#include "../include/user.h"
#include "../include/logger.h"

using std::cout;
using std::vector;

string HashFunction(const string &input);
string MerkleRootHash(const vector<Transaction> &transactions);


Blockchain::Blockchain() {
    // generate users and transactions
    getLogger().Log("Initializing blockchain...");
    GenerateUsers();
    getLogger().Log("Generated " + to_string(this->users.size()) + " users.");
    GenerateMemPool();
    getLogger().Log("Generated " + to_string(this->memPool.size()) + " txes and added to mempool.");

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
    for (int i = 1; i <= 10000; i++) {
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
    string blockHeight = to_string(BlockToAdd.getHeight());

    // check if transactions are in mempool
    for (auto tx : BlockToAdd.getTransactions()) {
        if (memPool.find(tx.getHash()) == memPool.end()) {
            getLogger().Log("Block #" + blockHeight + " denied, reason: Txs are not present in mempool");
            return;
        }
    }

    //check merkle root
    if(BlockToAdd.getHeader().merkleRootHash != MerkleRootHash(BlockToAdd.getTransactions())) {
        getLogger().Log("Block #" + blockHeight + " denied, reason: MerkleRoot differs");
        return;
    } 

    // check if the block has correct previous block hash
    if (BlockToAdd.getHeader().prevBlockHash != this->getLastBlock().getHash()) {
        getLogger().Log("Block #" + blockHeight + " denied, reason: Previous block hash does not match");
        return;
    }

    //check block hash
    if (BlockToAdd.getHash().empty()) {
        getLogger().Log("Block #" + blockHeight + " denied, reason: Hash is empty");
        return;
    }
    // check if hash is correct (according to proof of work)
    if (BlockToAdd.getHash()[0] != '0' || BlockToAdd.getHash()[1] != '0' || BlockToAdd.getHash()[2] != '0') {
        getLogger().Log("Block #" + blockHeight + " denied, reason: Hash does not match difficulty");
        return;
    }
    //check if hash is correct
    if (BlockToAdd.getHash() != HashFunction(BlockToAdd.getHeader().ToString() + std::to_string(BlockToAdd.getHeader().nonce))) {
        getLogger().Log("Block #" + blockHeight + " denied, reason: Hash is not correct");
        return;
    };

    this->ExecuteTransactions(BlockToAdd.getTransactions());
    blockList.push_back(BlockToAdd);
    getLogger().Log("Block #" + to_string(BlockToAdd.getHeight()) + " added to blockchain!");
    
}

void Blockchain::ExecuteTransactions(const vector<Transaction> &transactions){
    // update balances
    for(auto tx : transactions) {
        auto balance = users.at(tx.getReceiver()).getBalance();
        users.at(tx.getReceiver()).setBalance(balance + tx.getAmount());
    
        memPool.erase(tx.getHash());
        
    }

    //clear completed transactions from sortedTransactionHashes
    if (sortedTransactionHashes.size() > 100) 
        sortedTransactionHashes.erase(sortedTransactionHashes.end() - 100, sortedTransactionHashes.end());
    else    
        sortedTransactionHashes.clear();


}


// Add transaction with verifying
bool Blockchain::addTransactionToMempool(const Transaction &transactionToAdd){

    // check if users exists
    if (users.find(transactionToAdd.getSender()) == users.end() ||
        users.find(transactionToAdd.getReceiver()) == users.end()) {
        return false;
    }

    if (transactionToAdd.getAmount() <=0) {
        return false;
    }

    // check if user has enough to send
    if(transactionToAdd.getAmount() > users.at(transactionToAdd.getSender()).getBalance()) {
        return false;
    }
    
    // check hash
    if (transactionToAdd.getHash()!= 
        HashFunction(std::to_string(transactionToAdd.getID()) + transactionToAdd.getSender() + transactionToAdd.getReceiver() + std::to_string(transactionToAdd.getAmount()) )) {
        return false;
    }

    //insert
    memPool.insert({transactionToAdd.getHash(), transactionToAdd});

    // insert into sorted vector
    auto amountToInsert = transactionToAdd.getAmount();
    auto pos = std::lower_bound(
        sortedTransactionHashes.begin(), 
        sortedTransactionHashes.end(), 
        amountToInsert, 
        [this](const string& existingHash, int amount) -> bool{
        return memPool.at(existingHash).getAmount() < amount;
    });
    sortedTransactionHashes.insert(pos, transactionToAdd.getHash());

    return true;
}

Blockchain::~Blockchain(){
    blockList.clear();
    memPool.clear();
    users.clear();
    sortedTransactionHashes.clear();
}