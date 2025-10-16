#include "../include/transaction.h"
#include <string>

string HashFunction(const string &input);

Transaction::Transaction(){
    mTransactionID = 0;
    mReceiver = "";
    mSender = "";
    mTransactionHash = "";
    mAmount = 0;
}

Transaction::Transaction(int id, const string& sender, const string& receiver, int amount) {
    mTransactionID = id;
    mSender = sender;
    mReceiver = receiver;
    mAmount = amount;
    mTransactionHash = HashFunction(std::to_string(id) + sender + receiver + std::to_string(amount) );
}

Transaction::~Transaction(){
    mTransactionHash.clear();
    mSender.clear();
    mReceiver.clear();
    mAmount = 0;
    mTransactionID = 0;
}