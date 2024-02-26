#include "pch.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <cstdint>


using namespace std;

#include "toolbox.h"

// Function to parse INI file and store values in a map
map<string, map<string, string>> parseIniFile(const string section, const string& filename, int& error )
{
    map<string, map<string, string>> iniData;

    error = 0;

    ifstream file(filename);
    if (!file.is_open()) {
        error = ERROR_FILE_DONT_EXIST;
        return iniData;
    }

    string line;
    string currentSection;

    while (getline(file, line)) 
    {
        if (section.length() && currentSection.length())
        {
            if (section.compare(currentSection) != 0)
                continue;
        }

        // Skip empty lines and comments
        if (line.empty() || line[0] == ';' || line[0] == '#')
            continue;

        // Check if this line represents a section
        if (line[0] == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.size() - 2);
            continue;
        }

        // Find key-value pairs
        size_t delimiterPos = line.find('=');
        if (delimiterPos == string::npos)
            continue;

        string key = line.substr(0, delimiterPos);
        string value = line.substr(delimiterPos + 1);

        // Remove leading/trailing whitespaces from key and value
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // Store key-value pair in map
        iniData[currentSection][key] = value;
    }

    file.close();
    return iniData;
}

std::string sha256(const std::string& input) {
    // Initialize SHA-256 context
    uint32_t hash[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    uint32_t k[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    // Padding
    std::string msg = input;
    msg += (char)0x80;
    while ((msg.size() * 8) % 512 != 448) {
        msg += (char)0x00;
    }
    uint64_t bit_length = input.size() * 8;
    for (int i = 0; i < 8; ++i) {
        msg += (char)(bit_length >> (56 - 8 * i));
    }

    // Process the message in successive 512-bit chunks
    for (size_t i = 0; i < msg.size(); i += 64) {
        uint32_t w[64] = { 0 };
        for (size_t j = 0; j < 16; ++j) {
            w[j] = (msg[i + j * 4] << 24) | (msg[i + j * 4 + 1] << 16) | (msg[i + j * 4 + 2] << 8) | (msg[i + j * 4 + 3]);
        }
        for (size_t j = 16; j < 64; ++j) {
            uint32_t s0 = (w[j - 15] >> 7 | w[j - 15] << 25) ^ (w[j - 15] >> 18 | w[j - 15] << 14) ^ (w[j - 15] >> 3);
            uint32_t s1 = (w[j - 2] >> 17 | w[j - 2] << 15) ^ (w[j - 2] >> 19 | w[j - 2] << 13) ^ (w[j - 2] >> 10);
            w[j] = w[j - 16] + s0 + w[j - 7] + s1;
        }

        // Initialize working variables
        uint32_t a = hash[0];
        uint32_t b = hash[1];
        uint32_t c = hash[2];
        uint32_t d = hash[3];
        uint32_t e = hash[4];
        uint32_t f = hash[5];
        uint32_t g = hash[6];
        uint32_t h = hash[7];

        // Compression function
        for (size_t j = 0; j < 64; ++j) {
            uint32_t S1 = (e >> 6 | e << 26) ^ (e >> 11 | e << 21) ^ (e >> 25 | e << 7);
            uint32_t ch = (e & f) ^ (~e & g);
            uint32_t temp1 = h + S1 + ch + k[j] + w[j];
            uint32_t S0 = (a >> 2 | a << 30) ^ (a >> 13 | a << 19) ^ (a >> 22 | a << 10);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        // Update hash values
        hash[0] += a;
        hash[1] += b;
        hash[2] += c;
        hash[3] += d;
        hash[4] += e;
        hash[5] += f;
        hash[6] += g;
        hash[7] += h;
    }

    // Produce the final hash value
    std::stringstream ss;
    for (int i = 0; i < 8; ++i) {
        ss << std::hex << std::setw(8) << std::setfill('0') << hash[i];
    }

    return ss.str();
}

/*int main() {
    string filename = "example.ini";
    int error = 0;
    map<string, map<string, string>> iniData = parseIniFile("", filename, error);

    // Accessing data
    for (const auto& section : iniData) {
        cout << "[" << section.first << "]" << endl;
        for (const auto& entry : section.second) {
            cout << entry.first << " = " << entry.second << endl;
        }
    }

    return 0;
}
*/