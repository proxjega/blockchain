// hashfunction
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <iostream>
#include <iomanip>
#include <array>

using std::vector;
using std::string;
using std::mt19937;
using std::uniform_int_distribution;
using std::ostringstream;

// string HashFunction(const string &input){
//     constexpr int HASH_LENGTH = 64;
//     vector<int> integerVector;
//     integerVector.reserve(input.length()*2);
//     long int uniqueStringNumber = 1;
//     int counter = 0;
//     for (char c : input) {
//         int castedChar = static_cast<int>(c);
//         if (castedChar < 0) castedChar*= -1;
//         integerVector.push_back(castedChar);
//         if (counter % 4 == 0) uniqueStringNumber = std::abs(uniqueStringNumber) - 255;
//         if (counter % 3 == 0) uniqueStringNumber = (uniqueStringNumber+1) * -1;
//         if (counter % 2 == 0) uniqueStringNumber+=castedChar+1;
//         else uniqueStringNumber-=castedChar;
//         // cout << "castedchar " << castedChar << endl;
//         // cout << "USN " <<uniqueStringNumber << endl;
//         counter++;
//     }
//     // cout << "USN: " << uniqueStringNumber << endl;
//     mt19937 engine(uniqueStringNumber+3);
//     uniform_int_distribution<int> indexDistribution(0, HASH_LENGTH - 1);
//     uniform_int_distribution<int> randomDistribution(0, 15);
//     int hash[HASH_LENGTH];
//     for (int i = 0; i < HASH_LENGTH; i++) {
//         hash[i]=0;
//     }
//     for (auto el : integerVector) {
//         hash[indexDistribution(engine)]+= el;
//     }
//     for (int i = 0; i < HASH_LENGTH; i++) {
//         hash[i]+= randomDistribution(engine);
//         hash[i]=hash[i]%16;
//     }
//     // hash[0] = 0;
//     // convert to string
//     ostringstream oss;
//     for (int i = 0; i < HASH_LENGTH; i++) {
//         oss << std::hex << hash[i];
//     }
//     string hexstr = oss.str();
//     return hexstr;
// }

string HashFunction(const string &input) {
    constexpr int HASH_LENGTH = 64;
    
    // Ultra-fast seed calculation with bias toward smaller numbers
    int64_t seed = 1;
    for (size_t i = 0; i < input.size(); ++i) {
        int c = static_cast<unsigned char>(input[i]);
        
        // Operations that tend to keep seed small
        seed = (seed ^ c) + ((seed << 5) | (seed >> 27));
        seed = seed & 0xFFFFF; // Keep it small to bias distribution
        
        // Force more negative/positive flips to create zeros
        if (!(i % 2)) seed = -seed;
    }
    
    // RNG with biased distribution for more zeros
    thread_local std::mt19937 rng;
    rng.seed(static_cast<uint32_t>(std::abs(seed) + 3));
    
    // Biased distributions - higher probability of lower indices and values
    thread_local std::discrete_distribution<int> indexDist{
        8,7,6,5,4,3,2,1, // Higher weight for early indices (0-7)
        1,1,1,1,1,1,1,1, // Lower weight for middle indices
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1 // Lower weight for later indices
    };
    
    // Biased value distribution - much higher chance of zeros
    thread_local std::discrete_distribution<int> valueDist{
        8,  // 0 - highest probability
        4,   // 1
        2,   // 2  
        1,   // 3
        1,1,1,1,1,1,1,1,1,1,1,1 // very low probability for higher values
    };
    
    std::array<int, HASH_LENGTH> hash{};
    
    // Accumulate with bias toward early positions
    for (unsigned char c : input) {
        int idx = indexDist(rng);
        hash[idx] = (hash[idx] + c) % 256; // Keep values from overflowing
    }
    
    // Apply biased random values - increasing zero probability
    for (int i = 0; i < HASH_LENGTH; ++i) {
        hash[i] = (hash[i] + valueDist(rng)) % 16;
        
        // Extra bias: if value is already small, make it even more likely to be 0
        if (hash[i] < 4 && (rng() % 3 == 0)) {
            hash[i] = 0;
        }
    }
    
    
    // Fast hex conversion
    static constexpr char hex[] = "0123456789abcdef";
    char buffer[HASH_LENGTH];
    
    for (int i = 0; i < HASH_LENGTH; ++i) {
        buffer[i] = hex[hash[i] & 0xF];
    }
    
    return string(buffer, HASH_LENGTH);
}

string HashFunction(string input, bool salt){
    if (salt == true) {
        std::random_device rd;
        mt19937 engine(rd());
        uniform_int_distribution<int> charDistribution(33, 126);
        string s;
        for (int i = 0; i < 10; i++) {
            s+=static_cast<char>(charDistribution(engine));
        }
        input.append(s);
    }
    constexpr int HASH_LENGTH = 64;
    vector<int> integerVector;
    integerVector.reserve(input.length()*2);
    long int uniqueStringNumber = 1;
    int counter = 0;
    for (char c : input) {
        int castedChar = static_cast<int>(c);
        if (castedChar < 0) castedChar*= -1;
        integerVector.push_back(castedChar);
        if (counter % 4 == 0) uniqueStringNumber = std::abs(uniqueStringNumber) - 255;
        if (counter % 3 == 0) uniqueStringNumber = (uniqueStringNumber+1) * -1;
        if (counter % 2 == 0) uniqueStringNumber+=castedChar+1;
        else uniqueStringNumber-=castedChar;
        // cout << "castedchar " << castedChar << endl;
        // cout << "USN " <<uniqueStringNumber << endl;
        counter++;
    }
    // cout << "USN: " << uniqueStringNumber << endl;
    mt19937 engine(uniqueStringNumber+3);
    uniform_int_distribution<int> indexDistribution(0, HASH_LENGTH - 1);
    uniform_int_distribution<int> randomDistribution(0, 15);
    int hash[HASH_LENGTH];
    for (int i = 0; i < HASH_LENGTH; i++) {
        hash[i]=0;
    }
    for (auto el : integerVector) {
        hash[indexDistribution(engine)]+= el;
    }
    for (int i = 0; i < HASH_LENGTH; i++) {
        hash[i]+= randomDistribution(engine);
        hash[i]=hash[i]%16;
    }
    // convert to string
    ostringstream oss;
    for (int i = 0; i < HASH_LENGTH; i++) {
        oss << std::hex << hash[i];
    }
    string hexstr = oss.str();
    return hexstr;
}

inline uint32_t rotl(uint32_t x, int r) {
    return (x << r) | (x >> (32 - r));
}

// rotate right
inline uint32_t rotr(uint32_t x, int r) {
    return (x >> r) | (x << (32 - r));
}

std::string HashFunctionAI(const std::string &input) {
    constexpr int HASH_WORDS = 8;      // 8 * 32 = 256 bits

    uint32_t state[HASH_WORDS] = {
        0x12345678u, 0x9abcdef0u, 0xdeadbeefu, 0xfeedfaceu,
        0x0badc0deu, 0xcafebabeu, 0x8badf00du, 0x1337c0deu
    };

    // Mix each character into the state
    for (size_t i = 0; i < input.size(); i++) {
        uint32_t c = static_cast<unsigned char>(input[i]);
        uint32_t pos = i % HASH_WORDS;

        // XOR the character into the state
        state[pos] ^= c + i;

        // Rotate based on character value
        state[pos] = rotl(state[pos], (c % 31) + 1);

        // Shift and XOR with neighbor
        uint32_t neighbor = state[(pos + 1) % HASH_WORDS];
        state[pos] ^= rotr(neighbor, (i % 29) + 1);

        // Add some constant (helps avalanche)
        state[pos] += 0x9e3779b9u; // golden ratio
    }

    // Convert state into hex string
    std::ostringstream oss;
    for (int i = 0; i < HASH_WORDS; i++) {
        oss << std::hex << std::setw(8) << std::setfill('0') << state[i];
    }

    return oss.str();
}