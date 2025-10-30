#include <iostream>
#include <string>

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
    Transaction firstTransaction(0, "Block Reward", Satoshi.getKey(), 50);
    this->mData.push_back(firstTransaction);
    mHeader.timestamp = "2009-01-09 04:54:25";
    mHeader.version = "1";
    mHeader.height = 0;
    mHeader.merkleRootHash = MerkleRootHash(mData);
    mHeader.difficultyTarget = 3;
    long long int nonce = 130948;
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

Block::Block(const Blockchain &blockchain){
    mHeader.hash.clear();
    mHeader.prevBlockHash = blockchain.getLastBlock().getHash();
    mHeader.version = "1";
    mHeader.difficultyTarget = 3;
    mHeader.height = blockchain.getLastBlock().getHeight() + 1;

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
    while (true) {
        string arg = mHeader.ToString() + std::to_string(mHeader.nonce);
        string hash = HashFunction(arg);
        if (hash[0] == '0' && hash[1] == '0' && hash[2] == '0') {
            mHeader.hash = hash;
            mHeader.timestamp = GetCurrentTimeStamp();
            break;
        }
        mHeader.nonce++;
    }
    Logger::getLogger().Log("Block #" + to_string(this->mHeader.height) + " mined with nonce: " + to_string(mHeader.nonce) + " Hash: " + mHeader.hash);
    return true;
}


void Block::CoutBlock() const {
    cout << "Hash: " << mHeader.hash << "\n"
    << "nonce: " << mHeader.nonce << "\n"
    << "Previous block Hash: " << mHeader.prevBlockHash << "\n"
    << "Timestamp: " << mHeader.timestamp << "\n"
    << "version: " << mHeader.version << "\n"
    << "Merkle root hash: " << mHeader.merkleRootHash << "\n"
    << "Difficulty target: " << mHeader.difficultyTarget << "\n"
    << "Number of transactions: " << mData.size() << "\n\n";
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