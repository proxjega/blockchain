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
using std::string;
using std::ofstream;

class Logger {
    private:
    ofstream logFile;


    public:
    Logger();
    
    ~Logger();
    
    void Log(const string &textToLog);
    void Log(const Block &blockToLog);
    void Log(const Transaction &txToLog);
    void Log(const User &userToLog);
};

Logger& getLogger();


