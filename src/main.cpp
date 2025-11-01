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
    cout << "Create your user: \n";
    cout << "What is your name?\n";
    string name;
    cin >> name;
    User yourUser(name, GenerateKey(), 0);
    Btc.addUser(yourUser);
    while (true) {
        cout << "Choose what to do:\n1 - mine 1 block\n2 - mine 5 blocks for 5 secs\n3 - get block info\n4 - get block info (super verbose)\n5 - get mempool info\n6 - my user info\n7 - user info by key\n8 - send crypto\n";
        char choose;
        cin >> choose;
        switch (choose) {
            case ('1'):
                Case1(Btc, yourUser);
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
            case('6'):
                Case6(Btc, yourUser);
                break;
            case('7'):
                Case7(Btc);
                break;
            case('8'):
                Case8(Btc, yourUser);
                break;
            default:
                cout << "Enter correct number\n";
        }
    }

   


}