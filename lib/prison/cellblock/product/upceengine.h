/*
    Copyright (C) 2011  Ian gmail <ianhollander at gmail.com>

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


#ifndef UPCEENGINE_H
#define UPCEENGINE_H

#include "upcaengine.h"

using namespace shared;

namespace product 
{

class UpcEEngine : public UpcAEngine
{

public:
  
  /**
   * constructor
   *
   * @param defaultString default input used when user input is invalid
   * @param minLength minimum length of user input 
   * @param maxLength maximum length of user input
   * @param checkDigitOffset offset of the internal check digit.
   * @param blockSize formated block size
   * @param productCode constant indicating the current product code
   **/
  UpcEEngine(const QString &defaultString = upcE::DEFAULT_VALUE,
	      int minLength = upcE::MIN, 
	      int maxLength = upcE::MAX_LEN,
	      int checkDigitOffset = upcE::CHECK_DIGIT_OFFSET,
	      int blockSize = upcE::BLOCK_SIZE,
	      upc_common::PRODUCT_CODE_VALUES productCode = upc_common::PS__UPC_E);
  /**
   * destructor
   */
  virtual ~UpcEEngine();
  /**
   * Calculate UPC-E checksum digit for a particular barcode
   *
   * Calculate check sum based on expand UPC-A product code
   *
   * @param symbolArray array of symbol indexes
   * @return valid check digit
   */
  virtual int calculateCheckDigit(
    const LookupIndexArray &symbolArray) const;
  /**
   * Attempt to get UPC-E version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  virtual QStringList toUpcE() const;
  /**
   * Attempt to get UPC-A version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  virtual QStringList toUpcA() const;
  /**
   * Attempt to get EAN-13 version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  virtual QStringList toEan13() const;
protected:
  /**
   * Seperate digits into logical blocks based on encoded layout
   *
   * @note UPC-E  format  [0][1-6][(7)][8-9|13]
   * 
   * @param mainBlock first portion of the list of symbols 
   */
  virtual QStringList formatMainBlock(const QStringList &mainBlock) const;    
  /**
   * Encode complete number according to current barcode type
   *
   * UPC-E  format  [-][1-6][-][8-9|13]
   * 
   * @param symbolSrc full list of symbols
   * @param splitIndex index of the "end" of the first half
   */
  virtual void encodeSymbols(const QStringList &symbolSrc); 
  /**
   * Encode complete number according to current barcode type
   * 
   * Overload UPC-A version because UPC-E only has 1 block of digits
   *  
   * @note UPC-E  format  [-][1-6][-][8-9|13]
   * 
   * @param mainBlock first portion of the list of symbols 
   */ 
  virtual QStringList encodeMainBlock(const QStringList &mainBlock) const;   
  /**
   * Load all encoding patterns based on combo of system number (0-1) and check digit
   */
  virtual void fillWidthEncodingList();       
  /**
   * system number "0" encoding list
   */
  QStringList m_system0EncodingList;       
  /**
   * system number "1" encoding list
   */
  QStringList m_system1EncodingList;    
};
};
#endif // UPCEENGINE_H
