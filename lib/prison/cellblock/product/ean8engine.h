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

/**
 * EAN-8 barcode generator
 */
class Ean8Engine : public ProductEngine 
{
  
public:
  /**
   * @brief default constructor
   */
  Ean8Engine();
  /**
   * String constructor
   *
   * @param productCode constant indicating the current product code
   **/
  Ean8Engine(const QString &userBarcode,
	     CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct);
  /**
   * Pass-thru constructor
   *
   * @param productCode constant indicating the current product code
   **/
  Ean8Engine(const barcodeEngine::SymbolList &userBarcode,
	     CodeEngine::ConstructCodes flags = CodeEngine::AutoProduct);
  /**
   * destructor
   */
  virtual ~Ean8Engine();  
  /**
   * Encoded barcode sections
   *
   * [block1][block2][extendedBlock(if defined)]
   */
    const QList<QStringList> encoded() const;
  /**
   * Get a list of symbol blocks formatted according to the barcode's specification
   */
  const QStringList formatedSymbols() const;
protected:
  /**
   * Encode complete number according to current barcode type
   *
   * @note EAN-8  format  [ ][0-4][5-8][ ]
   *
   * @param mainBlock first portion of the list of symbols
   */ 
  QList<QStringList> encodeMainBlock(const barcodeEngine::SymbolList& mainBlock) const;   
  
  
};
};
#endif // EAN8ENGINE_H

