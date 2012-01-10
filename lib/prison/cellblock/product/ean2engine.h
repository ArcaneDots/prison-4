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
