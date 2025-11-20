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
#include <format>
#include <string>
#include "../include/transaction.h"

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

// hashes string pairs and check for collisions
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

// hashes string pairs and checks for avalanche effect
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
    
    ofstream resultFile("results/avalancheResults.txt");
    resultFile << "Results from " << fileName << "\n";
    resultFile << "HEX:\nmax: " << std::setprecision(5) << *std::max_element(hexDifferences.begin(), hexDifferences.end()) << "\n"
    << "min: " << std::setprecision(5) << *std::min_element(hexDifferences.begin(), hexDifferences.end()-1) << "\n"
    << "avg: " << std::setprecision(5) << std::accumulate(hexDifferences.begin(), hexDifferences.end()-1, 0) / hexDifferences.size() << "\n";
    resultFile << "BIT:\nmax: " << std::setprecision(5) << *std::max_element(bitDifferences.begin(), hexDifferences.end()) << "\n"
    << "min: " << std::setprecision(5) << *std::min_element(bitDifferences.begin(), bitDifferences.end()-1) << "\n"
    << "avg: " << std::setprecision(5) <<  std::accumulate(bitDifferences.begin(), bitDifferences.end()-1, 0) / bitDifferences.size() << "\n";
}

void Negriztamumas(){
    cout << "Hash(\"text\"): \n" << HashFunction("text") << "\n";
    cout << "Hash(\"text\" + salt): \n" << HashFunction("text", true) << "\n";
    cout << "Hash(\"text\" + salt): \n" << HashFunction("text", true) << "\n";
    cout << "Hash(\"text\" + salt): \n" << HashFunction("text", true) << "\n";
}

// ---blockchain helpers---

string GetCurrentTimeStamp(){
    auto now = std::chrono::system_clock::now();
    string time = std::format("{:%Y-%m-%d %H:%M:%S}", now);
    return time;
}

#include <bitcoin/system.hpp>
// Merkle Root Hash
string MerkleRootHash(const vector<Transaction> &transactions)
{
    bc::hash_list merkle;
     for (const auto& tx : transactions) {
        // Decode the hex string into a data_chunk
        bc::data_chunk decoded_data;
        bc::decode_base16(decoded_data, tx.getHash());

        // Ensure the decoded data is the correct size (32 bytes for a hash)
        if (decoded_data.size() == bc::hash_size) {
            // Create a hash_digest from the decoded data and add it to the hash list
            bc::hash_digest hash;
            std::copy(decoded_data.begin(), decoded_data.end(), hash.begin());
            merkle.push_back(hash);
        } else {
            // Handle the case where the string is not a valid hash
            std::cerr << "Invalid hash string: " << tx.getHash() << std::endl;
        }
    }
    // Stop if hash list is empty or contains one element
    if (merkle.empty())
    return bc::encode_base16(bc::null_hash); 
    else if (merkle.size() == 1)
    return bc::encode_base16(merkle[0]);
    // While there is more than 1 hash in the list, keep looping...
    while (merkle.size() > 1)
    {
        // If number of hashes is odd, duplicate last hash in the list.
        if (merkle.size() % 2 != 0)
        merkle.push_back(merkle.back());
        // List size is now even.
        assert(merkle.size() % 2 == 0);
        // New hash list.
        bc::hash_list new_merkle;
        // Loop through hashes 2 at a time.
        for (auto it = merkle.begin(); it != merkle.end(); it += 2)
        {
            // Join both current hashes together (concatenate).
            bc::data_chunk concat_data(bc::hash_size * 2);
            auto concat = bc::serializer<
            decltype(concat_data.begin())>(concat_data.begin());
            concat.write_hash(*it);
            concat.write_hash(*(it + 1));
            // Hash both of the hashes.
            bc::hash_digest new_root = bc::bitcoin_hash(concat_data);
            // Add this to the new list.
            new_merkle.push_back(new_root);
        }
        // This is the new list.
        merkle = new_merkle;
        // DEBUG output -------------------------------------
        // std::cout << "Current merkle hash list:" << std::endl;
        // for (const auto& hash: merkle)
        // std::cout << " " << bc::encode_base16(hash) << std::endl;
        // std::cout << std::endl;
        // --------------------------------------------------
    }
    // Finally we end up with a single item.
    return bc::encode_base16(merkle[0]);
}

