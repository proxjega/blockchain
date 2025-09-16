#include "../include/helpers.h"
#include <algorithm>
#include <bitset>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>

std::string HexToBin(const std::string& hex) {
    std::string bin;
    bin.reserve(hex.size() * 4);

    for (char c : hex) {
        int val;
        if (c >= '0' && c <= '9') val = c - '0';
        else if (c >= 'a' && c <= 'f') val = 10 + (c - 'a');
        else if (c >= 'A' && c <= 'F') val = 10 + (c - 'A');
        else throw std::invalid_argument("Invalid hex digit");

        for (int i = 3; i >= 0; --i)
            bin.push_back(((val >> i) & 1) ? '1' : '0');
    }

    return bin;
}

// hashes all files in testcases/ directory, and puts all hashes into one file
void HashTestFiles(){
    string path = "testcases";
    vector<string> files;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }

    ofstream hashResults("results/hashResults.txt");
    for (const auto& name : files) {
        ifstream file( "testcases/" + name, ios::binary);
        ostringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();
        // cout << content << endl;
        string hash = HashFunction(content);
        hashResults << hash << "\n";
    }
    hashResults.close();
    cout << "Results saved to hashResults.txt\n";
}

// hashes one file line by line, and puts hashes into one file
void HashTestFileLineByLine(string filename) {
    ifstream file(filename);
    if (!file){
        cout << "File not found!\n";
        return;
    }
    string name = filename.substr(filename.find('/'));
    name = name.erase(name.size()-4);
    ofstream resultFile("results/" + name + "HashedLineByLine.txt");
    string line;
    while (getline(file,line)) {
        string hash = HashFunction(line);
        resultFile << hash << "\n";
        line.clear();
    }
}

// checks hash file for collisions (compares each line with eachother)
void CollisionTest(string fileName){
    ifstream file(fileName);
    if (!file) {
        cout << "File not found\n";
        return;
    }
    string word, hex1, hex2;
    int collisionNum = 0;
    while (!file.eof()) {
        file >> word;
        hex1 = HashFunction(word);
        word.clear();
        file >> word;
        hex2 = HashFunction(word);
        if (hex1.compare(hex2) == 0) collisionNum++;
    }
    cout << "Found " << collisionNum << " collisions.\n";
}

void CheckKonstitucija(){
    ifstream file("konstitucija.txt");
    if (!file){
        cout << "File not found!\n";
        return;
    }
    string text;
    ofstream resultFile("results/konstitucijaResults.txt");
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
        auto start = std::chrono::high_resolution_clock::now();
        for (int k = 0; k < 5; k++) {
            string hash = HashFunction(text);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff = end - start;
        avgTime = diff.count() / 5.0;
        resultFile << "Average time for hashing " << i << " lines: " << avgTime << "ms\n";
    }
}

void AvalancheTest(string fileName) {
    ifstream file(fileName);
    if (!file) {
        cout << "File not found\n";
        return;
    }
    string word, hex1, hex2;
    vector<double> hexDifferences;
    vector<double> bitDifferences;
    hexDifferences.reserve(400000);
    bitDifferences.reserve(400000);
    while (!file.eof()) {
        file >> word;
        hex1 = HashFunction(word);
        word.clear();
        file >> word;
        hex2 = HashFunction(word);
        int sameChars = 0;
        for (size_t k = 0; k < hex1.size(); k++){
            if (hex1[k] == hex2[k]) sameChars++;
        }
        // if(sameChars==64) cout << word << "\n";
        double hexDifference = static_cast<double>(sameChars)  / 64.0 * 100.0;
        hexDifferences.push_back(hexDifference);
        
        int sameBits = 0;
        std::bitset<256> bitset1(HexToBin(hex1));
        std::bitset<256> bitset2(HexToBin(hex2));
        for (int k = 0; k < 256; k++) {
            if(bitset1[k] == bitset2[k]) sameBits++;
        }
        // if(sameBits==256) cout << word << "\n";
        double bitDifference = static_cast<double>(sameBits) / 256.0 * 100.0;
        bitDifferences.push_back(bitDifference);
    }
    
    ofstream resultFile("results/avalancheResults.txt");
    resultFile << "Results from " << fileName << "\n";
    resultFile << "HEX:\nmax: " << std::setprecision(5) << *std::max_element(hexDifferences.begin(), hexDifferences.end()) << "\n"
    << "min: " << std::setprecision(5) << *std::min_element(hexDifferences.begin(), hexDifferences.end()-1) << "\n"
    << "avg: " << std::setprecision(5) << std::accumulate(hexDifferences.begin(), hexDifferences.end()-1, 0) / hexDifferences.size() << "\n";
    resultFile << "BIT:\nmax: " << std::setprecision(5) << *std::max_element(bitDifferences.begin(), hexDifferences.end()) << "\n"
    << "min: " << std::setprecision(5) << *std::min_element(bitDifferences.begin(), bitDifferences.end()-1) << "\n"
    << "avg: " << std::setprecision(5) <<  std::accumulate(bitDifferences.begin(), bitDifferences.end()-1, 0) / bitDifferences.size() << "\n";
}

