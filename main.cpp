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

// Save_PW is a function used to store the passwords entered from the user

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

// Load_pw is function used to load the passwords stored in the passwords.txt file into a hash table

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

// add_pw is function used to add the passwords entered by the user into a hashtable

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

    // Absolute path of the passwords.txt file

    string filename = "G:\\programming(CW1)\\passwords.txt";

    cout << "Welcome to the Password Manager!\n";

    // Establishing a login system with "shehab" as the username and "programming" as the password
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

    // Declaring a hash table with the size of ARRAY_SIZE which is 100

    PasswordEntry passwordTable[ARRAY_SIZE];

    // Initializing the load_pw function, which will load the passwords stored into a hash table

    load_pw(passwordTable, filename);

    // Establishing a simple user interface with 3 options, this will loop until the user exits from the code

    while (true) {
        cout << "\n1. Add new password\n";
        cout << "2. Retrieve password\n";
        cout << "3. Exit\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;

        // Utilizing a switch statement in order to execute different operations based on the user's preferences

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