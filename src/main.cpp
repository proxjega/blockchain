#include "../include/main.h"


int main(int argc, char** argv) {
    // argument handling
    if (argc < 2) {
        cout << "Specify the option or file to Hash!\n";
        cout << " -t - hash all files from /testcases\n -g - generate test files\n -c - check results/HashedLineByLine.txt for collisions\n -a - check for avalanche effect\n -k - konstitucija.txt test";
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
            cout << "Hashing collisionTest.txt...\n";
            HashTestFileLineByLine("testcases/collisionTest.txt");
            return 0;
            break;
        case 'g':
            cout << "Generating files...\n";
            GenerateOneCharTestFiles();
            GenerateRandomCharactersTestFiles();
            GenerateRandomCharactersOneDifferentTestFiles();
            GenerateFileForCollisionTest();
            return 0;
            break;
        case 'c':
            cout << "Checking result file for collisions...\n";
            CheckHashesForCollision("results/hashResults.txt");
            return 0;
            break;
        case 'k':
            cout << "Checking konstitucija.txt file...\n";
            CheckKonstitucija();
            return 0;
            break;
        case 'a':
            cout << "Checking for avalanche effect...\n";
            CheckHashesForAvalancheEffect("results/hashResults.txt");
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