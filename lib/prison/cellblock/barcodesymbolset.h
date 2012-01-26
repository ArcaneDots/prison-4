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

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSharedDataPointer>

#include "shareddefines.h"

// class Symbol;
// class SymbolList;
using namespace shared;

typedef std::map<QString, QStringList> MultiEncodingMap;
typedef std::map<QString, QStringList> ExpansionMap; //Symbols;


struct SymbolData 
{
public:  
  SymbolData (int symbolIndex = NOT_FOUND, bool extendedSet = false) :
    m_index(symbolIndex),
    m_extendedSet(extendedSet)
    {
      // empty
    }
  int m_index;
  bool m_extendedSet;  
};

class BarcodeSymbolSet : public QSharedData
{
public:
  BarcodeSymbolSet();
  BarcodeSymbolSet(const BarcodeSymbolSet &other); 
  /**
   * symbol list and one to one encoding
   * 
   * @param symbolList list of string "symbols"
   * @param symbolEncodings corresponding list of symbol encoding
   */
  BarcodeSymbolSet(const QStringList &symbolList, 
		   const QStringList &symbolEncodings);
  /**
   * visble symbols -> expanded to list of encodable symbols => symbol encodings
   * 
   * @param extendedSymbols expanded set of symbols
   * @param encodableSymbols list of encodable "symbols"
   * @param expansionMap "extended" symbol -> encodableSymbol(s)
   * @param symbolEncodings corresponding list of symbol encoding
   */
  BarcodeSymbolSet(const QStringList &expandedSet, 
		   const QStringList &encodableSet,   
		   const ExpansionMap &expansionMap,
		   const QStringList &symbolEncodings);
  /**
   * encodable symbol => mulitple symbol encodabling (based on list id)
   */
  BarcodeSymbolSet(const QStringList &symbolList, 
		   const MultiEncodingMap &multiListSymbolEncodings);
  
  bool isValid();
//   /**
//    */
//   SymbolList parse(const QString &inputString);
  QString getSymbol(const SymbolData &symbolData) const;
  /**
   * barcode uses an extended symbol set
   * 
   * @returns true for extended symbol set
   */
  bool isExtendedSet() const;
  /**
   * get index list
   * 
   * @param currentSymbol
   */
  LookupIndexArray getExtendedIndex(const QString &currentSymbol) const; 
  /**
   * Get a symbol's look-up table index
   * 
   * @param symbol 1 or more character long "symbol"
   * @returns index of symbol or NOT_FOUND in case symbol does not exist
   */
  int getSymbolIndex(const QString &symbol) const;  
//   /**
//    * Get expanded symbol's look-up table indexes
//    * 
//    * @param symbol 1 or more character long "symbol"
//    * @returns index of symbol or NOT_FOUND in case symbol does not exist
//    */
//   int getSymbolIndex(const QString &symbol) const;  
  /**
   * Get the symbol at a particular look-up table index
   * @return symbol string
   */
  const QString getSymbolAtIndex(const SymbolData &symbolData) const;
//   /**
//    * Get the symbol at a particular look-up table index
//    * @return symbol string
//    */
//   const Symbol getSymbol(const QString &symbol) const;
//   /**
//    * Get the symbol at a particular look-up table index
//    * @return symbol string
//    */
//   const SymbolList getSymbolList(const QStringList &symbolList) const;
  /**
   * Encode an individual symbol 
   * 
   * @note overload in case the symbol encoding deviates from most linear barcodes 
   * 
   * @param symbol symbol to be encoded
   * @return actual encoding
   */
  QStringList getSymbolEncoding(const SymbolData &symbolData) const;
  /**
   * Encode an individual symbol 
   * 
   * @note overload in case the symbol encoding deviates from most linear barcodes 
   * 
   * @param symbol symbol to be encoded
   * @param encodingSetId id of encoding set
   * @return actual encoding
   */
  QStringList getSymbolEncoding(const SymbolData& symbolData, 
				const QString& encodingSetId) const;
protected:
  
  bool isSymbolVisible(const QString &symbolString) const;
  bool isSymbolEncodable(const QString &symbolString) const;
  

//   SymbolList parser(const QString& symbolString);
  // expanded symbol set
  /**
   * possible list of expanded symbol set visible to the user  
   */
  QStringList * m_visibleSymbolSet;
  /**
   * 
   */
  std::map<QString, QStringList> m_symbolExpansion;
  
  // actual symbols in final barcode
  /**
   */
  QStringList * m_encodableSymbolSet;  
  /**
   * list of encodable symbols 
   */
  QStringList m_symbolsEncodings;
  /**
   * list of symbol encodings
   */
  QStringList * m_symbolEncodings;
  
  std::map<QString, QStringList> m_multiCharactorSymbolReps;
  //m_stringExpandedSet
  
  MultiEncodingMap m_encodingLists;
  /**
   * 
   */
  ExpansionMap m_expansionMap;// ExpansionMap
//   RegexpCollection m_regexpCollection;
};



// public:
//     BarcodeSymbolSet();
//     BarcodeSymbolSet(const BarcodeSymbolSet& other);
//     virtual ~BarcodeSymbolSet();
//     virtual BarcodeSymbolSet& operator=(const BarcodeSymbolSet& other);
//     virtual bool operator==(const BarcodeSymbolSet& other) const;
//};

#endif // BARCODESYMBOLSET_H
