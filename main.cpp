#include <iostream>
#include <string>
#include <fstream>

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

void save_pw(PasswordEntry passwordTable[], const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            if (!passwordTable[i].username.empty()) {
                file << passwordTable[i].username << ":" << passwordTable[i].encryptedPassword << "\n";
            }
        }
        file.close();
        cout << "Passwords saved successfully.\n";
    } else {
        cerr << "Error: Unable to open file for writing.\n";
    }
}

void load_pw(PasswordEntry passwordTable[], const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string username = line.substr(0, pos);
                string encryptedPassword = line.substr(pos + 1);
                int index = hash_fn(username);
                int step = secondary_hash_fn(username);
                while (!passwordTable[index].username.empty()) {
                    index = (index + step) % ARRAY_SIZE;
                }
                passwordTable[index] = PasswordEntry(username, encryptedPassword);
            }
        }
        file.close();
        cout << "Passwords loaded successfully.\n";
    } else {
        cerr << "Error: Unable to open file for reading.\n";
    }
}
