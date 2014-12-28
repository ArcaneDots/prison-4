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

#ifndef PRODUCT_SYMBOLOGY_H
#define PRODUCT_SYMBOLOGY_H

#include "../abstractsymbology.h"

using namespace linearSymbology;
namespace product  
{
  
  class ProductSymbologyPrivate;

class ProductSymbology : public AbstractSymbology
{
public:
  /**
   * default constructor
   */
  ProductSymbology();
  /**
   * Copy constructor
   */
  ProductSymbology(const product::ProductSymbologyPrivate& other);
  /**
   * destructor
   */
  ~ProductSymbology();
    
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
  ProductSymbology(product::ProductSymbologyPrivate& d);
  
private:
   Q_DECLARE_PRIVATE(ProductSymbology);
};

QDebug operator<<(QDebug & dbg, const ProductSymbology &s);

QList<Symbol> operator<<(const QList<Symbol>& symbols, 
			 const QString& userInput);

QList<Symbol> operator<<(const ProductSymbology& symbols, 
				    const QString& userInput);

QList<Symbol> operator<<(const ProductSymbology& symbols, 
				    const QStringList& userInput);

}

#endif // PRODUCT_SYMBOLOGY_H
