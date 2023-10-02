/*
* MIT License
*
* Copyright (c) 2018 Derick Felix <derickfelix@zoho.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef __Fbr__Mindtct__
#define __Fbr__Mindtct__

#include <iostream>
#include <sstream>
#include <string>

class Mindtct
{
public:
  Mindtct(const std::string& name);
  Mindtct(const Mindtct& mindtct) = delete;
  Mindtct& operator=(const Mindtct& mindtct) = delete;
  ~Mindtct();

  void Execute();
private:
  std::ostringstream m_Command;
  std::string m_Name;
};

class MindtctException: public std::exception {
public:
	explicit MindtctException(const char* message)
		: m_message(message) { }

    [[nodiscard]] const char* what() const noexcept override
    {
        return m_message.c_str();
    }
private:
	std::string m_message;
};


#endif /* defined(__Fbr__Mindtct__)*/
