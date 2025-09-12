#include <bitset>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>

using namespace std; 

int HashFunction(string input){
    
    return 0;
}

int main(int argc, char** argv) {
    // argument handling
    if (argc < 2) {
        cout << "Specify the file to Hash!" << endl;
        return 0;
    }
    if (!filesystem::exists(argv[1])) {
        cout << "File \"" << argv[1] << "\" does not exists!" << endl;
        return 0;
    }
    cout << "Hashing file: \"" << argv[1] << "\": "<< endl;

    // reading file
    std::ifstream file(argv[1], std::ios::binary);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    std::cout << content << endl; // prints UTF-8 directly

}