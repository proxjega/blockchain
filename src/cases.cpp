#include <iostream>
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
    Btc.validateAndAddBlock(blockToMine, miner);
    getLogger().Log(Btc.getLastBlock());
}

void Case2(Blockchain &Btc) {
    cout << "good\n";
}

void Case3(Blockchain &Btc) {
    cout << "good\n";
}

void Case4(Blockchain &Btc) {
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
            block.CoutBlock();
            break; // Valid input, exit loop
        } else {
            cout << "Block number must be between 0 and " << (blockchainSize - 1) << ". Please try again: ";
        }
    }
}

void Case5(Blockchain &Btc) {
    cout << "Mempool's size: "<< Btc.getMemPool().size() << "\n";
    cout << "The most valuable transaction:\n";
    auto tx1 = Btc.getMemPool().at(Btc.getSortedHashVector().back());
    tx1.coutTx();
    cout << "The least valuable transaction:\n";
    auto tx2 = Btc.getMemPool().at(Btc.getSortedHashVector().at(0));
    tx2.coutTx();
}

void Case6(Blockchain &Btc) {
    cout << "good\n";
}

void Case7(Blockchain &Btc) {
    cout << "good\n";
}