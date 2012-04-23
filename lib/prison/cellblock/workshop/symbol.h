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

#ifndef SYMBOL_H
#define SYMBOL_H

#include <map>
#include <vector>
#include <memory>
#include <iostream>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QExplicitlySharedDataPointer>
#include <QtCore/QSharedDataPointer>

#include "symboldatum.h"
#include "barcodesymbolset.h"
typedef QExplicitlySharedDataPointer<BarcodeSymbolSet> BarcodeSetInfo;/*
class Symbol;
typedef QSharedDataPointer<Symbol> SharedSymbol;
typedef QList<SharedSymbol> SharedSymbolList;*/
/**
 * Collection of symbol index values
 */
typedef std::vector<int> LookupIndexArray;

class Symbol : public QSharedData 
{
  friend bool operator==(const Symbol& lhs, const Symbol& rhs);
  friend bool operator< (const Symbol& lhs, const Symbol& rhs);
protected:
  enum DataState {
    UNKOWN = -1,
    DATUM,
    STRING,
    BARCODESET    
  };
  
private:
  SymbolDatum * m_symbolDatum; 
  QString m_parsableSymbol;
  BarcodeSetInfo m_barcodeSetInfo;
  DataState m_dataState;
public:  
  /**
   * constructor
   */
  Symbol();
  /**
   * copy constructor
   */
  Symbol(const Symbol& other);    
  /**
   * symbol set constructor
   */
  Symbol(const BarcodeSetInfo &BarcodeSymbolSet);  
  /**
   * symbol set constructor
   * 
   * #make protected
   */
  Symbol(const SymbolDatum * symbolInfo );
  /**
   * symbol set constructor
   */
  Symbol(const BarcodeSetInfo &BarcodeSymbolSet, int index, bool extended = false);  
  /**
   * symbol set constructor
   */
  Symbol(const BarcodeSetInfo &BarcodeSymbolSet, const QString &string, bool extended = false);
  /**
   * Parsable string
   */
  Symbol(const QString &string);
  /**
   * destructor
   */
  virtual ~Symbol();
  // basic properties
  /**
   * points to a valid symbolset object 
   */
  bool isValid() const;
  /**
   * symbol object is not set to a symbol (text or object)
   */
  bool hasValue() const;
  // extended properties
  /**
   * get string version of the symbol
   * 
   * @returns string version of the stored symbol
   */
  QString toString() const;
  /**
   * Set BarcodeSymbolSet
   */
  void setBarcodeSymbolSet( const BarcodeSymbolSet* symbolSet ); 
  /**
   * Set SymbolDatum
   */
  void setSymbolDatum(const SymbolDatum * symboldatum);  
  /**
   * Get BarcodeSymbolSet
   */
   const BarcodeSymbolSet* getBarcodeSymbolSet() const; 
  /**
   * Get SymbolDatum
   */
  const SymbolDatum * getSymbolDatum() const;  
  /**
   * Get symbol index
   */
  int getIndex() const;
  /**
   * Is the current symbol from a extended set
   * 
   * @returns true : symbol is from the barcode's extended set   
   */
  bool isExtendedSet() const;
  /**
   * get key based symbol encoding
   */
  QString getEncoding(const QString &encodingKey = defaultSet) const; 
  /**
   * get all symbol encodings
   */
  String_Pairs getEncodings() const;
  /**
   * Get all symbol indexes for check sum calculation 
   */
  barcodeEngine::LookupIndexArray getIndexes() const;
  /**
   * get pointer to symbol set
   */
  const BarcodeSetInfo getFullsymbolSet() const;
  /**
   * both symbol objects use the same symbol set
   */
  bool isSameSymbolSet(const Symbol &s) const;
  
  // operator(s)
  
  // Assignment operator
  /**
   * operator=
   * 
   * @param other 
   * @returns symbol copy
   */
  virtual Symbol& operator=(const Symbol& other);
  
  // can not "increament"; no x++ or ++x
  // no inherent order : no operator<
  template<class T>
  QList<Symbol> operator+ (const T &t) const
  {
    QList<Symbol> s(*this);
    s.append(s.parse<T>(t));
    return s;
  }
  /**
   * Clear symbol's value 
   */
  void clear();
  /**
   * Parse input into Symbol
   * 
   * @param symbolInfo potential symbol object 
   * @returns list of parsed symbols
   */
  template<class T>
  QList<Symbol> parse(const T& symbolInfo) const 
  {
    qDebug("Symbol parse(T)");
    return toSymbol(symbolInfo);
  }
protected:
  
  //Symbol * clone() const;
  //bool setSymbol(const SymbolDatum * symboldatum);
  /**
   * by index
   */
  QList<Symbol> toSymbol( int symbolInfo) const;
  /**
   * string
   */
  QList<Symbol> toSymbol( const QString& symbolInfo ) const;
  //const QList<Symbol> toSymbol(const SharedSymbol& symbolInfo ) const;
  //const QList<Symbol> toSymbol(const QList<Symbol>& symbolInfo ) const { return symbolInfo; }
  QList<Symbol> toSymbol( const QStringList& symbolInfo) const;  
  QList<Symbol> toSymbol (const Symbol& symbolInfo) const;
  
  QList<Symbol> toSymbol( const char& symbolInfo, int size ) const;
};

// non-member functions

bool operator==(const Symbol& lhs, const Symbol& rhs);
inline bool operator!=(const Symbol& lhs, const Symbol& rhs)
{return !operator==(lhs,rhs);} 
bool operator< (const Symbol& lhs, const Symbol& rhs);
inline bool operator> (const Symbol& lhs, const Symbol& rhs)
{return  operator< (rhs,lhs);} 
inline bool operator<=(const Symbol& lhs, const Symbol& rhs)
{return !operator> (lhs,rhs);} 
inline bool operator>=(const Symbol& lhs, const Symbol& rhs)
{return !operator< (lhs,rhs);}
/**
 * templated operator+()
 */

QList<Symbol> operator-(const QList<Symbol> &symbols, const Symbol &s);

QStringList& operator<<( QStringList &stringList, const Symbol &s);

/**
 * Symbol -> ostream
 */
std::ostream& operator<<(std::ostream& os, const Symbol &s);
/**
 * Symbol -> ostream
 */
QDebug& operator<<(QDebug &debugString, const Symbol &s);

#endif // SYMBOL_H
