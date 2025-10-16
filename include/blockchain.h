#pragma once
#include "transaction.h"
#include <array>
#include <cstdint>
#include <list>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::array;
using std::list;

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
        //constructors and rule of five
        Block();
        Block(const string &SatoshiKey);
        Block(const Block &BlockToCopy);
        Block operator=(const Block &BlockToCopy);
        Block(Block &&BlockToMove);
        Block operator=(Block &&BlockToMove);
        ~Block();

        //getters and setters
        BlockHeader getHeader() {return mHeader;}
        vector<Transaction> getTransactions() ;
        void setHeader(BlockHeader header);
        void setTransactions(vector<Transaction> transactions);

        bool Mine(long long int number);
};

class Blockchain {
    private:
        list<Block> blockList;
    public:
        //constructors
        Blockchain();
        ~Blockchain();

        //getters and setters
        Block getLastBlock() { return blockList.back();}
        list<Block> getBlockChain() {return blockList;}


        void validateAndAddBlock(Block &BlockToAdd);
};
