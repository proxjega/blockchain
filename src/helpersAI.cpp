#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <algorithm>
#include <bitset>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>

using std::cout;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::ostringstream;
using std::ios;

string HashFunction(const string &input);
string sha256(const string &str);
string HashFunction(string input, bool salt);
std::string HashFunctionAI(const std::string &input);

std::string HexToBin(const std::string& hex);

// hashes all files in testcases/ directory, and puts all hashes into one file
void HashTestFilesAI(){
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
        string hash = HashFunctionAI(content);
        hashResults << hash << "\n";
    }
    hashResults.close();
    cout << "Results saved to hashResults.txt\n";
}

// hashes one file line by line, and puts hashes into one file
void HashTestFileLineByLineAI(string filename) {
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
        string hash = HashFunctionAI(line);
        resultFile << hash << "\n";
        line.clear();
    }
}

// hashes string pairs and check for collisions
void CollisionTestAI(string fileName){
    ifstream file(fileName);
    if (!file) {
        cout << "File not found\n";
        return;
    }
    string word, hex1, hex2;
    int collisionNum = 0;
    while (!file.eof()) {
        file >> word;
        hex1 = HashFunctionAI(word);
        word.clear();
        file >> word;
        hex2 = HashFunctionAI(word);
        if (hex1.compare(hex2) == 0) collisionNum++;
    }
    cout << "Found " << collisionNum << " collisions.\n";
}

void CheckKonstitucijaAI(){
    ifstream file("konstitucija.txt");
    if (!file){
        cout << "File not found!\n";
        return;
    }
    string text;
    ofstream resultFile("results/konstitucijaResultsAI.txt");
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
            string hash = HashFunctionAI(text);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff = end - start;
        avgTime = diff.count() / 5.0;
        resultFile << "Average time for hashing " << i << " lines: " << avgTime << "ms\n";
    }
}

// hashes string pairs and checks for avalanche effect
void AvalancheTestAI(string fileName) {
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
        hex1 = HashFunctionAI(word);
        word.clear();
        file >> word;
        hex2 = HashFunctionAI(word);
        int sameChars = 0;
        for (size_t k = 0; k < hex1.size(); k++){
            if (hex1[k] == hex2[k]) sameChars++;
        }
        // if(sameChars==64) cout << word << "\n\n\n";
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
    
    ofstream resultFile("results/avalancheResultsAI.txt");
    resultFile << "Results from " << fileName << "\n";
    resultFile << "HEX:\nmax: " << std::setprecision(5) << *std::max_element(hexDifferences.begin(), hexDifferences.end()) << "\n"
    << "min: " << std::setprecision(5) << *std::min_element(hexDifferences.begin(), hexDifferences.end()-1) << "\n"
    << "avg: " << std::setprecision(5) << std::accumulate(hexDifferences.begin(), hexDifferences.end()-1, 0) / hexDifferences.size() << "\n";
    resultFile << "BIT:\nmax: " << std::setprecision(5) << *std::max_element(bitDifferences.begin(), hexDifferences.end()) << "\n"
    << "min: " << std::setprecision(5) << *std::min_element(bitDifferences.begin(), bitDifferences.end()-1) << "\n"
    << "avg: " << std::setprecision(5) <<  std::accumulate(bitDifferences.begin(), bitDifferences.end()-1, 0) / bitDifferences.size() << "\n";
}


