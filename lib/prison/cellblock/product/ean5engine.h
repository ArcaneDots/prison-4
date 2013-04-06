/*
    Copyright (C) 2011  Ian Hollander <ianhollander at gmail.com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/


#ifndef EAN5ENGINE_H
#define EAN5ENGINE_H

#include "ean2engine.h"

namespace product
{
/**
 * 5 Digit Add-on symbol - often used for ISBN numbers
 *  
 * Can be appended to UPC A/E and EAN-13
 */ 
class Ean5Engine : public Ean2Engine
{

public:
    Ean5Engine(const QString& defaultString = ean5::DEFAULT_VALUE,
	       int blockSize = ean5::BLOCK_SIZE);
    virtual ~Ean5Engine();
    /**
   * Calculate EAN checksum digit used for parity encoding 
   * 
   * right to left, starting with  right-most value as odd, odd * 3, even * 1
   * 
   * @note Will not check whether supplied string has an invalid length. 
   * 
   * @param symbolArray array of symbol indexes
   * @return valid check digit
   */
  virtual int calculateCheckDigit(const shared::LookupIndexArray &symbolArray) const;
};
};
#endif // EAN5ENGINE_H
