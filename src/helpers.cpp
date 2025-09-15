#include "../include/helpers.h"

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
    ofstream resultFile("results/HashedLineByLine.txt");
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