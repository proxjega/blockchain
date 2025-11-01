#include "../include/main.h"
#include "../include/user.h"
#include "../include/transaction.h"
#include "../include/blockchain.h"
#include "../include/logger.h"
#include <cassert>

using std::cin;

string GetCurrentTimeStamp();
int main() {

    //initialization
    Blockchain Btc;
    getLogger().Log(Btc.getLastBlock());

    while (true) {
        cout << "Choose what to do:\n1 - mine 1 block\n2 - mine 5 blocks for 5 secs\n3 - validation check\n4 - get block info\n5 - get mempool info";
        char choose;
        cin >> choose;
        switch (choose) {
            case ('1'):
                Case1(Btc);
                break;
            case('2'):
                Case2(Btc);
                break;
            case('3'):
                Case3(Btc);
                break;
            case('4'):
                Case4(Btc);
                break;
            case('5'):
                Case5(Btc);
                break;
            default:
                cout << "enter correct number";
        }
    }
    Block newblock(Btc);
    getLogger().Log(newblock);

    Btc.validateAndAddBlock(newblock);    
    newblock.Mine();
    getLogger().Log(newblock);
    cout << "Sizes before adding:" << Btc.getMemPool().size() << " " << Btc.getSortedHashVector().size() << "\n";
    Btc.validateAndAddBlock(newblock);
    cout << "Sizes after adding:" << Btc.getMemPool().size() << " " << Btc.getSortedHashVector().size() << "\n";

    Transaction tx(3,"a","2",1000);
    Btc.addTransactionToMempool(tx);

    while (true){
        Block blockToMine(Btc);
        blockToMine.Mine();
        Btc.validateAndAddBlock(blockToMine);
        getLogger().Log(Btc.getLastBlock());
        if(Btc.getMemPool().size()==0) break;
    }


}