#ifndef SHA256_H
#define SHA256_H

#include <string>

//hash methodu : djb2
static std::string sha256(std::string input) {
    unsigned long hash = 5381;
    for (char c : input) {
        hash = ((hash << 5) + hash) + c;
    }
    return std::to_string(hash);
}

#endif