#include "../include/main.h"
#include "../include/user.h"
#include "../include/transaction.h"
#include "../include/blockchain.h"
#include <cassert>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

string GetCurrentTimeStamp();
int main() {
    // create user satoshi (for genesis block)

    //create blockchain
    Blockchain Btc;
    
    Btc.getLastBlock().CoutBlock();
    Block newblock(Btc);
    newblock.CoutBlock();    
    // for (auto transaction : Btc.getMemPool()) {
    //     cout << "TX: " << transaction.second.getID() <<" from:" << transaction.second.getSender() << 
    //     " to:" << transaction.second.getReceiver() << " " << transaction.second.getAmount() << "\n hash: " << transaction.second.getHash() << "\n";
    // }


}