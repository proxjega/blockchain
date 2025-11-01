#include <iostream>
#include <vector>
#include "../include/user.h"
#include "../include/transaction.h"
#include "../include/blockchain.h"
#include "../include/logger.h"
#include "../include/blockchain.h"

using std::cin;
using std::cout;

void Case1(Blockchain &Btc, User &miner) {
    Block blockToMine(Btc, miner);
    blockToMine.Mine();
    Btc.validateAndAddBlock(blockToMine);
    getLogger().Log(Btc.getLastBlock());
}

void Case2(Blockchain &Btc) {
    int counter = 0;
    vector<User> miners;
    for (auto user : Btc.getUsers()) {
        miners.push_back(user.second);
        getLogger().Log(user.second);
        counter++;
        if (counter == 5 ) break;
    }
    vector<Block> blocks;
    for (int i = 0; i < 5; i++) {
        Block block(Btc, miners.at(i));
        blocks.push_back(block);
        getLogger().Log(block);
    }
    for (auto &block : blocks) {
        block.Mine5secs();
    }
    for (auto block : blocks) {
        getLogger().Log(block);
        Btc.validateAndAddBlock(block);
    }
}

void Case3(const Blockchain &Btc) {
    int blockchainSize = Btc.getBlockChain().size();
    cout << "Blockchain has " << blockchainSize << " blocks.\nWhich block info do you want?\n";
    int blockNumber;
    while (true) {
        cin >> blockNumber;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
            continue;
        }
        
        if (blockNumber >= 0 && blockNumber < blockchainSize) {
            auto block = Btc.getBlock(blockNumber);
            getLogger().Log(block);
            break; // Valid input, exit loop
        } else {
            cout << "Block number must be between 0 and " << (blockchainSize - 1) << ". Please try again: ";
        }
    }
}

void Case4(const Blockchain &Btc) {
    int blockchainSize = Btc.getBlockChain().size();
    cout << "Blockchain has " << blockchainSize << " blocks.\nWhich block info do you want?\n";
    int blockNumber;
    while (true) {
        cin >> blockNumber;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
            continue;
        }
        
        if (blockNumber >= 0 && blockNumber < blockchainSize) {
            auto block = Btc.getBlock(blockNumber);
            getLogger().Log(block);
            for (auto tx : block.getTransactions()) {
                getLogger().Log(tx);
            }
            break; // Valid input, exit loop
        } else {
            cout << "Block number must be between 0 and " << (blockchainSize - 1) << ". Please try again: ";
        }
    }
}

void Case5(const Blockchain &Btc) {
    cout << "Mempool's size: "<< Btc.getMemPool().size() << "\n";
    cout << "The most valuable transaction:\n";
    auto tx1 = Btc.getMemPool().at(Btc.getSortedHashVector().back());
    tx1.coutTx();
    cout << "The least valuable transaction:\n";
    auto tx2 = Btc.getMemPool().at(Btc.getSortedHashVector().at(0));
    tx2.coutTx();
}

void Case6(const Blockchain &Btc, const User& user) {
    auto userToShow = Btc.getUsers().at(user.getKey());
    getLogger().Log(userToShow);
}

void Case7(const Blockchain &Btc) {
    cout << "Which user info do you want to see?\n";
    string key;
    cin >> key;
    if(!Btc.getUsers().contains(key)) {
        cout << "User with this key was not found.\n";
        return;
    }
    auto userToShow = Btc.getUsers().at(key);
    getLogger().Log(userToShow);
}

void Case8(Blockchain &Btc, User& sender) {
    cout << "To who do you want to sent crypto?\n";
    string receiver;
    cin >> receiver;
    cout << "How many bitcoins do you want to send?\n";
    int amount = 0;
    while (true) {
        cin >> amount;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Enter correct amount: ";
            continue;
        }
        break;
    }
    Transaction tx(sender.getKey(), receiver, amount);
    Btc.addTransactionToMempool(tx);
} 