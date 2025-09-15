#include <bitset>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <filesystem>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std; 

string HashFunction(const string &input){
    constexpr int HASH_LENGTH = 64;
    int hash[HASH_LENGTH];
    vector<int> integerVector;
    integerVector.reserve(input.length()*2);
    long int uniqueStringNumber = 1;
    int counter = 0;
    for (char c : input) {
        int castedChar = static_cast<int>(c);
        if (castedChar < 0) castedChar*= -1;
        integerVector.push_back(castedChar);
        if (counter % 4 == 0) uniqueStringNumber = abs(uniqueStringNumber) - 255;
        if (counter % 3 == 0) uniqueStringNumber = (uniqueStringNumber+1) * -1;
        if (counter % 2 == 0) uniqueStringNumber+=castedChar+1;
        else uniqueStringNumber-=castedChar;
        // cout << "castedchar " << castedChar << endl;
        // cout << "USN " <<uniqueStringNumber << endl;
        counter++;
    }
    // cout << "USN: " << uniqueStringNumber << endl;
    mt19937 engine(uniqueStringNumber+3);
    uniform_int_distribution<int> indexDistribution(0, HASH_LENGTH - 1);
    uniform_int_distribution<int> randomDistribution(0, 15);
    for (auto el : integerVector) {
        hash[indexDistribution(engine)]+= el;
    }
    for (int i = 0; i < HASH_LENGTH; i++) {
        hash[i]+= randomDistribution(engine);
        hash[i]=hash[i]%16;
    }
    // convert to string
    ostringstream oss;
    for (int i = 0; i < HASH_LENGTH; i++) {
        oss << std::hex << hash[i];
    }
    string hexstr = oss.str();
    return hexstr;
}

void GenerateOneCharTestFiles(){
    for (int i = 0; i < 256; i++) {
        ofstream testFile("testcases/testOneChar" + to_string(i) + ".txt");
        testFile << static_cast<char>(i);
        testFile.close();
    }
}

void GenerateRandomCharactersTestFiles(){
    for (int i = 0; i < 100; i++) {
        ofstream testFile("testcases/testRandomChars" + to_string(i) + ".txt");
        random_device rd;
        mt19937 mt(rd());
        uniform_int_distribution<int> dist(32, 126);
        for (int j = 0; j <1000; j++) {
            testFile << static_cast<char>(dist(mt));
        }
        testFile.close();
    }
}
void GenerateRandomCharactersOneDifferentTestFiles(){
    for (int i = 0; i < 100; i++) {
        ofstream testFile("testcases/testRandomCharsOneDifferent" + to_string(i) + ".txt");
        mt19937 mt(1);
        uniform_int_distribution<int> dist(32, 126);
        testFile << static_cast<char>(i+32);
        for (int j = 1; j <1000; j++) {
            testFile << static_cast<char>(dist(mt));
        }
        testFile.close();
    }
}

void HashTestFiles(){
    string path = "testcases";
    vector<string> files;

    for (const auto& entry : filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }

    ofstream hashResults("hashResults.txt");
    for (const auto& name : files) {
        ifstream file( "testcases/" + name, ios::binary);
        ostringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();
        // cout << content << endl;
        string hash = HashFunction(content);
        hashResults << hash << "\n";
    }
    cout << "Results saved to hashResults.txt\n";
}

void CheckHashesForCollision(string resultFileName){
    ifstream resultFile(resultFileName);
    if (!resultFile) {
        cout << "File not found\n";
        return;
    }
    cout << "Checking results for collisions...\n";
    string line;
    while (getline(resultFile, line)) {
        cout << line << endl;
        string secondLine;
        while (getline(resultFile, secondLine)) {
            if (line.compare(secondLine)==0) cout << "COLLISION FOUND!\n";
        }
    }
}
//to do: switch case for argument handling, fix checkHashes for collision;
int main(int argc, char** argv) {
    // argument handling
    if (argc < 2) {
        cout << "Specify the option or file to Hash!\n";
        return 0;
    }
    char option;
    if (argv[1][0]=='-') {
        option = argv[1][1];
    }
    switch (option) {
        case 't':
            cout << "Testing test files...\n";
            HashTestFiles();
            return 0;
            break;
        case 'o':
            cout << "Generating one char test files...\n";
            GenerateOneCharTestFiles();
            return 0;
            break;
        case 'r':
            cout << "Generating random character test files... \n";
            GenerateRandomCharactersTestFiles();
            return 0;
            break;
        case 'd':
            cout << "Generating random characters but one different test files... \n";
            GenerateRandomCharactersOneDifferentTestFiles();
            return 0;
            break;
        case 'c':
            cout << "Checking result file for collisions...\n";
            CheckHashesForCollision("hashResults.txt");
            return 0;
    }
    if (!filesystem::exists(argv[1])) {
        cout << "File \"" << argv[1] << "\" does not exists!\n";
        return 0;
    }
    cout << "Hashing file: \"" << argv[1] << "\": \n";

    // reading file
    ifstream file(argv[1], ios::binary);
    ostringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    string hash = HashFunction(content);
    cout << hash << "\n";
}