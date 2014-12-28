/*
 * Copyright (c) 2013 ian hollander @ gmail dot com
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
#include <algorithm>
#include <iterator>
#include <numeric>

#include "productsymbology_p.h"
#include "productsymbology.h"
namespace product
{

ProductSymbology::ProductSymbology() : 
  AbstractSymbology(*new ProductSymbologyPrivate)
{ /* Empty */ }

ProductSymbology::ProductSymbology(const ProductSymbologyPrivate& other) : 
  AbstractSymbology(*new ProductSymbologyPrivate(other))
{ /* Empty */ }


ProductSymbology::~ProductSymbology()
{ /* Empty */ }

QString ProductSymbology::getName() const 
{
  Q_D(const ProductSymbology);
  return d->m_name; 
}

QString ProductSymbology::errorEncoding() const { 
  Q_D(const ProductSymbology);
  return d->m_errorEncoding;
}

Symbol ProductSymbology::getSymbol(QString string, QString symbolSet)
{
    return linearSymbology::AbstractSymbology::getSymbol(string, symbolSet);
}

Symbol ProductSymbology::getSymbol(int index, QString symbolSet)
{
    return linearSymbology::AbstractSymbology::getSymbol(index, symbolSet);
}

QList< Symbol > ProductSymbology::parse(const QString& userInput) const
{
    return linearSymbology::AbstractSymbology::parse(userInput);
}

QList< Symbol > ProductSymbology::parse(const QStringList& userInput) const
{
    return linearSymbology::AbstractSymbology::parse(userInput);
}

QDebug operator<<(QDebug& dbg, const ProductSymbology& s)
{ 
  dbg << s.getName(); 
  return dbg;
}

QList< Symbol > operator<<(const QList<Symbol>& symbols, const QString& userInput)
{
  return linearSymbology::operator<<(symbols, userInput);
}

QList< Symbol > operator<<(const ProductSymbology& symbols, const QString& userInput)
{
  return operator<<(
    dynamic_cast<const AbstractSymbology &>(symbols), userInput);
}

QList<Symbol> operator<<(const ProductSymbology& symbols, 
				  const QStringList& userInput)
{
  return operator<<(
    dynamic_cast<const AbstractSymbology &>(symbols), userInput);
}

}