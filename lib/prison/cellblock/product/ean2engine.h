/*
    Copyright (C) 2011  Ian gmail <ianhollander@gmail.com>

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


#ifndef EAN2ENGINE_H
#define EAN2ENGINE_H

#include "upcaengine.h"

namespace product
{
/**
 * Mock object
 */
class Ean2Engine : public UpcAEngine
{

public:
  Ean2Engine(const QString& defaultString = ean2::DEFAULT_VALUE,
	      int blockSize = ean2::BLOCK_SIZE,
	     upc_common::PRODUCT_CODE_VALUES productCode = upc_common::PS__EAN_2);
  virtual ~Ean2Engine();
  /**
   * 
   */
  virtual QStringList processSymbolList(const QStringList &userSymbols);
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
   /**
   * Seperate digits into logical blocks based on encoded layout
   * 
   * UPC-E  format  [0][1-6][(7)][8-9|13]
   * UPC-A  format: [0][1-5][6-(10)][12-13|15]
   * EAN-8  format:    [0-4][5-(8)][9-11|13]
   * EAN-13 format: [0][1-6][7-(12)][13-14|17] 
   */
  virtual void formatSymbols(const QStringList& symbolSrc, int splitIndex);    
  /**
   * Encode complete number according to current barcode type
   * 
   * UPC-E  format  [-][1-6][-][8-9|13]
   * UPC-A  format: [0][1-5][6-(10)][12-13|15]
   * EAN-8  format:    [0-4][5-(8)][9-11|13]
   * EAN-13 format: [-][1-6][7 - 12][13-14|17] 
   */ 
  virtual void encodeSymbols(const QStringList& symbolSrc, int splitIndex);
    
};
};
#endif // EAN2ENGINE_H
