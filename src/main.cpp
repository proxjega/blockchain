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
    Btc.addTransactionToMempool(tx);

    getLogger().Log(Btc.getLastBlock());
    Block newblock(Btc);
    getLogger().Log(newblock);

    Btc.validateAndAddBlock(newblock);    
    newblock.Mine();
    getLogger().Log(newblock);
    cout << "Sizes before adding:" << Btc.getMemPool().size() << " " << Btc.getSortedHashVector().size() << "\n";
    Btc.validateAndAddBlock(newblock);
    cout << "Sizes after adding:" << Btc.getMemPool().size() << " " << Btc.getSortedHashVector().size() << "\n";

    while (true){
        Block blockToMine(Btc);
        blockToMine.Mine();
        Btc.validateAndAddBlock(blockToMine);
        getLogger().Log(Btc.getLastBlock());
        if(Btc.getMemPool().size()==0) break;
    }


}