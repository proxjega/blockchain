#pragma once
#include <string>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <filesystem>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include <iomanip>

using std::string;
using std::vector;
using std::cout;
using std::ofstream;
using std::ostringstream;
using std::ifstream;
using std::ios;

string HashFunction(const string &input);
void GenerateRandomCharactersTestFiles();
void GenerateRandomCharactersOneDifferentTestFiles();
void GenerateOneCharTestFiles();
void HashTestFiles();
void CheckHashesForCollision(string resultFileName);
void CheckKonstitucija();
void GenerateFileForCollisionTest();
void HashTestFileLineByLine(string filename);