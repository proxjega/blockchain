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
#include "../include/blockchain.h"

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
void GenerateFileForCollisionTest();
void GenerateFileForAvalancheEffectTest();
string GenerateKey();

void HashTestFiles();
void CollisionTest(string fileName);
void AvalancheTest(string fileName);
void Negriztamumas();

void CheckKonstitucija();
void HashTestFileLineByLine(string filename);

void CollisionTestSHA256(string fileName);
void CheckKonstitucijaSHA256();
void AvalancheTestSHA256(string fileName);

void CollisionTestAI(string fileName);
void CheckKonstitucijaAI();
void AvalancheTestAI(string fileName);

void Case1(Blockchain &Btc, User &miner);
void Case2(Blockchain &Btc);
void Case3(Blockchain &Btc);
void Case4(Blockchain &Btc);
void Case5(Blockchain &Btc);
void Case6(Blockchain &Btc, const User& user);
void Case7(Blockchain &Btc, User& sender);