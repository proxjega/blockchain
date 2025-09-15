#include "../include/main.h"


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