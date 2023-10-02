#include "Utils.h"
#include <fstream>
#include <string>

namespace Fbr {
int WriteFile(const std::string& path, const std::string& content) {
  std::ofstream file;
  file.open(path);
  if (!file.is_open()) {
    LOG("Failed to open file " + path + " for writing!");
    return -1;
  }
  file << content;
  file.close();
  return 0;
}

std::string RandomString(int length) {
  std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  std::string ret;
  int pos;
  while (ret.size() != length) {
    pos = ((rand() % (characters.size() - 1)));
    ret += characters.substr(pos, 1);
  }
  return ret;
}
}