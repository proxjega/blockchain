#pragma once
#include <string>

using std::string;

class User {
    private:
        string mName;
        string mPublicKey;
        int mBalance;
    public:
        User() = default;
        User(const string &name);
        User(const string &name, const string &key, int balance);
        ~User();

        string getName() const {return mName; }
        string getKey() const { return mPublicKey; }
        int getBalance() const { return mBalance; }
        void setBalance(int newBalance) { mBalance = newBalance;}
};