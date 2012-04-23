/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Ian gmail <ianhollander@gmail.com>

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


#include "lookuptableentry.h"
//#include <map>

#ifndef SYMBOLDATUM_H
#define SYMBOLDATUM_H

class SymbolDatum;
typedef QList<SymbolDatum *> ExpansionSymbolList;
class BarcodeSymbolSet;
 
class SymbolDatum
{  
private:
  /**
   * Barcode set that contains this object
   */
  BarcodeSymbolSet * m_srcSymbolSet;
  /**
   * lookup table index 
   */
  int m_tableIndex;
   /**
   * String represention
   */
  QString m_stringRep;  
  // encodable
  /**
   * all possible encodings
   * 
   * @note considered only "visible" when empty
   */
  String_Pairs m_encodingPair;
  /**
   * list of encodable symbols represnted this symbol
   * 
   * @note can not be encodable and be expanded
   */
  ExpansionSymbolList  m_expansionSymbols;
  
private:
  /// Default constructor
  SymbolDatum();
public:
  friend QDebug operator<<(QDebug debugString, const SymbolDatum &sd);
  friend QDebug operator<<(QDebug debugString, const SymbolDatum *sd);
  /**
   * copy constructor
   */
  SymbolDatum ( const SymbolDatum& other );
  
  /**
   * directly encodable symbols
   * 
   * @param srcSymbolSet
   * @param stringSymbol
   * @param encoding
   * @param symbolSetName
   */
   SymbolDatum (const BarcodeSymbolSet * parent,
                 const QString &stringSymbol, 
                 const int index, 
                 const String_Pairs &encoding);
  /**
   * expansion symbols 
   * 
   * when defined, they hide directly encodable symbols
   * 
   * @param srcSymbolSet
   * @param stringSymbol
   * @param encoding
   * @param symbolSetName
   */
   SymbolDatum (const BarcodeSymbolSet * parent,
               const QString &stringSymbol, 
               const int index, 
               const String_Pairs &encoding,
               const ExpansionSymbolList encodableSymbols);
  /**
   * visible symbols
   * 
   * string 
   * member_of (defaultSet)
   * expansionSymbol(s) expansionMap
   * parse pattern(s) MultiStringList
   */
  /**
   * expanded symbols
   * 
   * string
   * member_of (defaultSet)
   * encoding(s) string_pair
   * parse pattern(s) MultiStringList
   */
  
  /**
   * Destructor
   */
  virtual ~SymbolDatum();
  /**
   * operator =
   */
  virtual SymbolDatum& operator= ( const SymbolDatum& other );
  /**
   * operator==
   */
  virtual bool operator== ( const SymbolDatum& other ) const;
    
  int operator()() const
  {
    return m_tableIndex;
  }
  bool isValid() const;
  const BarcodeSymbolSet * getBarcodeSet() const { return m_srcSymbolSet; }
  /**
   * Get symbol index
   */
  int getIndex() const { return m_tableIndex; }
  QString getString() const {return m_stringRep; }
  String_Pairs getEncodings() const { return m_encodingPair; }
  ExpansionSymbolList getExpandedSymbols() const { return m_expansionSymbols; }  
  bool operator== (const QString &other) const 
  { return getString() == other; };
};

/**
 * SymbolDatum -> QDebug stream
 */
QDebug operator<<(QDebug debugString, const SymbolDatum &sd);
QDebug operator<<(QDebug debugString, const SymbolDatum *sd);

#endif // SYMBOLDATUM_H

