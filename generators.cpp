//functions which generates files (for testing)
#include "generators.h"

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