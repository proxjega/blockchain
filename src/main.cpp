#include "../include/main.h"
#include "../include/user.h"
#include "../include/transaction.h"
#include "../include/blockchain.h"
#include <cassert>

string GetCurrentTimeStamp();
int main() {
    // create user satoshi (for genesis block)

    //create blockchain
    Blockchain Btc;
    Transaction tx(3,"a","2",1000);
    bool check = Btc.addTransactionToMempool(tx);
    if (!check) cout << "Could not add tx\n";

    Btc.getLastBlock().CoutBlock();
    Block newblock(Btc);
    newblock.CoutBlock();
    Btc.validateAndAddBlock(newblock);    
    newblock.Mine();
    newblock.CoutBlock();
    cout << "Sizes before adding:" << Btc.getMemPool().size() << " " << Btc.getSortedHashVector().size() << "\n";
    Btc.validateAndAddBlock(newblock);
    cout << "Sizes after adding:" << Btc.getMemPool().size() << " " << Btc.getSortedHashVector().size() << "\n";

    // for (auto transaction : Btc.getMemPool()) {
    //     cout << "TX: " << transaction.second.getID() <<" from:" << transaction.second.getSender() << 
    //     " to:" << transaction.second.getReceiver() << " " << transaction.second.getAmount() << "\n hash: " << transaction.second.getHash() << "\n";
    // }


}