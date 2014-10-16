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

#ifndef ABSTRACTSYMBOLOGY_H
#define ABSTRACTSYMBOLOGY_H

#include <QSharedData>
#include <QStringList>

#include "shareddefines.h"
//#include "symbolnode.h"
//#include "symbol_p.h"

namespace linearSymbology  
{
//  namespace hidden{
//     const QString DEFAULT_SET("__DEFAULT_SET__");
//     const QString EmptyString("");
//   class SymbolNode;
//     
// //     typedef QMap<QString, QStringList> One2Many;
//      typedef QMap<QString, QString> SymbolEncoding;
// //     typedef QMap<QString, QStringList> NamedSet;
// //     
// //     typedef QSharedDataPointer<SymbolNode> SharedSymbolsData;
//      typedef QList<SymbolNode> SymbolDataList;
// //    typedef QMap<QString, SymbolDataList> SymbolDataListMap;
//      typedef QSet<SymbolNode> ExpandedSymbolsSet;
//      typedef QList<SymbolNode> ExpandedSymbolsList;
//  };
  
  class Symbol;
  class SymbolNode;
  class SymbolPrivate;
  
  class AbstractSymbologyPrivate;
  
class AbstractSymbology 
{
public: 
  
  friend class Symbol;
  /**
   * default constructor
   */
  AbstractSymbology();
  /**
   * Copy constructor
   */
  AbstractSymbology(const AbstractSymbology &other);
//   /**
//    * Full constructor
//    */
//   AbstractSymbology(const QString &string);
//   AbstractSymbology(int index);
  /**
   * destructor
   */
  ~AbstractSymbology();
  
  virtual QString getName() const {return QString("Abstract");}
  

  /**
   * Attempt to parse a single QString into a list of ordered Symbols
   */ 
  virtual QList<Symbol> parse(const QString& userInput) const;
  /**
   * Attempt to parse a list of QStrings into a list of ordered Symbols
   */ 
  virtual QList<Symbol> parse(const QStringList& userInput) const;
  
  virtual Symbol getSymbol(int index, 
			   QString symbolSet = shared::DEFAULT_SET);
  
  virtual Symbol getSymbol(QString string, 
			   QString symbolSet = shared::DEFAULT_SET);
  
  
  virtual QString errorEncoding();
  
  SymbolNode * findNode(int index,
	    QString symbolSet = shared::DEFAULT_SET) const;
  SymbolNode * findNode(const QString& userSymbol,
	    QString symbolSet = shared::DEFAULT_SET) const;

protected: 

 
    
  const QScopedPointer<AbstractSymbologyPrivate> d_ptr;
private:  
  Q_DECLARE_PRIVATE(AbstractSymbology);
};

//typedef QList<AbstractSymbology> SymbolList;


QDebug operator<<(QDebug & dbg, const AbstractSymbology &s);
// QList<AbstractSymbology> operator<<(const QList<AbstractSymbology>& symbols, const QString& userInput);
// QStringList toStringList(const QList<AbstractSymbology> & symbolStrings);
// QString toStrings(const QList<AbstractSymbology> & symbolStrings);
// std::vector<int> toIntVector(const QList<AbstractSymbology> & symbolStrings);

class PatternEncoder
{
  public:
    PatternEncoder (const QString & pattern); 
    QString operator() (const Symbol& s);
  private:
    QString m_pattern;
    int m_index;
};

// shared::LookupIndexArray convertSymbolsToIndexes(
//   const QList<Symbol>& symbolList);

// QStringList encodeSymbolParity(const QList<Symbol> & symbols, 
// 			       const QString& parityPattern);

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

#endif // ABSTRACTSYMBOLOGY_H
