#include "../include/blockchain.h"
#include "../include/transaction.h"
#include <string>

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
    header.version = 1;
    header.merkleRootHash = MerkleRootHash(mData);
    header.difficultyTarget = 1;
    int nonce = 0;
    while (true) {
        string hash = HashFunction(header.ToString() + std::to_string(nonce));
        if (hash[0] == '0' && hash[1] == '0' && hash[2] == '0') {
            header.hash = hash;
            header.nonce = nonce;
            break;
        }
        nonce++;
    }
}