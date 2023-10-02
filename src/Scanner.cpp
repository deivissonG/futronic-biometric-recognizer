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
#include "library.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <chrono>
#include <thread>
#endif
#include <iostream>
#include <string>
#include <cstring>
#include <utility>

#include "Utils.h"
#include "ftrScanAPI.h"
#include "Scanner.h"
#include "bmpmini.hpp"


Scanner::Scanner(std::string  output, FBR_MATCH_CB& cb)
{
  m_Output = std::move(output);
  if (m_Output[0] == '"')
    m_Output.erase(0, 1);

  if (m_Output[m_Output.size() - 1] == '"')
    m_Output.pop_back();
  m_Device = ftrScanOpenDevice();
  m_Buffer = nullptr;

  m_MatchCB = cb;
  tries = 0;

  if(m_Device == nullptr)
  {
    if (ftrScanGetLastError() == 21) std::cout << "Permission denied, are you root?" << std::endl;
    throw ScannerException("Failed to open device!");
  }
}

Scanner::~Scanner()
{
  LOG("Device closed!");
  if (m_Device != nullptr) ftrScanCloseDevice(m_Device);
  if (m_Buffer != nullptr) free(m_Buffer);
}

void Scanner::ScanImage()
{
  if(!ftrScanGetImageSize(m_Device, &m_ImageSize))
  {
    throw ScannerException("Failed to get image size");
  }

  m_Buffer = (unsigned char *) malloc(m_ImageSize.nImageSize);

  LOG("Waiting for finger...");
  while(true)
  {
    tries++;
    ftrScanSetDiodesStatus(m_Device, (unsigned int)100/2, 0); // green led ON, red led OFF
    if(ftrScanIsFingerPresent(m_Device, NULL)) break;
    if (m_MatchCB != NULL)
    {
      if (m_MatchCB(FBR_FLAG_WAITING_FINGER) != FBR_RET_CODE_OK) {
        return;
      }
    }
    // sleep
#ifdef _WIN32
    Sleep(100);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
  }

  if (m_MatchCB != NULL)
  {
    if (m_MatchCB(FBR_FLAG_PROCESSING) != FBR_RET_CODE_OK) {
      return;
    }
  }
  if(ftrScanGetFrame(m_Device, m_Buffer, NULL))
  {
    LOG("Done\nWriting to file...");
    image::ImageView image = {m_ImageSize.nWidth, m_ImageSize.nHeight, 1, m_Buffer};

    image::BMPMini bmp;
    LOG("Writing to file... Done!");
    bmp.write(image, m_Output);
    if (m_MatchCB != NULL)
    {
      if (m_MatchCB(FBR_FLAG_SUCCESS) != FBR_RET_CODE_OK) {
        return;
      }
    }
  }
  else
  {
    unsigned long error = ftrScanGetLastError();
    // in case of moveable finger there is a way to try again
    if (error == FTR_ERROR_MOVABLE_FINGER || error == FTR_ERROR_EMPTY_FRAME)
    {
      if (m_MatchCB != NULL)
      {
        if (m_MatchCB(FBR_FLAG_MOVABLE_FINGER) != FBR_RET_CODE_OK) {
          return;
        }
      }
      if (tries < MAX_SCAN_RETRIES) {
        ScanImage();
        return;
      }
    }
    else
    {
      if (m_MatchCB != NULL)
      {
        m_MatchCB(FBR_FLAG_ERROR);
      }
      ShowError(error);
    }
    if (m_MatchCB != NULL)
    {
      m_MatchCB(FBR_FLAG_NO_CAPTURE);
    }
    throw ScannerException("Failed to get image!");
  }
}

void Scanner::ShowError(unsigned long error)
{
  std::cout << "Failed to get image:" << std::endl;
  switch(error)
  {
    case 0:
      std::cout << "OK" ;
      break;
    case FTR_ERROR_EMPTY_FRAME:	// ERROR_EMPTY
      std::cout << "- Empty frame -" << std::endl;
      break;
    case FTR_ERROR_NO_FRAME:
      std::cout <<  "- No frame -" << std::endl;
      break;
    case FTR_ERROR_USER_CANCELED:
      std::cout << "- User canceled -" << std::endl;
      break;
    case FTR_ERROR_HARDWARE_INCOMPATIBLE:
      std::cout << "- Incompatible hardware -" << std::endl;
      break;
    case FTR_ERROR_FIRMWARE_INCOMPATIBLE:
      std::cout << "- Incompatible firmware -" << std::endl;
      break;
    case FTR_ERROR_INVALID_AUTHORIZATION_CODE:
      std::cout << "- Invalid authorization code -" << std::endl;
      break;
    default:
      std::cout << "Unknown return code - " << error << std::endl;
  }
}
