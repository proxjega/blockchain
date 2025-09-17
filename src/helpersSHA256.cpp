#include "../include/helpers.h"
string HexToBin(const std::string& hex);


// hashes string pairs and check for collisions
void CollisionTestSHA256(string fileName){
    ifstream file(fileName);
    if (!file) {
        cout << "File not found\n";
        return;
    }
    string word, hex1, hex2;
    int collisionNum = 0;
    while (!file.eof()) {
        file >> word;
        hex1 = sha256(word);
        word.clear();
        file >> word;
        hex2 = sha256(word);
        if (hex1.compare(hex2) == 0) collisionNum++;
    }
    cout << "Found " << collisionNum << " collisions.\n";
}

void CheckKonstitucijaSHA256(){
    ifstream file("konstitucija.txt");
    if (!file){
        cout << "File not found!\n";
        return;
    }
    string text;
    ofstream resultFile("results/konstitucijaResultsSHA256.txt");
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
            string hash = sha256(text);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff = end - start;
        avgTime = diff.count() / 5.0;
        resultFile << "Average time for hashing " << i << " lines: " << avgTime << "ms\n";
    }
}

// hashes string pairs and checks for avalanche effect
void AvalancheTestSHA256(string fileName) {
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
        hex1 = sha256(word);
        word.clear();
        file >> word;
        hex2 = sha256(word);
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
    
    ofstream resultFile("results/avalancheResultsSHA256.txt");
    resultFile << "Results from " << fileName << "\n";
    resultFile << "HEX:\nmax: " << std::setprecision(5) << *std::max_element(hexDifferences.begin(), hexDifferences.end()) << "\n"
    << "min: " << std::setprecision(5) << *std::min_element(hexDifferences.begin(), hexDifferences.end()-1) << "\n"
    << "avg: " << std::setprecision(5) << std::accumulate(hexDifferences.begin(), hexDifferences.end()-1, 0) / hexDifferences.size() << "\n";
    resultFile << "BIT:\nmax: " << std::setprecision(5) << *std::max_element(bitDifferences.begin(), hexDifferences.end()) << "\n"
    << "min: " << std::setprecision(5) << *std::min_element(bitDifferences.begin(), bitDifferences.end()-1) << "\n"
    << "avg: " << std::setprecision(5) <<  std::accumulate(bitDifferences.begin(), bitDifferences.end()-1, 0) / bitDifferences.size() << "\n";
}



