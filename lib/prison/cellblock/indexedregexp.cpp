/*
    Copyright (C) 2012  Ian gmail <ianhollander at gmail.com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/


#include "indexedregexp.h"

using namespace shared;

IndexedRegExp::IndexedRegExp(const QString &pattern) :
    m_sourceString(),
    m_RegExpPattern(pattern),
    m_capCounter(0),
    m_CapturedTexts()
{    
  // empty
};

bool IndexedRegExp::setSourceString(const QString & sourceString) const {
  m_sourceString = sourceString;
  m_CapturedTexts.clear();
  m_startingIndexArray.clear();
  m_capCounter = 0;
  
  QRegExp tRegExp(m_RegExpPattern);
  int nextPos = tRegExp.indexIn(sourceString, 0);
  m_CapturedTexts = tRegExp.capturedTexts();
  while ((nextPos = tRegExp.indexIn(sourceString, nextPos)) != NOT_FOUND) {
    m_startingIndexArray.push_back(nextPos);
  }
  if (m_CapturedTexts.isEmpty()) {
    m_capCounter = NOT_FOUND;
  }
}; 

int IndexedRegExp::getMatchingIndex() const {
  return m_startingIndexArray.at(m_capCounter);
}

QString IndexedRegExp::getMatchingString() const {
  return m_CapturedTexts.at(m_capCounter);
}

bool IndexedRegExp::operator== (const IndexedRegExp& lh) const {
  return ((lh.m_RegExpPattern == m_RegExpPattern) && 
  (lh.m_sourceString == m_sourceString));
} 

int IndexedRegExp::operator() () const{
  return getMatchingIndex();
}

void IndexedRegExp::nextMatch() const{
  if (m_capCounter < m_startingIndexArray.size()) {
    m_capCounter++; 
  }
}

bool IndexedRegExp::isValid() const {
  return (getMatchingIndex() > NOT_FOUND);
};
