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
        Transaction() = default;
        Transaction(int id, const string& sender, const string& receiver, int amount);
        ~Transaction();

        string getSender() const { return mSender;}
        string getReceiver() const { return mReceiver; }
        string getHash() const { return mTransactionHash;}
        int getAmount() const { return mAmount;}
        int getID() const { return mTransactionID; }
};