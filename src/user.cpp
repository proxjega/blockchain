#include "../include/user.h"

string GenerateKey();
int GenerateBalance();

User::User(){
    mName = "";
    mPublicKey = "";
    mBalance = 0;
}

User::User(string name) {
    mName = name;
    mPublicKey = GenerateKey();
    mBalance = GenerateBalance();
}

User::~User(){
    mName.clear();
    mPublicKey.clear();
    mBalance = 0;
}

