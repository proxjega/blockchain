#pragma once
#include <string>

using std::string;

class User {
    private:
        string mName;
        string mPublicKey;
        int mBalance;
        int mPendingBalance;
    public:
        User() = default;
        User(const User &other) = default;
        User& operator=(const User &other) = default;
        User(const string &name);
        User(const string &name, const string &key, int balance);
        ~User();

        string getName() const {return mName; }
        string getKey() const { return mPublicKey; }
        int getBalance() const { return mBalance; }
        int getPendingBalance() const { return mPendingBalance; }
        void setBalance(int newBalance) { mBalance = newBalance;}
        void setPendingBalance(int newBalance) {mPendingBalance = newBalance;}
};
