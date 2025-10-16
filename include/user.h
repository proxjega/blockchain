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
        User(string name, string key, int balance);
        ~User();
};