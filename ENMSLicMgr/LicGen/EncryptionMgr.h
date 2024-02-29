#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class CustomCypher {
private:
    int key;
    string salt;
    int randomLength;

public:
    CustomCypher(int _key, const string& _salt, int _randomLength) : key(_key), salt(_salt), randomLength(_randomLength) {}

    string encrypt(const string& message) {
        string encrypted = "";

        for (char c : message) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                char shifted = (c - base + key) % 26 + base;
                encrypted += shifted;
            }
            else {
                encrypted += c;
            }
        }

        return stringToHex(salt + encrypted + generateRandomChars(randomLength));
    }

    string decrypt(const string& input) {
        string encrypted = hexToString(input);
        string extracted_salt = encrypted.substr(0, salt.size());
        string extracted_message = encrypted.substr(salt.size(), encrypted.size() - salt.size() - randomLength);

        string decrypted = "";

        for (char c : extracted_message) {
            if (isalpha(c)) {
                char base = islower(c) ? 'a' : 'A';
                char shifted = (c - base - key + 26) % 26 + base;
                decrypted += shifted;
            }
            else {
                decrypted += c;
            }
        }

        return decrypted;
    }

private:
    string generateRandomChars(int length) {
        string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        string randomChars = "";

        for (int i = 0; i < length; ++i) {
            randomChars += chars[rand() % chars.length()];
        }

        return randomChars;
    }

    string stringToHex(const string& input) {
        stringstream hexStream;
        hexStream << hex << setfill('0');
        for (unsigned char c : input) {
            hexStream << setw(2) << static_cast<unsigned int>(c);
        }
        return hexStream.str();
    }

    string hexToString(const string& hexInput) {
        stringstream ss;
        for (size_t i = 0; i < hexInput.length(); i += 2) {
            string byteString = hexInput.substr(i, 2);
            char byte = static_cast<char>(stoul(byteString, nullptr, 16));
            ss << byte;
        }
        return ss.str();
    }
};


