#ifndef __Fbr_Bozorth__
#define __Fbr_Bozorth__

#include <iostream>
#include <sstream>
#include <string>
#include "library.h"

class Bozorth
{
public:
    Bozorth(const std::string& probe, const std::string[MAX_TEMPLATES]);
    Bozorth(const Bozorth& bozorth) = delete;
    Bozorth& operator=(const Bozorth& bozorth) = delete;
    ~Bozorth();

    void Execute(int &score);
private:
    std::ostringstream m_Command;
    std::string m_Name;
};

class BozorthException : std::exception {
  public:
    explicit BozorthException(std::string message)
    : m_message(std::move(message)) { }

    [[nodiscard]] const char* what() const noexcept override
    {
      return m_message.c_str();
    }
  private:
    std::string m_message;
};

#endif /* defined(__Fbr_Bozorth__) */
