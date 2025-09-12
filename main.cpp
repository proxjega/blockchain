#include <bitset>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std; 

int HashFunction(string input){
    constexpr int HASH_LENGTH = 64;
    int hash[HASH_LENGTH];
    vector<int> bitVector;
    bitVector.reserve(input.length()*2);
    long int uniqueStringNumber = 1;
    int counter = 0;
    for (char c : input) {
        int castedChar = static_cast<int>(c);
        bitVector.push_back(castedChar);
        if (counter % 4 == 0) uniqueStringNumber = abs(uniqueStringNumber) - 255;
        if (counter % 3 == 0) uniqueStringNumber = (uniqueStringNumber+1) * -1;
        if (counter % 2 == 0) uniqueStringNumber+=castedChar+1;
        else uniqueStringNumber-=castedChar;
        // cout << "castedchar " << castedChar << endl;
        // cout << "USN " <<uniqueStringNumber << endl;
        counter++;
    }
    cout << "USN: " << uniqueStringNumber << endl;
    mt19937 engine(uniqueStringNumber+3);
    uniform_int_distribution<int> indexDistribution(0, HASH_LENGTH - 1);
    uniform_int_distribution<int> randomDistribution(0, 15);
    for (auto el : bitVector) {
        hash[indexDistribution(engine)]+= el;
    }

    for (int i = 0; i < HASH_LENGTH; i++) {
        hash[i]+= randomDistribution(engine);
        hash[i]=hash[i]%16;
        cout << hex << hash[i];
    }
    cout << endl;
    return 0;
}

int main(int argc, char** argv) {
    // argument handling
    if (argc < 2) {
        cout << "Specify the file to Hash!" << endl;
        return 0;
    }
    if (!filesystem::exists(argv[1])) {
        cout << "File \"" << argv[1] << "\" does not exists!" << endl;
        return 0;
    }
    cout << "Hashing file: \"" << argv[1] << "\": "<< endl;

    // reading file
    std::ifstream file(argv[1], std::ios::binary);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    HashFunction(content);
    // HashFunction("Abc");
}