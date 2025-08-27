#include <bits/stdc++.h>
using namespace std;

static inline bool is_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static string build_cipher_alphabet(string keyword) {
    // Uppercase, remove duplicates, then append remaining letters Z..A
    string key;
    key.reserve(26);
    vector<bool> seen(26, false);
    for (char c : keyword) {
        if (c >= 'a' && c <= 'z') c = char(c - 'a' + 'A');
        if (c >= 'A' && c <= 'Z') {
            int idx = c - 'A';
            if (!seen[idx]) { seen[idx] = true; key.push_back(c); }
        }
    }
    for (char c = 'Z'; c >= 'A'; --c) {
        int idx = c - 'A';
        if (!seen[idx]) { seen[idx] = true; key.push_back(c); }
        if (c == 'A') break; // prevent char underflow
    }
    if (key.size() != 26) {
        cerr << "Internal error: cipher alphabet length != 26\n";
        exit(EXIT_FAILURE);
    }
    return key;
}

static void usage(const char *prog) {
    cerr << "Usage: " << prog << " [-d] -kKEYWORD <input> <output>\n"
         << "  -d           Decrypt (default: encrypt)\n"
         << "  -kKEYWORD    Keyword for substitution (required)\n"
         << "Example:\n"
         << "  " << prog << " -kFEATHER plain.txt cipher.txt\n"
         << "  " << prog << " -d -kFEATHER cipher.txt plain_out.txt\n";
}

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    bool decrypt = false;
    string keyword;
    vector<string> positional;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-d") {
            decrypt = true;
        } else if (arg.rfind("-k", 0) == 0) {
            keyword = arg.substr(2);
        } else if (!arg.empty() && arg[0] == '-') {
            cerr << "Unknown option: " << arg << "\n";
            usage(argv[0]);
            return EXIT_FAILURE;
        } else {
            positional.push_back(arg);
        }
    }

    if (keyword.empty() || positional.size() != 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    string cipher = build_cipher_alphabet(keyword);
    array<char,26> forward{}; // A..Z -> cipher
    array<char,26> reverse{}; // cipher -> A..Z
    for (int i = 0; i < 26; ++i) {
        char p = char('A' + i);
        char c = cipher[i];
        forward[i] = c;
        reverse[c - 'A'] = p;
    }

    ifstream in(positional[0], ios::binary);
    if (!in) {
        cerr << "Error: could not open input file: " << positional[0] << "\n";
        return EXIT_FAILURE;
    }
    ofstream out(positional[1], ios::binary);
    if (!out) {
        cerr << "Error: could not open output file: " << positional[1] << "\n";
        return EXIT_FAILURE;
    }

    char ch;
    while (in.get(ch)) {
        if (is_letter(ch)) {
            bool lower = (ch >= 'a' && ch <= 'z');
            char up = lower ? char(ch - 'a' + 'A') : ch;
            char sub;
            if (!decrypt) {
                sub = forward[up - 'A'];
            } else {
                sub = reverse[up - 'A'];
            }
            if (lower) sub = char(sub - 'A' + 'a');
            out.put(sub);
        } else {
            out.put(ch);
        }
    }

    return 0;
}


