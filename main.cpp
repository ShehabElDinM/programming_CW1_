// Libraries used for the program
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

// Decrypting the encrypted password stored in the passwords.txt file using XOR

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

void add_pw(PasswordEntry passwordTable[]) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    string encryptedPassword = encrypt(password);
    int index = hash_fn(username);
    int step = secondary_hash_fn(username);
    while (!passwordTable[index].username.empty()) {
        index = (index + step) % ARRAY_SIZE;
    }
    passwordTable[index] = PasswordEntry(username, encryptedPassword);
    cout << "Password added successfully.\n";
}

// retrieve_pw is function used to retrieve passwords associated with a specific user that is inside a hashtable

void retrieve_pw(const PasswordEntry passwordTable[]) {
    string username;
    cout << "Enter username: ";
    cin >> username;
    int index = hash_fn(username);
    int step = secondary_hash_fn(username);
    bool found = false; // Flag to track if any matching username is found
    while (!passwordTable[index].username.empty()) {
        if (passwordTable[index].username == username) {
            cout << "Password for " << username << ": " << decrypt(passwordTable[index].encryptedPassword) << endl;
            found = true; // Set flag to true indicating at least one match is found
        }
        index = (index + step) % ARRAY_SIZE;
    }
    if (!found) {
        cout << "Password not found for " << username << endl;
    }
}

int main() {


    string filename = "passwords.txt";

    cout << "Welcome to the Password Manager!\n";


    string username, password;
    while (true) {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (username == "shehab" && password == "programming") {
            cout << "Login successful.\n";
            break; // Break out of the login loop
        } else {
            cout << "Invalid username or password. Please try again.\n";
        }
    }

    PasswordEntry passwordTable[ARRAY_SIZE];



    load_pw(passwordTable, filename);



    while (true) {
        cout << "\n1. Add new password\n";
        cout << "2. Retrieve password\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;



        switch (choice) {
            case 1:
                add_pw(passwordTable);
                save_pw(passwordTable, filename);
                break;
            case 2:
                retrieve_pw(passwordTable);
                break;
            case 3:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}