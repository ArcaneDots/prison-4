/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Ian gmail <ianhollander@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef UPCEENGINE_H
#define UPCEENGINE_H

#include "upcaengine.h"

using namespace barcodeEngine;

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
  virtual int calculateCheckValue(const SymbolList &symbols) const;
  /**
   * Attempt to get UPC-E version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  virtual SymbolList toUpcE() const;
  /**
   * Attempt to get UPC-A version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  virtual SymbolList toUpcA() const;
  /**
   * Attempt to get EAN-13 version of the inputed product code
   * 
   * @note not tested
   * 
   * @returns product code or empty list in case conversion is not possible
   */
  virtual SymbolList toEan13() const;
protected:
  /**
   * Seperate digits into logical blocks based on encoded layout
   *
   * @note UPC-E  format  [0][1-6][(7)][8-9|13]
   * 
   * @param mainBlock first portion of the list of symbols 
   */
  virtual QStringList formatMainBlock(const SymbolList &mainBlock) const;    
  /**
   * Encode complete number according to current barcode type
   *
   * UPC-E  format  [-][1-6][-][8-9|13]
   * 
   * @param symbolSrc full list of symbols
   * @param splitIndex index of the "end" of the first half
   */
  virtual void encodeSymbols(const SymbolList &symbolSrc); 
  /**
   * Encode complete number according to current barcode type
   * 
   * Overload UPC-A version because UPC-E only has 1 block of digits
   *  
   * @note UPC-E  format  [-][1-6][-][8-9|13]
   * 
   * @param mainBlock first portion of the list of symbols 
   */ 
  virtual QStringList encodeMainBlock(const SymbolList &mainBlock) const;   
  /**
   * Load all encoding patterns based on combo of system number (0-1) and check digit
   */
  virtual StringTableEntry fillWidthEncodingList();       
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
