#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <vector>
#include <unordered_map>

#include "../include/blockchain.h"
#include "../include/transaction.h"
#include "../include/user.h"
#include "../include/logger.h"

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

    difficulty = 3;
    //satoshi
    User Satoshi("Satoshi Nakamoto", "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX", 0);
    this->users.insert({Satoshi.getKey(), Satoshi});

    // add genesis block
    Block genesisBlock(Satoshi);
    this->ExecuteTransactions(genesisBlock.getTransactions());
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
    std::uniform_int_distribution<int> amountDistribution(1, 100);
    memPool.reserve(10000);
    for (int i = 1; i <= 10000; i++) {
        User user1 = usersVector.at(userDistribution(mt));
        User user2 = usersVector.at(userDistribution(mt));
        int amount = amountDistribution(mt);
        Transaction transaction(user1.getKey(), user2.getKey(), amount);
        memPool.insert({transaction.getHash(), transaction});
        this->users.at(user1.getKey()).setPendingBalance(user1.getBalance() - amount);
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
        if(tx.getSender() == "Block Reward") continue;
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
    bool valid = true;
    for (int i = 0; i < difficulty; i++) {
        if (BlockToAdd.getHash()[i]!='0') valid = false;
    }
    if (!valid) {
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
        if (tx.getSender() == "Block Reward") {
            auto receiverBalance = users.at(tx.getReceiver()).getBalance();
            auto receiverPendingBalance = users.at(tx.getReceiver()).getPendingBalance();
            users.at(tx.getReceiver()).setPendingBalance(receiverPendingBalance + tx.getAmount());
            users.at(tx.getReceiver()).setBalance(receiverBalance + tx.getAmount());
            continue;
        }

        auto receiverPendingBalance = users.at(tx.getReceiver()).getPendingBalance();
        auto receiverBalance = users.at(tx.getReceiver()).getBalance();

        auto senderBalance = users.at(tx.getSender()).getBalance();

        users.at(tx.getReceiver()).setBalance(receiverBalance + tx.getAmount());
        users.at(tx.getSender()).setBalance(senderBalance - tx.getAmount());
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
        getLogger().Log("Tx " + transactionToAdd.getHash() + " denied. Reason: users not found");
        return false;
    }

    if (transactionToAdd.getAmount() <=0) {
        return false;
    }

    // check if user has enough to send
    if(transactionToAdd.getAmount() > users.at(transactionToAdd.getSender()).getPendingBalance()) {
        getLogger().Log("Tx " + transactionToAdd.getHash() + " denied. Reason: not enough balance");
        return false;
    }
    
    // check hash
    if (transactionToAdd.getHash()!= 
        HashFunction(transactionToAdd.getTimeStamp() + transactionToAdd.getSender() + transactionToAdd.getReceiver() + std::to_string(transactionToAdd.getAmount()) )) {
        getLogger().Log("Tx " + transactionToAdd.getHash() + " denied. Reason: hashes doesnt match");
        return false;
    }

    //insert
    memPool.insert({transactionToAdd.getHash(), transactionToAdd});
    users.at(transactionToAdd.getSender()).setPendingBalance(users.at(transactionToAdd.getSender()).getPendingBalance() - transactionToAdd.getAmount());
    users.at(transactionToAdd.getReceiver()).setPendingBalance(users.at(transactionToAdd.getReceiver()).getPendingBalance() + transactionToAdd.getAmount());
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

    getLogger().Log("Tx " + transactionToAdd.getHash() + " successfully added to mempool.");
    return true;
}

void Blockchain::addUser(const User& user){
    users.insert({user.getKey(), user});
    getLogger().Log(user);
}


Block Blockchain::getBlock(int n) const {
    int counter = 0;
    for (auto block : blockList) {
        if (counter == n) return block;
        counter ++;
    }
    return blockList.back();
}


Blockchain::~Blockchain(){
    blockList.clear();
    memPool.clear();
    users.clear();
    sortedTransactionHashes.clear();
}