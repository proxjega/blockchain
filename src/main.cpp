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
    // for (auto transaction : Btc.getMemPool()) {
    //     cout << "TX: " << transaction.second.getID() <<" from:" << transaction.second.getSender() << 
    //     " to:" << transaction.second.getReceiver() << " " << transaction.second.getAmount() << "\n hash: " << transaction.second.getHash() << "\n";
    // }


}