#include "../include/logger.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../include/logger.h"
#include "../include/user.h"
#include <ios>
#include <chrono>

using std::cout;
using std::endl;
using std::string;
using std::ofstream;

string Timestamp(){
    auto now = std::chrono::system_clock::now();
    auto now_ms = floor<std::chrono::milliseconds>(now);
    string time = std::format("[{:%Y-%m-%d %H:%M:%S}] ", now_ms);
    
    return time;
}

Logger::Logger(){
    logFile.open("log/blockchainLog.log");
    if(!logFile) std::cerr << "Error opening logfile!" << endl;
    logFile << Timestamp() << "PROGRAM STARTED ----------------------" << endl;
    cout << Timestamp() << "PROGRAM STARTED ----------------------" << endl;
}

void Logger::Log(const string &textToLog) {
    logFile << Timestamp() << textToLog << endl;
    cout << Timestamp() << textToLog  << endl;
}

void Logger::Log(const Block &blockToLog){
    BlockHeader header = blockToLog.getHeader();
    logFile << Timestamp() << "Block #" << header.height << " info:" << endl
    << "Hash: " << header.hash << "" << endl
    << "nonce: " << header.nonce << "" << endl
    << "Previous block Hash: " << header.prevBlockHash << "" << endl
    << "Timestamp: " << header.timestamp << "" << endl
    << "version: " << header.version << "" << endl
    << "Merkle root hash: " << header.merkleRootHash << "" << endl
    << "Difficulty target: " << header.difficultyTarget << "" << endl
    << "Number of transactions: " << blockToLog.getTransactions().size() << "\n----------------" << endl;

    cout << Timestamp() << "Block #" << header.height << " info:" << endl
    << "Hash: " << header.hash << "" << endl
    << "nonce: " << header.nonce << "" << endl
    << "Previous block Hash: " << header.prevBlockHash << "" << endl
    << "Timestamp: " << header.timestamp << "" << endl
    << "version: " << header.version << "" << endl
    << "Merkle root hash: " << header.merkleRootHash << "" << endl
    << "Difficulty target: " << header.difficultyTarget << "" << endl
    << "Number of transactions: " << blockToLog.getTransactions().size() << "\n----------------" << endl;
}

void Logger::Log(const Transaction &txToLog){
    logFile << Timestamp() << "Transaction " << txToLog.getHash() << " info:\n"
    << "From: " << txToLog.getSender()
    << "\nTo: " << txToLog.getReceiver()
    << "\nAmount:" << txToLog.getAmount()
    << "\nTimestamp: " << txToLog.getTimeStamp() << "\n----------\n";
    cout << Timestamp() << "Transaction " << txToLog.getHash() << " info:\n"
    << "From: " << txToLog.getSender()
    << "\nTo: " << txToLog.getReceiver()
    << "\nAmount:" << txToLog.getAmount()
    << "\nTimestamp: " << txToLog.getTimeStamp() << "\n----------\n";
}

void Logger::Log(const User &userToLog){
    logFile << Timestamp() << "User " << userToLog.getKey() << " info:" << endl
    << "Name: " << userToLog.getName()
    << "\nBalance: " << userToLog.getBalance()
    << "\nPending balance: " << userToLog.getPendingBalance() << "\n----------" << endl;
    cout << Timestamp() << "User " << userToLog.getKey() << " info:" << endl
    << "Name: " << userToLog.getName()
    << "\nBalance: " << userToLog.getBalance()
    << "\nPending balance: " << userToLog.getPendingBalance() << "\n----------" << endl;
}

Logger::~Logger(){
    logFile.close();
}

Logger& getLogger() {
    static Logger instance; // initialized once, thread-safe
    return instance;
}
