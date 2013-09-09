/*
    Copyright (C) 2011  Ian gmail <ianhollander at gmail.com>

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

#ifndef EAN8ENGINE_H
#define EAN8ENGINE_H

#include "productengine.h"

namespace product 
{

class Ean8Engine : public ProductEngine 
{
public:
  /**
   * default constructor
   */
  Ean8Engine();
  /**
   * constructor
   *
   * @param productCode constant indicating the current product code
   **/
  Ean8Engine(const QString &userBarcode, 
	     CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct);
  /**
   * destructor
   */
  virtual ~Ean8Engine();  
  
  /**
   * Number System 
   * 
   * @note may be blank since EAN-8 dosn't use one
   */
  const QString numberSystem() const;
  /**
   * First block
   */
  const QStringList block1() const;
  /**
   * Second block
   * 
   * @note  may be blank since UPC-E doesn't have one
   */
  const QStringList block2() const;  
  /**
   * Encoded barcode sections
   * 
   * [block1][block2(optinal)][extendedBlock(optional)]
   */
  const QStringList encoded() const;
  /**
   * Get a list of symbol blocks formatted according to the barcode's specification
   */
  const QStringList formatedSymbols() const;
protected:
  /**
   * UpcAEngine Class specicfic initialization
   * 
   * Validate the check digit value and 
   * populate the value of each section
   * 
   * @sa populateSections
   * @sa validateCheckDigit
   */
  void localInitialize();
  /**
   * Populate the values associated with each section of the barcode
   */
  void populateSections();  
  /**
   * Encode complete number according to current barcode type
   *
   * UPC-E  format  [-][1-6][-][8-9|13]
   * 
   * @param symbolSrc full list of symbols
   * @param splitIndex index of the "end" of the first half
   */
  void encodeSymbols(const shared::SymbolList& symbolSrc);   
  /**
   * Encode complete number according to current barcode type
   * 
   * Overload UPC-A version because UPC-E only has 1 block of digits
   *  
   * @note UPC-E  format  [-][1-6][-][8-9|13]
   * 
   * @param mainBlock first portion of the list of symbols 
   */ 
  QStringList encodeMainBlock(const shared::SymbolList& mainBlock) const;   
  
protected:
  
  
};
};
#endif // EAN8ENGINE_H

