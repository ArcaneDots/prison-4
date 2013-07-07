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


#ifndef INDEXEDREGEXP_H
#define INDEXEDREGEXP_H

#include <vector>
#include <QtCore/QString>
#include <QtCore/QStringList>

/**
 * Default value for most class data members
 */
const int NOT_FOUND = -1;

namespace shared
{
  /**
   * Regular expression results storage object
   */
  class IndexedRegExp 
  {
  public:
    /**
     * Initialize the object with the regexp string
     * 
     * @param pattern string containing a regexp formated search pattern
     */
    IndexedRegExp(const QString &pattern);
    /**
     * Generate and store locations and text that is found in the source string
     * 
     * @param sourceString string to be searched
     * @return pattern was found in sourceString
     */
    bool setSourceString(const QString & sourceString) const;
    /**
     * Get the current index where the sourceString matches the pattern
     * 
     * @note "current" match is nth match based on a internal counter
     * @return current index
     */
    int getMatchingIndex() const;
    /**
     * Get the contents of the current match pattern in sourceString 
     * 
     * @note "current" match is nth match based on a internal counter
     * @return copy of matching string 
     */
    QString getMatchingString() const;
    inline QString regPattern() const { return m_RegExpPattern; };
    inline QString sourceString() const { return m_sourceString; };
    /**
     * operator()
     * 
     * @returns matching index
     */
    int operator() () const;
    /**
     * Advance object's "nextMatch" pointer
     */
    void nextMatch() const;
    /**
     * Get object's validity
     * 
     * A IndexedRegExp object is considered "valid" 
     * as long as there is current matching 
     * 
     * @returns true when matches remaining
     */
    bool isValid() const;
  private:
    mutable QString m_sourceString;
    mutable QStringList m_CapturedTexts;
    QString m_RegExpPattern;
    mutable int m_capCounter;  
    mutable std::vector<int> m_startingIndexArray;
  }; 
  
  /**
   * operator<
   * 
   * Define operator "<" so IndexedRegExp objects can be sort "correctly"
   * Move objects that empty or "exhusted" to the left and sort the remaining
   * object on the right.
   * 
   * @param lh left-hand IndexedRegExp object
   * @param rh right-hand IndexedRegExp object
   * @return is the left-hand IndexedRegExp object less than the on the right
   */
  bool operator<(const IndexedRegExp &lh, const IndexedRegExp &rh);
  
  
  /**
   * operator==
   * 
   * @param rh right-hand IndexedRegExp object argument
   * @returns true in case rh and current IndexedRegExp objects are "equal"
   */
  bool operator== (const IndexedRegExp &lh, const IndexedRegExp &rh); 
};
#endif // INDEXEDREGEXP_H
