#include <iostream>

#include "../include/block.h"

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

// Block::Block(const unordered_map<string, Transaction> &memPool) {
//     for (int i = 0; i < 100; i++) {
//         memPool.b
//     }
// }


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