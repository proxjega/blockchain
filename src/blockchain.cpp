#include "../include/blockchain.h"
#include "../include/transaction.h"
#include "../include/user.h"
#include <random>
#include <string>
#include <iostream>

using std::cout;

string GetCurrentTimeStamp();
string HashFunction(const string &input);
string MerkleRootHash(const vector<Transaction> &transactions);

//Genesis block 
Block::Block(const string &SatoshiKey){
    BlockHeader header;
    string nullHash = "";
    for (int i = 0 ; i < 64; i++) {
        nullHash.push_back('0');
    }
    header.prevBlockHash = nullHash;
    Transaction firstTransaction(0, "Block Reward", SatoshiKey, 50);
    this->mData.push_back(firstTransaction);
    header.timestamp = GetCurrentTimeStamp();
    header.version = "1";
    header.merkleRootHash = MerkleRootHash(mData);
    header.difficultyTarget = 1;
    long int nonce = 8400199;
    while (true) {
        cout << nonce << "\n";
        string hash = HashFunction(header.ToString() + std::to_string(nonce));
        if (hash[0] == '0' && hash[1] == '0' && hash[2] == '0') {
            header.hash = hash;
            header.nonce = nonce;
            break;
        }
        nonce++;
    }
    this->mHeader = header;
}

void Block::CoutBlock() {
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

    mHeader.prevBlockHash.clear();
    mHeader.timestamp.clear();
    mHeader.version.clear();
    mHeader.merkleRootHash.clear();
    mHeader.difficultyTarget = 0;
}

// Blockchain methods:

Blockchain::Blockchain(const string &satoshisKey) {
    // add genesis block
    Block genesisBlock(satoshisKey);
    blockList.push_back(genesisBlock);
    
    
}

void Blockchain::GenerateMemPool(const vector<User> &users) {
    //generate mempool
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> userDistribution(0, users.size() - 1);
    std::uniform_int_distribution<int> amountDistribution(0, 100);
    memPool.reserve(10000);
    for (int i = 1; i < 10000; i++) {
        User user1 = users.at(userDistribution(mt));
        User user2 = users.at(userDistribution(mt));
        Transaction transaction(i, user1.getKey(), user2.getKey(), amountDistribution(mt));
        memPool.push_back(transaction);
    }
}

Blockchain::~Blockchain(){
    blockList.clear();
    memPool.clear();
}