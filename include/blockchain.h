#pragma once
#include "transaction.h"
#include <array>
#include <cstdint>
#include <list>
#include <unordered_map>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::array;
using std::list;
using std::unordered_map;

class Transaction;
class User;

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
        Block(const vector<Transaction> &memPool);

        //getters and setters
        BlockHeader getHeader() {return mHeader;}
        vector<Transaction> getTransactions() ;
        void setHeader(BlockHeader header);
        void setTransactions(vector<Transaction> transactions);
        
        void CoutBlock();
        bool Mine(long long int number);
};

class Blockchain {
    private:
        list<Block> blockList;
        vector<Transaction> memPool;
        void ExecuteTransactions(const Block &block, std::unordered_map<string, User> &users); //TODO
    public:
        //constructors
        Blockchain(const string &satoshisKey);
        ~Blockchain();

        //getters and setters
        Block getLastBlock() { return blockList.back();}
        
        // protected getters 
        const list<Block> &getBlockChain() const {return blockList;}
        const vector<Transaction> &getMemPool() const {return memPool;}

        void GenerateMemPool(const vector<User> &users);
        void validateAndAddBlock(Block &BlockToAdd); // TODO
};
