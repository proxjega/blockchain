#pragma once

#include "transaction.h"
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

using std::vector;
using std::string;
using std::unordered_map;
using std::to_string;

class Transaction;
class Blockchain;
class User;

struct BlockHeader {
    // calculated by mining
    string hash;
    string timestamp;
    long long int nonce;

    // calculated after block creation
    long int height;
    string prevBlockHash;
    string version;
    string merkleRootHash;
    int difficultyTarget;  

    string ToString() {
        return to_string(height) + prevBlockHash + version + merkleRootHash + to_string(difficultyTarget);
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
        Block(const Blockchain &blockchain, User &miner);

        //getters and setters
        long long int getNonce() const {return mHeader.nonce;}
        BlockHeader getHeader() const {return mHeader;}
        string getHash() const {return mHeader.hash;}
        long int getHeight() const {return mHeader.height;}
        const vector<Transaction> &getTransactions() const  {return mData;}
        void setHeader(BlockHeader header);
        void setTransactions(vector<Transaction> transactions);

        void addTransaction(const Transaction &tx);

        void CoutBlock() const;
        bool Mine();
        bool Mine5secs();
};