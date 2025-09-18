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
#include <string>

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