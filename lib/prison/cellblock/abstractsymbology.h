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

#include <QStringList>

#include "shareddefines.h"

namespace linearSymbology  
{
  
  class Symbol;
  class SymbolNode;
  
  class AbstractSymbologyPrivate;
  
class AbstractSymbology 
{
public: 
  
  friend class Symbol;
  /**
   * default constructor
   */
  AbstractSymbology();
  
  //AbstractSymbology(const AbstractSymbology &other);
  /**
   * destructor
   */
  ~AbstractSymbology();
  
  virtual QString getName() const; 
  
  virtual QString errorEncoding() const;
  
  virtual Symbol getSymbol(int index, 
			   QString symbolSet = shared::DEFAULT_SET);
  
  virtual Symbol getSymbol(QString string, 
			   QString symbolSet = shared::DEFAULT_SET);
   
  /**
   * Attempt to parse a single QString into a list of ordered Symbols
   */ 
  virtual QList<Symbol> parse(const QString& userInput) const;
  /**
   * Attempt to parse a list of QStrings into a list of ordered Symbols
   */ 
  virtual QList<Symbol> parse(const QStringList& userInput) const;

protected:       
  SymbolNode const * findNode(int index,
	    QString symbolSet = shared::DEFAULT_SET) const;
  SymbolNode const * findNode(const QString& userSymbol,
	    QString symbolSet = shared::DEFAULT_SET) const;

  const QScopedPointer<AbstractSymbologyPrivate> d_ptr;

  /**
   * Copy constructor
   */
  AbstractSymbology(AbstractSymbologyPrivate &d);
private:  
  Q_DECLARE_PRIVATE(AbstractSymbology);
};

QDebug operator<<(QDebug & dbg, const AbstractSymbology &s);

QList<Symbol> operator<<(const QList<Symbol>& symbols, 
			 const QString& userInput);

QList<Symbol> operator<<(const AbstractSymbology& symbols, 
				    const QString& userInput);

QList<Symbol> operator<<(const AbstractSymbology& symbols, 
				    const QStringList& userInput);
};

#endif // ABSTRACTSYMBOLOGY_H
