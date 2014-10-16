/*
 * Copyright (c) 2013 ian <email>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <QSharedData>
#include <QString>
#include <QStringList>



//#include "abstractsymbology.h"
//#include "abstractsymbology_p.h"

  
namespace linearSymbology{
  
  //class SymbolPrivate;
  
//   const int NOT_FOUND = -1;  
//   const QString DEFAULT_SET("__DEFAULT_SET__");
//    const QString EmptyString("");
//   
  class SymbolNode;
  class AbstractSymbology; 
  class AbstractSymbologyPrivate;
  
  

  /**
   * Object that represents a single distinct symbol
   */
  class Symbol 
  {
  public:
    friend class AbstractSymbologyPrivate;
    /**
     * default constructor - "null" symbol
     * 
     * isValid == false
     * hasValue = false
     */
    Symbol();
    /**
     * copy constructor - duplicate symbol
     * 
     * isValid == s.isValid
     * hasValue = s.hasValue
     */
    Symbol(const Symbol &s);
    /**
     * unset constructor
     */
    Symbol(SymbolNode * symbolData);
    
    ~Symbol();
    
     /**
      * Symbol has complete barcode symbology
      */
     bool isValid() const;
     /**
      * Symbol refers to a particular value
      */
     bool hasValue() const;
     
    QString encoding(const QChar& encodingType) const;
    QString encoding(const QString& encodingType) const;
    /**
     * Attempt to parse a single QString into a list of ordered Symbols
     */
    virtual QList<Symbol> parse(const QString& userInput) const;
    /**
     * Attempt to parse a list of QStrings into a list of ordered Symbols
     */
    virtual QList<Symbol> parse(const QStringList& userInput) const;
  
    
    const Symbol & operator=(const Symbol &other);
    const Symbol & operator=(int index);
    
    //  stl operators
   /**
    * Equals
    *
    * @note will return if matching not if not self-test  
    */
   bool operator==(const Symbol& right) const;
    bool operator!=(const Symbol &other) const;

    /**
    * Only compares index
    */
    bool operator==(int index) const;
 
  
  operator int() const { return getIndex(); }
  operator QString() const { return toString(); }
  
  /**
   * Get string version of symbol
   * 
   * @return empty string if not set
   */
  QString toString() const;  
  /**
   * Get symbol's value
   * 
   * @return integer value
   */
  int getIndex() const;  
  
  protected:            
    //friend Symbol(const SymbolNode * symbolData);
    
  private:  
    AbstractSymbology * m_symbology;
    SymbolNode * m_node;    
};

bool operator< (const Symbol& left, const Symbol& right);
bool operator> (const Symbol& left, const Symbol& right);

bool operator<= (const Symbol& left, const Symbol& right);
bool operator>= (const Symbol& left, const Symbol& right);

QDebug operator<<(QDebug& dbg, const Symbol& s);
}
#endif // SYMBOLS_H