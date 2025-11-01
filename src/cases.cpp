#include <iostream>
#include "../include/user.h"
#include "../include/transaction.h"
#include "../include/blockchain.h"
#include "../include/logger.h"
#include "../include/blockchain.h"


using std::cout;

void Case1(Blockchain &Btc) {
    Block blockToMine(Btc);
    blockToMine.Mine();
    Btc.validateAndAddBlock(blockToMine);
    getLogger().Log(Btc.getLastBlock());
}

void Case2(Blockchain &Btc) {
    cout << "good\n";
}

void Case3(Blockchain &Btc) {
    cout << "good\n";
}

void Case4(Blockchain &Btc) {
    cout << "good\n";
}