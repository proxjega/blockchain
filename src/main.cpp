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
    User Satoshi("Satoshi Nakamoto", "12c6DSiU4Rq3P4ZxziKxzrL5LmMBrzjrJX", 50);

    //create blockchain
    Blockchain Btc(Satoshi.getKey());
    
    Btc.getLastBlock().CoutBlock();

    

    // for (auto transaction : Btc.getMemPool()) {
    //     cout << "TX: " << transaction.getID() <<" from:" << transaction.getSender() << 
    //     " to:" << transaction.getReceiver() << " " << transaction.getAmount() << "\n hash: " << transaction.getHash() << "\n";
    // }


}