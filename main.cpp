#include <bitset>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <vector>
#include <random>
#include <cmath>

using namespace std; 

int HashFunction(string input){
    constexpr int HASH_LENGTH = 32;
    vector<bitset<8>> bitVector;
    bitVector.reserve(input.length()*2);
    long int uniqueStringNumber = 0;
    int counter = 0;
    for (char c : input) {
        bitVector.push_back(bitset<8>(c));
        int castedChar = static_cast<int>(c);
        if (counter % 2 == 0) uniqueStringNumber+=castedChar;
        else uniqueStringNumber-=castedChar;
        cout << uniqueStringNumber << endl;
        counter++;
    }
    cout << "USN: " << uniqueStringNumber << endl;
    mt19937 engine(uniqueStringNumber);
    uniform_int_distribution<int> distInt(1, 16);
    cout << distInt(engine) << endl;
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