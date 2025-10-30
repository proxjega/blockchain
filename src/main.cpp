#include "../include/main.h"
#include "../include/user.h"
#include "../include/transaction.h"
#include "../include/blockchain.h"
#include "../include/logger.h"
#include <cassert>

string GetCurrentTimeStamp();
int main() {

    //create blockchain
    Blockchain Btc;
    Transaction tx(3,"a","2",1000);
    bool check = Btc.addTransactionToMempool(tx);
    if (!check) Logger::getLogger().Log("Transaction " + tx.getHash() + " denied.");

    Logger::getLogger().Log(Btc.getLastBlock());
    Block newblock(Btc);
    Logger::getLogger().Log(newblock);

    Btc.validateAndAddBlock(newblock);    
    newblock.Mine();
    Logger::getLogger().Log(newblock);
    cout << "Sizes before adding:" << Btc.getMemPool().size() << " " << Btc.getSortedHashVector().size() << "\n";
    Btc.validateAndAddBlock(newblock);
    cout << "Sizes after adding:" << Btc.getMemPool().size() << " " << Btc.getSortedHashVector().size() << "\n";

    while (true){
        Block blockToMine(Btc);
        blockToMine.Mine();
        Btc.validateAndAddBlock(blockToMine);
        Logger::getLogger().Log(Btc.getLastBlock());
        if(Btc.getMemPool().size()==0) break;
    }



}