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


//   /**
//    * symbol list and one to one encoding
//    */
//   Symbol(const QStringList &encodableSymbols, const QStringList &symbolEncodings);
//   /**
//    * visble symbols -> expanded to list of encodable symbols => symbol encodings
//    */
//   Symbol(const QStringList &expanedSymbols, const QStringList &encodableSymbols,   
// 	 const QStringList &symbolEncodings);
//   /**
//    * encodable symbol => mulitple symbol encodabling (based on list id)
//    */
//   Symbol(const QStringList &expanedSymbols, const EncodingLists &symbolEncodings);
//   Symbol(const QChar* unicode, int size);
//   explicit Symbol(const QChar* unicode);
//   Symbol(QChar c);
//   Symbol(int size, QChar c);
//   Symbol(const QLatin1String& latin1);
//   Symbol(const QString& );
//   Symbol(const char* ch);
//   Symbol(const QByteArray& a);
//   Symbol(const QString::Null& );
//   Symbol(int size, Qt::Initialization );
//   Symbol();
#ifndef SYMBOL_H
#define SYMBOL_H

#include <map>
#include <vector>
#include <memory>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSharedDataPointer>

#include "barcodesymbolset.h"

//class SymbolList;

class Symbol //: public QString
{

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
  Symbol(const QSharedDataPointer<BarcodeSymbolSet> &BarcodeSymbolSet);
  
  /**
   * symbol set constructor
   */
  Symbol(const QString &stringSymbol,
	 const QSharedDataPointer<BarcodeSymbolSet> &BarcodeSymbolSet);
  /**
   * symbol set constructor
   */
  Symbol(const QString &stringSymbol, bool fromExtendedSet,
	 const QSharedDataPointer<BarcodeSymbolSet> &BarcodeSymbolSet);
  /**
   * destructor
   */
  virtual ~Symbol();
  /**
   * contains a valid symbol set
   */
  bool isValid() const;
  
  QSharedDataPointer<BarcodeSymbolSet> getFullsymbolSet() const;
  /**
   * Is the current symbol from a extended set
   * 
   * @returns true : symbol is from the barcode's extended set   
   */
  bool isExtendedSet() const;
  /**
   * get string version of the symbol
   * 
   * @returns string version of the stored symbol
   */
  QString getString() const;
  /**
   * Get symbol index
   */
  int getIndex() const;
  /**
   * get symbol encodabling
   */
  QStringList getEncoding() const;
  /**
   * get key based symbol encoding
   */
  QStringList getEncoding(const QString &encodingKey) const;
  /**
   * operator=
   * 
   * @param other 
   * @returns symbol copy
   */
  virtual Symbol& operator=(const Symbol& other);
  /**
   * operator==
   * 
   * @param other
   * @returns true; if objects are equal, false; otherwise
   */
  virtual bool operator==(const Symbol& other) const;
  
  //friend class SymbolList;
    
private:
  QSharedDataPointer<BarcodeSymbolSet> d;
//  SymbolData m_symbol
  /**
   * symbol index
   */
  int m_symbolIndex;
  /**
   * 
   */
  bool m_usesExtendedSet;
};

#endif // SYMBOL_H
