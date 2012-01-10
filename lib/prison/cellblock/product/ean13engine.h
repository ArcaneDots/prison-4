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


#ifndef EAN13ENGINE_H
#define EAN13ENGINE_H

#include "upcaengine.h"

namespace product
{
class Ean13Engine :  public UpcAEngine 
{
public:
  Ean13Engine(const QString& defaultString = ean13::DEFAULT_VALUE, 
	      int minLength = ean13::MIN, 
	      int maxLength = ean13::MAX_LEN, 
	      int checkDigitOffset = ean13::CHECK_DIGIT_OFFSET, 
	      int blockSize = ean13::BLOCK_SIZE,
	      upc_common::PRODUCT_CODE_VALUES productCode = upc_common::PS__EAN_13);
  virtual ~Ean13Engine();   
  /**
   * Class specicfic initialization
   */
  virtual void initialize();
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
   * Get productCode specific encoding pattern for the first block of symbols
   * 
   * @param indexedPattern index of assiocated pattern
   */
  virtual QString getFirstBlockEncodePattern(int indexedPattern = 0) const;
    /**
   * Load all encoding patterns based on combo of system number (0-1) and check digit
   */
  virtual void fillWidthEncodingList(); 
  /**
   * encoding patterns for EAN-13 first block
   */
  QStringList m_parity13WidthEncoding;
};
};
#endif // EAN13ENGINE_H

