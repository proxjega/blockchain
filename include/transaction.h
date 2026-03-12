#pragma once
#include <string>

using std::string;

class Transaction {
    private:
        string mTransactionHash;
        string mTimeStamp;
        string mSender;
        string mReceiver;
        int mAmount;
    public:
        Transaction() = default;
        Transaction(const string& timestamp, const string& sender, const string& receiver, int amount);
        Transaction(const string& sender, const string& receiver, int amount);
        ~Transaction();

        string getSender() const { return mSender;}
        string getReceiver() const { return mReceiver; }
        string getHash() const { return mTransactionHash;}
        int getAmount() const { return mAmount;}
        string getTimeStamp() const { return mTimeStamp; }
        void coutTx() const;
};