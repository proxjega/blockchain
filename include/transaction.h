#pragma once
#include <string>

using std::string;

class Transaction {
    private:
        string mTransactionHash;
        int mTransactionID;
        string mSender;
        string mReceiver;
        int mAmount;
    public:
        Transaction();
        Transaction(int id, const string& sender, const string& receiver, int amount);
        ~Transaction();

        string getSender() { return mSender;}
        string getReceiver() { return mReceiver; }
        string getHash() { return mTransactionHash;}
        int getAmount() { return mAmount;}
        int getID() { return mTransactionID; }
};