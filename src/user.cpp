#include "../include/user.h"

string GenerateKey();
int GenerateBalance();

User::User(){
    mName = "";
    mPublicKey = "";
    mBalance = 0;
}

User::User(const string &name) {
    mName = name;
    mPublicKey = GenerateKey();
    mBalance = GenerateBalance();
}

User::User(const string &name, const string &key, int balance) {
    mName = name;
    mPublicKey = key;
    mBalance = balance;
}


User::~User(){
    mName.clear();
    mPublicKey.clear();
    mBalance = 0;
}

