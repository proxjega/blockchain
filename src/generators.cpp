#include <random>
#include <fstream>
#include <string>

using std::ofstream;
using std::mt19937;
using std::uniform_int_distribution;
using std::random_device;
using std::to_string;
using std::string;

//functions which generates files (for testing)
void GenerateFileForCollisionTest(){
    ofstream file("testcases/collisionTest.txt");
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(33, 126);
    for (int i = 0; i < 100000; i++){

        for (int j = 0; j < 10; j++) {
            file << static_cast<char>(dist(mt));
        }
        file << " ";
        for (int j = 0; j < 10; j++) {
            file << static_cast<char>(dist(mt));
        }
        file << "\n";

        for (int j = 0; j < 100; j++) {
            file << static_cast<char>(dist(mt));
        }
        file << " ";
        for (int j = 0; j < 100; j++) {
            file << static_cast<char>(dist(mt));
        }
        file << "\n";

        for (int j = 0; j < 500; j++) {
            file << static_cast<char>(dist(mt));
        }
        file << " ";
        for (int j = 0; j < 500; j++) {
            file << static_cast<char>(dist(mt));
        }
        file << "\n";

        for (int j = 0; j < 1000; j++) {
            file << static_cast<char>(dist(mt));
        }
        file << " ";
        for (int j = 0; j < 1000; j++) {
            file << static_cast<char>(dist(mt));
        }
        file << "\n";
    }
}

void GenerateFileForAvalancheEffectTest(){
    ofstream file("testcases/avalancheTest.txt");
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(33, 126);
    for (int i = 0; i < 100000; i++){
        
        string word;
        for (int j = 0; j < 10; j++) {
            word+= static_cast<char>(dist(mt));
        }
        file << word << " ";
        word[0]=='a' ? word[0]='b' : word[0]='a'; 
        file << word << "\n";
        word.clear();

        for (int j = 0; j < 100; j++) {
            word+= static_cast<char>(dist(mt));
        }
        file << word << " ";
        word[0]=='a' ? word[0]='b' : word[0]='a'; 
        file << word << "\n";
        word.clear();

        for (int j = 0; j < 500; j++) {
            word+= static_cast<char>(dist(mt));
        }
        file << word << " ";
        word[0]=='a' ? word[0]='b' : word[0]='a'; 
        file << word << "\n";
        word.clear();

        for (int j = 0; j < 1000; j++) {
            word+= static_cast<char>(dist(mt));
        }
        file << word << " ";
        word[0]=='a' ? word[0]='b' : word[0]='a'; 
        file << word << "\n";
        word.clear();
    }
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