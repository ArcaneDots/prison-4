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


#ifndef SYMBOLLIST_H
#define SYMBOLLIST_H

#include <QStringList>

#include "shareddefines.h"
#include "barcodesymbolset.h"
#include "symbol.h"

using namespace shared;

class SymbolList : public QList<Symbol> //: public QStringList
{

public:
    SymbolList();
    SymbolList(const Symbol& i);
    //explicit SymbolList(const QString& i);
    SymbolList(const QList<Symbol>& l);
    SymbolList(const BarcodeSymbolSet &barcodeSymbol, 
	       const QList<QString>& l);
//     SymbolList();
    SymbolList(const SymbolList& other);
    virtual ~SymbolList();
    virtual SymbolList& operator=(const SymbolList& other);
    virtual bool operator==(const SymbolList& other) const;    
//     /**
//      * empty user symbol list
//      */
//     bool isEmpty();
    /**
     * invalid barcode symbol information
     */
    bool isValid();
   void insert(int i, const QString &s);
    SymbolList parse(const QString& symbolString);
    /**
     * get list of values used to generate check symbols
     */
    shared::LookupIndexArray getIndexValues();
    
    /**
     * 
     */
    Symbol operator <<(const QString &stringSymbol);
//     /**.
//      * Returns the symbol at index position i in the list. 
//      * @note i must be a valid index position in the list (i.e., 0 <= i < size()).
//      * 
//      * This function is very fast (constant time).
//      * @sa value() and operator[]().
//      */
//     Symbol at(int index);
private:  
  QSharedDataPointer<BarcodeSymbolSet> m_barcodeSymbolSet;
//   /**
//    * list of QString pointer based on parser user input
//    */ 
//   std::list<QStringRef> m_userSymbolList;
  /**
   * user defined blocks of user parsed symbols  
   */
  std::map<QString, QStringRef> m_userBlocks;
};

#endif // SYMBOLLIST_H
