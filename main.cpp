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
