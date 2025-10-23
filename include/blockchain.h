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
        void ExecuteTransactions(const vector<Transaction> transactions); //TODO
        void GenerateUsers();
        void GenerateMemPool();
    public:
        //constructors
        Blockchain();
        ~Blockchain();

        //getters and setters
        const Block &getLastBlock() const { return blockList.back();}
        
        // protected getters 
        const list<Block> &getBlockChain() const {return blockList;}
        const unordered_map<string, Transaction> &getMemPool() const {return memPool;}
        const vector<string> &getSortedHashVector() const {return sortedTransactionHashes;}
        const unordered_map<string, User> &getUsers() const {return users;}

        bool addTransactionToMempool(const Transaction &transactionToAdd);

        void validateAndAddBlock(Block &BlockToAdd); // TODO
};
