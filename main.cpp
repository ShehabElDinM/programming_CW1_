#include <iostream>
#include <string>

using namespace std;

// Declaring a variable to store the length of the hash table
const int ARRAY_SIZE = 100;

// Constructor for the password manager
struct PasswordEntry {
    string username;
    string encryptedPassword;

    PasswordEntry() : username(""), encryptedPassword("") {}
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

// hash_fn is used to calculate the hash value for the hash tables

int hash_fn(const string& key) {
    int hash = 0;
    for (char c : key) {
        hash += c;
    }
    return hash % ARRAY_SIZE;
}

// (Double hashing) Utilizing a second hash value in order to resolve collisions
int secondary_hash_fn(const string& key) {
    int hash = 0;
    for (char c : key) {
        hash += c * 31; // Using a different prime multiplier for diversity
    }
    return (hash % (ARRAY_SIZE - 1)) + 1; // Ensure non-zero and less than ARRAY_SIZE
}