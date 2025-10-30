#pragma once

#include "block.h"
#include "transaction.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../include/logger.h"
#include "../include/user.h"
#include <ios>
#include <chrono>

using std::cout;
using std::endl;

string Timestamp(){
    auto now = std::chrono::system_clock::now();
    auto now_ms = floor<std::chrono::milliseconds>(now);
    string time = std::format("[{:%Y-%m-%d %H:%M:%S}] ", now_ms);
    
    return time;
}


using std::string;
using std::ofstream;

class Logger {
    private:
    ofstream logFile;

    Logger(){
        logFile.open("log/blockchainLog.log", std::ios::app);
        if(!logFile) std::cerr << "Error opening logfile!" << endl;
        logFile << Timestamp() << "PROGRAM STARTED ----------------------" << endl;
        cout << Timestamp() << "PROGRAM STARTED ----------------------" << endl;
    }

    public:
    static Logger& getLogger(){
        static Logger logger_instance;
        return logger_instance;
    }
    
    ~Logger(){
        logFile.close();
    }
    
    void Log(const string &textToLog) {
        logFile << Timestamp() << textToLog << endl;
        cout << Timestamp() << textToLog  << endl;
    }
    void Log(const Block &blockToLog){
        BlockHeader header = blockToLog.getHeader();
        logFile << Timestamp() << "Block #" << header.height << ":" << endl
        << "Hash: " << header.hash << "" << endl
        << "nonce: " << header.nonce << "" << endl
        << "Previous block Hash: " << header.prevBlockHash << "" << endl
        << "Timestamp: " << header.timestamp << "" << endl
        << "version: " << header.version << "" << endl
        << "Merkle root hash: " << header.merkleRootHash << "" << endl
        << "Difficulty target: " << header.difficultyTarget << "" << endl
        << "Number of transactions: " << blockToLog.getTransactions().size() << "\n----------------" << endl;

        cout << Timestamp() << "Block #" << header.height << ":" << endl
        << "Hash: " << header.hash << "" << endl
        << "nonce: " << header.nonce << "" << endl
        << "Previous block Hash: " << header.prevBlockHash << "" << endl
        << "Timestamp: " << header.timestamp << "" << endl
        << "version: " << header.version << "" << endl
        << "Merkle root hash: " << header.merkleRootHash << "" << endl
        << "Difficulty target: " << header.difficultyTarget << "" << endl
        << "Number of transactions: " << blockToLog.getTransactions().size() << "\n----------------" << endl;
    }
    void Log(const Transaction &txToLog){
        logFile << Timestamp() << "Transaction #" << txToLog.getID() << ":" << endl
        << "From: " << txToLog.getSender()
        << "\nTo: " << txToLog.getReceiver()
        << "\nAmount:" << txToLog.getAmount()
        << "\nHash: " << txToLog.getHash() << "\n----------";
        cout << Timestamp() << "Transaction #" << txToLog.getID() << ":" << endl
        << "From: " << txToLog.getSender()
        << "\nTo: " << txToLog.getReceiver()
        << "\nAmount:" << txToLog.getAmount()
        << "\nHash: " << txToLog.getHash() << "\n----------";
    }
    void Log(const User &userToLog){
    logFile << Timestamp() << "User " << userToLog.getKey() << ":" << endl
    << "Name: " << userToLog.getName()
    << "\nBalance: " << userToLog.getBalance() << "\n----------";
    cout << Timestamp() << "User " << userToLog.getKey() << ":" << endl
    << "Name: " << userToLog.getName()
    << "\nBalance: " << userToLog.getBalance() << "\n----------";
    }
};


