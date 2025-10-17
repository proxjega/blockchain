#pragma once

#include "transaction.h"
#include <string>
#include <unordered_map>
#include <vector>

using std::vector;
using std::string;
using std::unordered_map;

class Transaction;

struct BlockHeader {
    // calculated by mining
    string hash;
    long long int nonce;

    // calculated after block creation
    string prevBlockHash;
    string timestamp;
    string version;
    string merkleRootHash;
    int difficultyTarget;  

    string ToString() {
        return prevBlockHash + timestamp + version + merkleRootHash + std::to_string(difficultyTarget);
    }
};

class Block {
    private:
        BlockHeader mHeader;
        vector<Transaction> mData;
    public:
        //default constructor, destructor and rule of five
        Block() = default;
        Block(const Block &BlockToCopy) = default;
        Block& operator=(const Block &BlockToCopy) = default;
        Block(Block &&BlockToMove) = default;
        Block& operator=(Block &&BlockToMove) = default;
        ~Block();

        //genesis block
        Block(const string &SatoshiKey);

        //block with 100 random transactions
        Block(const unordered_map<string, Transaction> &memPool);

        //getters and setters
        BlockHeader getHeader() {return mHeader;}
        vector<Transaction> getTransactions() ;
        void setHeader(BlockHeader header);
        void setTransactions(vector<Transaction> transactions);
        
        void CoutBlock();
        bool Mine(long long int number);
};