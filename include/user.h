#pragma once
#include <string>

using std::string;

class User {
    private:
        string mName;
        string mPublicKey;
        int mBalance;
    public:
        User();
        User(const string &name);
        User(const string &name, const string &key, int balance);
        ~User();

        string getName() {return mName; }
        string getKey() { return mPublicKey; }
        int getBalance() { return mBalance; }
        void setBalance(int newBalance) { mBalance = newBalance;}
};