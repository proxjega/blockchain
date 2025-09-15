#include <bitset>
#include <cstddef>
#include <cstdio>
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
#include <chrono>
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
    for (int i = 0; i < 1000; i++) {
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
    for (int i = 0; i < 256; i++) {
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
    string line;
    vector<string> lines;
    while (getline(resultFile, line)) {
        lines.push_back(line);
    }
    for (size_t i = 0; i < lines.size(); i++) {
        for (size_t j = i+1; j < lines.size(); j++) {
            if(lines[i]==lines[j]) cout << "Collision found!: \n";
        }
    }
}

void CheckKonstitucija(){
    ifstream file("konstitucija.txt");
    if (!file){
        cout << "File not found!\n";
        return;
    }
    string text;
    ofstream resultFile("konstitucijaResults.txt");
    for (int i = 1; i < 789; i*=2){
        text.clear();
        file.close();
        file.open("konstitucija.txt");
        for (int j = 0; j < i; j++) {
            string line;
            getline(file, line);
            text += line;
        }
        double avgTime;
        auto start = chrono::high_resolution_clock::now();
        for (int k = 0; k < 5; k++) {
            string hash = HashFunction(text);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> diff = end - start;
        avgTime = diff.count() / 5.0;
        resultFile << "Average time for hashing " << i << " lines: " << avgTime << "ms\n";
    }
}
int main(int argc, char** argv) {
    // argument handling
    if (argc < 2) {
        cout << "Specify the option or file to Hash!\n";
        cout << "-t - hash all files from /testcases\n -o - generate one char test files\n -r - generate 1000 random string files\n -d - generate random string files but with one different char\n -c - check resultHash.txt for collisions\n";
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
            break;
        case 'k':
            cout << "Checking konstitucija.txt file...\n";
            CheckKonstitucija();
            return 0;
            break;

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