#include "../include/helpers.h"
#include <algorithm>
#include <bitset>
#include <cstddef>
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
void CheckHashesForCollision(string fileName){
    ifstream resultFile(fileName);
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

void CheckHashesForAvalancheEffect(string filename) {
    ifstream file(filename);
    if (!file){
        cout << "File not found!\n";
        return;
    }
    string line;
    vector<string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    vector<double> hexDifferences;
    vector<double> bitDifferences;
    for (size_t i = 0; i < lines.size(); i++) {
        for (size_t j = i+1; j < lines.size(); j++) {
            int sameChars = 0;
            for (size_t k = 0; k < lines[i].size(); k++){
                if (lines[i][k] == lines[j][k]) sameChars++;
            }
            double hexDifference = sameChars * 1.0 / 64.0 * 100;
            hexDifferences.push_back(hexDifference);
            
            int sameBits = 0;
            std::bitset<256> bitset1(HexToBin(lines[i]));
            std::bitset<256> bitset2(HexToBin(lines[j]));
            for (int k = 0; k < 256; k++) {
                if(bitset1[k] == bitset2[k]) sameBits++;
            }
            double bitDifference = sameBits * 1.0 / 256.0 * 100;
            bitDifferences.push_back(bitDifference);
        }
    }
    ofstream resultFile("results/avalancheResults.txt");
    resultFile << "Results from " << filename << "\n";
    resultFile << "HEX:\nmax: " << *std::max_element(hexDifferences.begin(), hexDifferences.end()) << "\n"
    << "min: " <<*std::min_element(hexDifferences.begin(), hexDifferences.end()) << "\n"
    << "avg: " << std::accumulate(hexDifferences.begin(), hexDifferences.end(), 0) / hexDifferences.size() << "\n";
    resultFile << "BIT:\nmax: " << *std::max_element(bitDifferences.begin(), hexDifferences.end()) << "\n"
    << "min: " <<*std::min_element(bitDifferences.begin(), bitDifferences.end()) << "\n"
    << "avg: " << std::accumulate(bitDifferences.begin(), bitDifferences.end(), 0) / bitDifferences.size() << "\n";
}

