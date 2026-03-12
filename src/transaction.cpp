#include "../include/transaction.h"
#include <string>
#include <iostream>

string HashFunction(const string &input);
string GetCurrentTimeStamp();

Transaction::Transaction(const string& timestamp, const string& sender, const string& receiver, int amount){
    mTimeStamp = timestamp;
    mSender = sender;
    mReceiver = receiver;
    mAmount = amount;
    mTransactionHash = HashFunction(mTimeStamp + sender + receiver + std::to_string(amount) );
}


Transaction::Transaction(const string& sender, const string& receiver, int amount) {
    mTimeStamp = GetCurrentTimeStamp();
    mSender = sender;
    mReceiver = receiver;
    mAmount = amount;
    mTransactionHash = HashFunction(mTimeStamp + sender + receiver + std::to_string(amount) );
}

Transaction::~Transaction(){
    mTransactionHash.clear();
    mSender.clear();
    mReceiver.clear();
    mAmount = 0;
    mTimeStamp.clear();
}

void Transaction::coutTx() const {
    std::cout << "Transaction " << this->getHash() << " info:\n"
    << "From: " << this->getSender()
    << "\nTo: " << this->getReceiver()
    << "\nAmount:" << this->getAmount()
    << "\nTimestamp: " << this->getTimeStamp() << "\n----------\n";
}