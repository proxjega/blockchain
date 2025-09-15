#include "main.h"



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
    if (!std::filesystem::exists(argv[1])) {
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