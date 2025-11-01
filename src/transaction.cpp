#include "../include/transaction.h"
#include <string>
#include <iostream>

string HashFunction(const string &input);

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

void Transaction::coutTx() const {
    std::cout << "Transaction #" << this->getID() << " info:\n"
    << "From: " << this->getSender()
    << "\nTo: " << this->getReceiver()
    << "\nAmount:" << this->getAmount()
    << "\nHash: " << this->getHash() << "\n----------\n";
}