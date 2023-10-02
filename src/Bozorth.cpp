#include <stdio.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <array>
#include "Utils.h"
#include "Bozorth.h"

Bozorth::Bozorth(const std::string& probe, const std::string gallery[MAX_TEMPLATES])
{
    std::string gallery_str;
    for (int i = 0; i < MAX_TEMPLATES; i++) {
        if (gallery[i].empty()) {
            continue;
        }
        gallery_str += " " + gallery[i];
    }
    if (gallery_str.empty()) {
        throw BozorthException("No templates to compare!");
    }
    m_Command << "bozorth3 -q -T 30 -p " << probe << gallery_str;
}

Bozorth::~Bozorth()
{
}

void Bozorth::Execute(int &score)
{
    LOG("Executing Bozorth");
    std::array<char, 128> buffer{};
    std::string result;
    char* cmd = new char[m_Command.str().length() + 1];
    strcpy(cmd, m_Command.str().c_str());
    std::unique_ptr<FILE, decltype(&pclose) > pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw BozorthException("popen() failed!");
    }
    while (fgets(buffer.data(), (int)buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    if (isdigit(atoi(result.c_str()))) {
        throw BozorthException("Unexpected character: " + result + "!");
    }
    score = atoi(result.c_str());
}

