#include <iostream>
#include <string>
#include <chrono>

#include "../include/block.h"
#include "../include/blockchain.h"
#include "../include/user.h"
#include "../include/logger.h"

using std::cout;

string GetCurrentTimeStamp();
string HashFunctionAI(const string &input);
string HashFunction(const string &input);
string MerkleRootHash(const vector<Transaction> &transactions);

//Genesis block 
Block::Block(const User &Satoshi){
    string nullHash = "";
    for (int i = 0 ; i < 64; i++) {
        nullHash.push_back('0');
    }
    mHeader.prevBlockHash = nullHash;
    Transaction firstTransaction("2009-01-09 04:54:25", "Block Reward", Satoshi.getKey(), 50);
    this->mData.push_back(firstTransaction);
    mHeader.timestamp = "2009-01-09 04:54:25";
    mHeader.version = "1";
    mHeader.height = 0;
    mHeader.merkleRootHash = MerkleRootHash(mData);
    mHeader.difficultyTarget = 3;
    long long int nonce = 0;
    while (true) {
        string arg = mHeader.ToString() + std::to_string(nonce);
        string hash = HashFunction(arg);
        if (hash[0] == '0' && hash[1] == '0' && hash[2] == '0') {
            mHeader.hash = hash;
            mHeader.nonce = nonce;
            break;
        }
        nonce++;
    }
}

Block::Block(const Blockchain &blockchain, User &miner){
    mHeader.hash.clear();
    mHeader.prevBlockHash = blockchain.getLastBlock().getHash();
    mHeader.version = "1";
    mHeader.difficultyTarget = blockchain.getDifficulty();
    mHeader.height = blockchain.getLastBlock().getHeight() + 1;

    Transaction Reward("Block Reward", miner.getKey(), 50);
    mData.push_back(Reward);

    // push 100 tx
    if (blockchain.getSortedHashVector().size() > 100){
        for (auto it = blockchain.getSortedHashVector().end()-100; it!=blockchain.getSortedHashVector().end(); it++) {
            mData.push_back(blockchain.getMemPool().at(*it));
        }
    }
    //or push all remaining tx
    else {
        for (auto tx : blockchain.getSortedHashVector()) {
            mData.push_back(blockchain.getMemPool().at(tx));
        }
    }

    mHeader.merkleRootHash = MerkleRootHash(mData);
}

bool Block::Mine(){
    if(!mHeader.hash.empty()) return true;
    mHeader.nonce = 0;
    string blockNumber = to_string(this->mHeader.height);
    getLogger().Log("Starting to mine block #" + blockNumber + "...");
    auto miningStart = std::chrono::high_resolution_clock::now(); 
    while (true) {
        string arg = mHeader.ToString() + std::to_string(mHeader.nonce);
        string hash = HashFunction(arg);
        bool mined = true;
        for (int i = 0; i < mHeader.difficultyTarget; i++) {
            if (hash[i]!='0') mined = false;
        }
        if (mined == true) {
            mHeader.hash = hash;
            mHeader.timestamp = GetCurrentTimeStamp();
            auto miningEnd = std::chrono::high_resolution_clock::now(); 
            mHeader.miningTime = duration_cast<std::chrono::milliseconds>(miningEnd - miningStart);
            break;
        }
        mHeader.nonce++;
        if (mHeader.nonce % 1000000 == 0) getLogger().Log("(Block #" + blockNumber + "): " + to_string(mHeader.nonce) + " hashes checked...");
    }
    
    getLogger().Log("Block #" + blockNumber + " mined with nonce: " + to_string(mHeader.nonce) + ", Hash: " + mHeader.hash);
    return true;
}

bool Block::Mine5secs(){
    if(!mHeader.hash.empty()) return true;
    mHeader.nonce = 0;
    string blockNumber = to_string(this->mHeader.height);
    getLogger().Log("Starting to mine block #" + blockNumber + "...");
    auto miningStart = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    while (true) {
        string arg = mHeader.ToString() + std::to_string(mHeader.nonce);
        string hash = HashFunction(arg);
        bool mined = true;
        for (int i = 0; i < mHeader.difficultyTarget; i++) {
            if (hash[i]!='0') mined = false;
        }
        if (mined == true) {
            mHeader.hash = hash;
            mHeader.timestamp = GetCurrentTimeStamp();
            auto miningEnd = std::chrono::high_resolution_clock::now(); 
            mHeader.miningTime = duration_cast<std::chrono::milliseconds>(miningEnd - miningStart);
            break;
        }
        mHeader.nonce++;
        if (mHeader.nonce % 1000000 == 0) {
            getLogger().Log("(Block #" + blockNumber + "): " + to_string(mHeader.nonce) + " hashes checked...");
            end = std::chrono::high_resolution_clock::now();
            auto seconds = duration_cast<std::chrono::seconds>(end - miningStart);
            if (seconds.count() > 5) {
                getLogger().Log("Stopping mining after 5 seconds.");
                return false;
            }
        }
    }
    getLogger().Log("Block #" + blockNumber + " mined with nonce: " + to_string(mHeader.nonce) + ", Hash: " + mHeader.hash);
    return true;
}


void Block::CoutBlock() const {
    cout << "Block #" << mHeader.height << " info:" << endl
    << "Hash: " << mHeader.hash << "" << endl
    << "nonce: " << mHeader.nonce << "" << endl
    << "Previous block Hash: " << mHeader.prevBlockHash << "" << endl
    << "Timestamp: " << mHeader.timestamp << "" << endl
    << "version: " << mHeader.version << "" << endl
    << "Merkle root hash: " << mHeader.merkleRootHash << "" << endl
    << "Difficulty target: " << mHeader.difficultyTarget << "" << endl
    << "Number of transactions: " << this->getTransactions().size() << "\n----------------" << endl;
}

void Block::addTransaction(const Transaction &tx){
    this->mData.push_back(tx);
}


Block::~Block() {
    mHeader.hash.clear();
    mHeader.nonce = 0;

    mHeader.height = 0;
    mHeader.prevBlockHash.clear();
    mHeader.timestamp.clear();
    mHeader.version.clear();
    mHeader.merkleRootHash.clear();
    mHeader.difficultyTarget = 0;
}