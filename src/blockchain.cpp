#include "../include/blockchain.h"
#include "../include/transaction.h"

string GetCurrentTimeStamp();
string HashFunction(const string &input);

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
    header.merkleRootHash = MerkleRootHash(mData);
    header.version = 1;
    header.difficultyTarget = 1;
    int nonce = 0;
    while (true) {
        
    }
    header.hash = "00000000839a8e6886ab5951d76f411475428afc90947ee320161bbf18eb6048"
}