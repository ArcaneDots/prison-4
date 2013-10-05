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
#include <QStringList>

#include "shareddefines.h"

namespace shared  
{
  
const int NOT_FOUND = -1;  
  

class Symbol
{
public:
  const static QString ERROR_ENCODING;
  const static int NOT_FOUND;
  /**
   * default constructor
   */
  Symbol();
  /**
   * Copy constructor
   */
  Symbol(const Symbol &other);
  /**
   * Full constructor
   */
  Symbol(const QString &string);
  Symbol(int index);
  /**
   * destructor
   */
  ~Symbol();
  
  QString encoding(const QChar& encodingType) const;
  QString encoding(const QString& encodingType) const;
  
  const Symbol & operator=(const Symbol &other);
  const Symbol & operator=(int index);
  
  /**
   * Test whereter the current Symbol object contains the same Barcode information and current value
   *
   * @note will return if matching not if not self-test  
   */
  bool operator==(const Symbol &other) const;
  bool operator==(int index) const;
  bool operator!=(int index) const { return !(operator==(index)); }
  
  QList<Symbol>  operator+=(const Symbol& other);
  
  /**
   * Attempt to parse a single QString into a list of ordered Symbols
   */ 
  QList<Symbol> parse(const QString& userInput) const;
  /**
   * Attempt to parse a list of QStrings into a list of ordered Symbols
   */ 
  QList<Symbol> parse(const QStringList& userInput) const;
  
  /**
   * Symbol has complete barcode symbology
   */
  bool isValid() const;
  /**
   * Symbol refers to a particular value
   */
  bool hasValue() const;
  
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
  /**
   * Is the passed string valid according to the shared barcode information
   * 
   * @note case sensitive 
   * 
   * @rsturns bool
   */
  bool isValidString(const QString& symbolString) const;
  /**
   * Convert string version to int value
   * 
   * @note case sensitive 
   *
   * @returns shared::NOT_FOUND if string not valid 
   */
  const QString convertIndexToString(const int index) const;
  /**
   * Convert int value to printable string 
   *
   * @returns empty string if index out of range
   */  
  int convertStringToIndex(const QString& symbolString) const;  
  
private:
  /**
   * Set symbol's value by index
   */
  void setSymbolIndex(int index);
  /**
   * Set symbol's value by string version
   * 
   * @note case sensitive 
   */
  void setSymbolIndex(const QString & string);
  
  class Private;
  Symbol(Symbol::Private * sharedData);
  QSharedDataPointer<Private> d;
};

typedef QList<Symbol> SymbolList;

QDebug operator<<(QDebug & dbg, const Symbol &s);
QList<Symbol> operator<<(const QList<Symbol>& symbols, const QString& userInput);
QStringList toStringList(const QList<Symbol> & symbolStrings);
QString toStrings(const QList<Symbol> & symbolStrings);
std::vector<int> toIntVector(const QList<Symbol> & symbolStrings);

class PatternEncoder
{
  public:
    PatternEncoder (const QString & pattern); 
    QString operator() (Symbol s);
  private:
    QString m_pattern;
    int m_index;
};

// shared::LookupIndexArray convertSymbolsToIndexes(
//   const QList<Symbol>& symbolList);
QStringList encodeSymbolParity(const QList<Symbol> & symbols, 
			       const QString& parityPattern);

// /*
// /**
//   * Calculate check sum digit for a array of symbol look-up indexes
//   * 
//   * Uses the most common method of calculating a check digit; 
//   * = Sum(index values) % modulus value
//   * @note used by Code 39, MSI (2 Mod 10)
//   * 
//   * @param symbolArray array of symbol's look-up index
//   * @return number of valid check digit
//   */
// int calculateCheckDigit(int modulusValue, const QList<Symbol>& symbolArray);  
// /**
//  * Get the remainder of the check sum divided by the modulus value
//  * 
//  * @param checksum total of all the individual symbol's index and positional value
//  * @return = (checksum % modulus value
//  */
// int SimpleRemainderCheckDigit(int checksumModulus, int checksum);
// /**
//  * Calculate "Odd/Even" check sum pattern for a array of symbol look-up indexes
//  * 
//  * Move left -> right, the left-most index treated as "odd".
//  * Uses the  method of calculating a check sum; 
//  * @sa = NextMultipleCheckDigit(
//  * @sa sum of all odd indexes * oddMultipler + sum of all even indexes * evenMultipler)
//  * 
//  * @param symbolArray array of symbol's look-up index
//  * @param oddMultipler value multipled with indexes in the "Odd" position
//  * @param evenMultipler value multiplied with indexes in the "Odd" position
//  * @param reverse move right -> left, the right-most index treated as "odd" 
//  * @return value of valid check digit
//  */
// int CommonChecksumOddEven(int checksumModulus,
// 			  const SymbolList& symbols, 
// 			  int oddMultipler,
// 			  int evenMultipler,
// 			  bool reverse = false);
// /**
//  * Calculate "Linear" check sum digit for a array of symbol look-up indexes
//  * 
//  * Uses the  method of calculating a check digit; 
//  * = modulus - Sum(index * (counter < modulus)) % modulus value
//  * 
//  * @param symbolArray array of symbol's look-up index
//  * @param reverse
//  * @return number of valid check digit
//  */
// int CommonChecksumLinear(int checksumModulus,
// 			 const shared::SymbolList& symbols,
// 			 bool reverse = false);
// /**
//  * Get the value when added to the check sum makes it a multiple of the "Modulus" value
//  * 
//  * @param checksum total of all the individual symbol's index and positional value 
//  * @return modulus value - (checksum & modulus value)
//  */
// int NextMultipleCheckDigit(int modulusValue, int checksum);
// */


// std::ostream& operator<<(std::ostream& stream, const A& val)
// {
//   stream << val.toString(); // would need to make function const, of course
//   
//   return stream;
// }

};

#endif // SYMBOLS_H
