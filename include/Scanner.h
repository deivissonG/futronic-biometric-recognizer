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
#ifndef __Fbr__Scanner__
#define __Fbr__Scanner__
#include "library.h"

#include <iostream>
#include "ftrScanAPI.h"

class ScannerException: public std::exception {
  public:
    explicit ScannerException(const char* message)
        : m_message(message) { }

    [[nodiscard]] const char* what() const noexcept override
    {
      return m_message.c_str();
    }
  private:
    std::string m_message;
};

class Scanner {
  public:
    Scanner(std::string  output, FBR_MATCH_CB& cb);
    Scanner(const Scanner& scanner) = delete;
    Scanner& operator=(const Scanner& scanner) = delete;
    ~Scanner();

    void ScanImage();
  private:
    static void ShowError(unsigned long error);
  private:
    int tries = 0;
    void *m_Device;
    std::string m_Output;
    unsigned char *m_Buffer;
    FTRSCAN_IMAGE_SIZE m_ImageSize;
    FBR_MATCH_CB m_MatchCB;
};

#endif /* defined(__Fbr__Scanner__) */
