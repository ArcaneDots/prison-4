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


#ifndef BARCODESYMBOLSET_H
#define BARCODESYMBOLSET_H

#include <map>
#include <list>
#include <set>
#include <functional>
#include <algorithm>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSharedDataPointer>

#include "indexedregexp.h"
#include "lookuptableentry.h"
class SymbolDatum;
typedef QList<SymbolDatum *> SymbolDataList;

using namespace barcodeEngine;

/**
 * collection of Indexed regular expression output objects
 */
typedef std::vector<IndexedRegExp> RegexpCollection;
  

typedef LookupTableEntry<QString> StringTableEntry;
typedef StringTableEntry VisibleSymbols;
typedef StringTableEntry EncodableSymbols;
typedef StringTableEntry EncodingMap;
typedef LookupTableEntry<LookupTableEntry<QString> > ExpansionMap;

typedef LookupTableEntry<SymbolDatum *> SymbolTableEntry;
typedef QList<const SymbolDatum *> SymbolDataPtrList;


/**
 * Convert a barcode's symbol lookup table into symbol data
 * 
 * uses QSharedData class to allow ref counted shared pointers
 */
class BarcodeSymbolSet : public QSharedData
{
private:
  /**
   * list of symbols that are visible
   *
   * points to same object when symbols are also encodable
   */ 
  SymbolTableEntry * m_visibleSymbolSet;
  /**
   * list of symbols that are encodable
   *
   * points to same object when symbols are also visible
   */ 
  SymbolTableEntry * m_encodableSymbolSet;
  /**
   * collection of specialized regexp objects
   * 
   * @sa IndexedRegExp
   */
  RegexpCollection m_regexpCollection;

public:
  /**
   * constructor
   * 
   * construct an empty barcode
   */
  BarcodeSymbolSet();
  /**
   * copy constructor
   * 
   * treats internal LookupTableEntry<SymbolDatum *> objects as shared pointers 
   */
  BarcodeSymbolSet(const BarcodeSymbolSet &other); 
  /**
   * Constructs a single set of both visible, directly encodable symbols
   * 
   * one set symbol(s) and one set of encoding(s) entries
   * 
   * @param symbolList list of string "symbols"
   * @param symbolEncodings corresponding list of symbol encoding
   */
  BarcodeSymbolSet(const EncodableSymbols &symbolList, 
		   const EncodingMap &symbolEncodings);
  /**
   * Constructs a set of visible symbols and a set that are directly encodable symbols
   * 
   * visble symbols -> expanded to list of encodable symbols => symbol encodings
   * 
   * @param extendedSymbols expanded set of symbols
   * @param encodableSymbols list of encodable "symbols"
   * @param expansionMap "extended" symbol -> encodableSymbol(s)
   * @param symbolEncodings corresponding list of symbol encoding
   */
  BarcodeSymbolSet(const EncodableSymbols &encodableSymbols,
                   const EncodingMap &symbolEncodings,
                   const VisibleSymbols &visibleSymbols,   
		   const ExpansionMap &expansionMap);
  /**
   * Has BarcodeSymbolSet been setup correctly
   */
  bool isValid() const;
  /**
   * Parse string into a list of one or more valid pointers to SymbolDatum(s)
   * 
   * @returns list of SymbolDatum pointers
   */
  SymbolDataPtrList parse (const QString &symbolString) const;
  /**
   * Get a visible symbol's look-up table index
   * 
   * @param symbol 1 or more character long "symbol"
   * @returns index of symbol or NOT_FOUND in case symbol does not exist
   */
  const SymbolDatum * getSymbol (const QString &symbol) const;  
  /**
   * Get a visible symbol's look-up table index
   * 
   * @param symbol 1 or more character long "symbol"
   * @returns index of symbol or NOT_FOUND in case symbol does not exist
   */
  const SymbolDatum * getExpandedSymbol (const QString &symbol) const;  
  /**
   * Get a visble symbol info from look-up table by index 
   * 
   * @return symbol string
   */
    const SymbolDatum* getSymbol( int symbolIndex ) const;
  /**
   * Get an encodable symbol info from look-up table by index 
   * 
   * @return symbol string
   */
  const SymbolDatum * getExpandedSymbol(int symbolIndex) const;
  /**
   * barcode uses an extended symbol set
   * 
   * @returns true for extended symbol set
   */
  bool isExtendedSet() const;
  /**
   * Equals matching symbol set 
   */
  bool operator== (const BarcodeSymbolSet &other) const;
protected:
  /**
   * Convert LookupTableEntry string pairs to String_Pair object
   */
  String_Pairs getSymbolEncoding(EncodableSymbols encodableSymbols, int index) const;
  /**
   * Get visible symbol info
   * 
   * @note when visible symbols are encodable, 
   * @note getVisibleSymbol(encodable_name) == getEncodableSymbol(visible_name)
   *
   * @param symbolString visible symbol 
   * @sa getEncodableSymbol
   */
   const SymbolDatum *getVisibleSymbol ( const QString& symbolString ) const;
  /**
   * Get encodable symbol info
   * 
   * @note when visible symbols are encodable, 
   * @note getVisibleSymbol(encodable_name) == getEncodableSymbol(visible_name)
   *
   * @param symbolString visible symbol 
   * @sa getVisibleSymbol
   */
   const SymbolDatum *getEncodableSymbol ( const QString& symbolString ) const;
  
private:
  /**
   * Generate encodable symbols from list of name and thier encoding
   * 
   * @param symbolList
   * @param symbolEncodings
   */
  SymbolTableEntry * generateEncodableSymbols(const EncodableSymbols &symbolList, 
                                              const EncodingMap &symbolEncodings);
  /**
   * Generate expanded visible symbols from symbol info and encoded symbols 
   * 
   * @param encodedableSymbolList
   * @param visibleSymbols
   * @param expansionMap
   */
  SymbolTableEntry * generateExpandedSymbols(const SymbolTableEntry * encodedableSymbolList,
                                             const StringTableEntry &visibleSymbols,                           
                                             const ExpansionMap &expansionMap);
  /**
   * Build expanded symbols list for a specified visible symbol
   *
   * @param encodedSymbolDatum
   * @param expansionMap
   * @param symbolString
   * @param symbolSet
   */                                              
  SymbolDataList * getVisibleSymbolExpansionList (
                                    const SymbolTableEntry * encodedSymbolDatum,
                                    const ExpansionMap& expansionMap,
                                    const QString& symbolString,
                                    const QString &symbolSet );
  
};

#endif // BARCODESYMBOLSET_H
