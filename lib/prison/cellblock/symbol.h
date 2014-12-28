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

#include <QString>
#include <QStringList>
#include <QDebug>

namespace linearSymbology{
  
  class SymbolNode;
  class AbstractSymbology; 
  class AbstractSymbologyPrivate;
  
  class SymbolPrivate;
  /**
   * Object that represents a single distinct symbol
   * 
   * @note Symbol object should act as a wrapper
   * for a Symbology and a particualr SymbolNode it generated
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
    Symbol(const SymbolNode * symbolData);
    
    Symbol(AbstractSymbology * symbology);
    
    ~Symbol();
    
     /**
      * Symbol has complete barcode symbology
      */
     bool isValid() const;
     /**
      * Symbol refers to a particular value
      */
     bool hasValue() const;
     
     QString encoding() const;
    QString encoding(const QChar& encodingType) const;
    QString encoding(const QString& encodingType) const;
    /**
     * Attempt to parse a single Symbol into a list of ordered Symbols
     */
    virtual QList<Symbol> parse(const QString& userInput) const;
    /**
     * Attempt to parse a list of Symbol into a list of ordered Symbols
     */
    virtual QList<Symbol> parse(const QStringList& userInput) const;
  
    bool isSameSymbology(const Symbol &s);
    
    /**
     * Set current Symbol to point to other symbol "value" 
     */
    const Symbol & operator=(const Symbol &other);
    /**
     * Point this object to a different symbol in to the same set based on index
     * 
     * Symbol will become undefine if index value is outside range of symbology 
     */
    const Symbol & operator=(int index); 
    /**
     * Compares symbol's index only
     */
    bool operator==(int index) const;
    /**
     * Compares symbol's full value 
     */
    bool operator==(const Symbol& other) const;
  
    /**
     * integer value of symbol
     */
    operator int() const { return getIndex(); }
    /**
     *  string "value" of symbol 
     */
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
     const QScopedPointer<SymbolPrivate> d_ptr;
  
     /**
      * Copy constructor
      */
     Symbol(SymbolPrivate &d);
     
  private:
    Q_DECLARE_PRIVATE(Symbol);
  
};

//  stl operators
/**
 * Not Equal
 *
 * @returns true if contents of symbols are not equal  
 */
bool operator!= (const Symbol& left, const Symbol& right);
/**
 * Less than
 * 
 * @returns true if left-hand symbol has a smaller index than right-hand symbol
 * @note required for Set<>
 */
bool operator< (const Symbol& left, const Symbol& right);

QString toString (const QList<Symbol>& symbolList);
QStringList toStringList (const QList<Symbol>& symbolList);

QDebug operator<< (QDebug& dbg, const Symbol& s);

QList<Symbol> operator+=(const QList<Symbol> & left, const Symbol& right);

QList<Symbol> operator<<(const QList<Symbol>& symbols, 
			 const QChar& userInput);

QList<Symbol> operator<<(const QList<Symbol>& symbols, 
			 const char* userInput);

QList<Symbol> operator<<(const QList<Symbol>& symbols, 
			 const QString& userInput);

QList<Symbol> operator<<(const QList<Symbol> & left,
			 const Symbol& right);

QStringList toRawEncoding (const QList<Symbol>& symbolList);

}
#endif // SYMBOLS_H