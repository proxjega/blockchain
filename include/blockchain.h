#pragma once
#include "transaction.h"
#include "block.h"
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

class Blockchain {
    private:
        list<Block> blockList;
        unordered_map<string, User> users;
        unordered_map<string, Transaction> memPool;
        vector<string> sortedTransactionHashes;
        
        // private methods (are called only by blockchain itself)
        void ExecuteTransactions(const Block &block, std::unordered_map<string, User> &users); //TODO
        void GenerateUsersAndTransactions();
        void GenerateMemPool(const vector<User> &users);
    public:
        //constructors
        Blockchain(const string &satoshisKey);
        ~Blockchain();

        //getters and setters
        Block getLastBlock() { return blockList.back();}
        
        // protected getters 
        const list<Block> &getBlockChain() const {return blockList;}
        const unordered_map<string, Transaction> &getMemPool() const {return memPool;}

        void validateAndAddBlock(Block &BlockToAdd); // TODO
};
