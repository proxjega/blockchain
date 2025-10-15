#pragma once
#include <string>

using std::string;

class Transaction {
    private:
        int mTransactionID;
        string mTransactionHash;
        string mSender;
        string mReceiver;
        int mAmount;
    public:
        Transaction(int id, const string& sender, const string& receiver, int amount);
};