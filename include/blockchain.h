#pragma once
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

struct BlockHeader{
    string prevBlockHash;
    string timestamp;
    string version;
    string merkleRootHash;
    long long int nonce;
    int difficultyTarget;   
};

class Block {
    private:
        BlockHeader mHeader;
        vector<Transaction> mData;
    public:
        Block();
};

class Blockchain {
    private:
        list<Block> list;
    public:
        void validateAndAddBlock(Block &BlockToAdd);
        Blockchain();
};