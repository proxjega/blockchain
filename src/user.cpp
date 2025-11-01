#include "../include/user.h"

// public key generator
string GenerateKey();

//generates random balance from 100 to 1000000
int GenerateBalance();

// user with given name and random balance
User::User(const string &name) {
    mName = name;
    mPublicKey = GenerateKey();
    mBalance = GenerateBalance();
    mPendingBalance = mBalance;
}

User::User(const string &name, const string &key, int balance) {
    mName = name;
    mPublicKey = key;
    mBalance = balance;
    mPendingBalance = mBalance;
}


User::~User(){
    mName.clear();
    mPublicKey.clear();
    mBalance = 0;
    mPendingBalance = 0;
}

