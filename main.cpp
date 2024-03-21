#include <iostream>
#include <string>

using namespace std;

// Declaring a variable to store the length of the hash table
const int array_num = 100;

// Constructor for the password manager
struct PasswordEntry {
    string username;
    string encryptedPassword;


    PasswordEntry(const string& user, const string& encPass) : username(user), encryptedPassword(encPass) {}
};

// Encrypting the passwords entered by the user
string encrypt(const string& password) {
    const char XOR_KEY = 0x77;
    string encrypted;
    for (char c : password) {
        encrypted += c ^ XOR_KEY;
    }
    return encrypted;
}

// Function for decrypting the encrypted password

string decrypt(const string& encryptedPassword) {
    const char XOR_KEY = 0x77;
    string decrypted;
    for (char c : encryptedPassword) {
        decrypted += c ^ XOR_KEY;
    }
    return decrypted;
}