#pragma once

#include "transaction.h"
#include <string>
#include <unordered_map>
#include <vector>

using std::vector;
using std::string;
using std::unordered_map;

class Transaction;
class Blockchain;
class User;

struct BlockHeader {
    // calculated by mining
    string hash;
    string timestamp;
    long long int nonce;

    // calculated after block creation
    string prevBlockHash;
    string version;
    string merkleRootHash;
    int difficultyTarget;  

    string ToString() {
        return prevBlockHash + version + merkleRootHash + std::to_string(difficultyTarget);
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
        Block(const User &satoshi);

        //new block with 100 random transactions
        Block(const Blockchain &blockchain);

        //getters and setters
        BlockHeader getHeader() const {return mHeader;}
        string getHash() const {return mHeader.hash;}
        const vector<Transaction> &getTransactions() const  {return mData;}
        void setHeader(BlockHeader header);
        void setTransactions(vector<Transaction> transactions);
        
        void CoutBlock() const;
        bool Mine();
};