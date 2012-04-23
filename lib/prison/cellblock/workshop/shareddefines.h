/*
    <shared defines> 
    Copyright (C) 2011  Ian gmail <ianhollander@gmail.com>

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

#ifndef SHARED_DEFINES_H
#define SHARED_DEFINES_H

#include <deque>
#include <QtCore/QtDebug>

/**
 * Default value for most class data members
 */
const int NOT_FOUND = -1;

namespace std
{
  /**
   * copy_n: copy n objects from source range
   *
   * valid stl fuction in algorithm for C++11. This implemention is for portability sake. 
   * @note add ifdef to use official version if available
   */
  template<class InIt, class OutIt> 
  OutIt copy_n(InIt src, OutIt dest, size_t n) 
  { 
    while (n--)
      *dest++ = *src++; 
    return dest; 
  }
}; 

namespace barcodeEngine
{
  /**
   * index of important barcode positions
   * 
   * for most barcodes use just use  "barstart" - "barend" to indicate the start and end 
   * of the encoded informstion.
   * sections are used by product codes block1/2 and extended block  in order 
   * to format thier human-readable inoformation. 
   */
  enum BAR_POSITIONS {
    /** first "bar" after official "quiet zone" */
    BAR_START = 0,
    /** start of primary */
    MAIN_START,
    MAIN_BLOCK_1_START,
    MAIN_BLOCK_1_END,
    MAIN_BLOCK_2_START,
    MAIN_BLOCK_2_END,
    MAIN_BLOCK_3_START,
    MAIN_BLOCK_3_END,
    MAIN_END,
    /** end of primary */
    EXTENDED_BLOCK_START,
    EXTENDED_BLOCK_END,
    /** end of barcode */
    BAR_END
  };
  // Typedefs
  /**
   * Collection of symbol index values
   */
  typedef std::vector<int> LookupIndexArray;
  
//   /**
//    * collection of Indexed regular expression output objects
//    */
//   typedef std::vector<IndexedRegExp> RegexpCollection;
//   
  /**
   * Mapped collection of barcode location offsets
   * 
   * Used to algin text(boxes) with the equivalent bar encoding 
   */
  typedef QMap<int, int> BarPositionsMap;
  
  /**
   * Calculate checksum 
   * 
   * @note used by EAN-8/13 and 2 of 5
   * @note "even" includes '0' 
   * 
   * @param oddMultipler 
   * @param evenMultipler
   */
    template <class T> class EvenOddChecksum :
       public std::binary_function<T, T, T> 
    {
    public:
      EvenOddChecksum (T oddMultipler,
		       T evenMultipler) :
		       m_oddMultipler(oddMultipler),
		       m_evenMultipler(evenMultipler),
		       m_parity(1) // start "odd"
      {	  
	qDebug("evenOddChecksum constructor() : start");
	Q_ASSERT_X(m_oddMultipler >= 1,
		   "invalid argument",
		   "Odd multipler is less then 1");
	Q_ASSERT_X(m_evenMultipler >= 1,
		   "invalid argument",
		   "Even multipler is less then 1");
	qDebug("evenOddChecksum constructor() : end");
      }
    T operator()(T checksum, T symbolIndex = NOT_FOUND) {
      qDebug("evenOddChecksum operator() : start");
      if (symbolIndex == NOT_FOUND) {
	qDebug("evenOddChecksum operator(missing index) : end");
	return checksum;
      }     
      qDebug() << "parity=" << m_parity << " symbolIndex=" << symbolIndex; 
      // 1 -> size of array
      // even
      if (m_parity % 2 == 0) {
	checksum += symbolIndex * m_evenMultipler;
	qDebug() << "Even parity; checksum =" << checksum;
      }
      // odd
      else {
	checksum += symbolIndex * m_oddMultipler;
	qDebug() << "Odd parity; checksum =" << checksum;
      }	
      m_parity++;      
      qDebug("evenOddChecksum operator() : end");
      return checksum;
    }
    private:
      T m_oddMultipler;
      T m_evenMultipler;
      int m_parity;
    };

  /**
   * linear increament multipler
   * 
   * @tparam T type of numbers 
   */
  template <class T> class LinearMultiple :
       public std::binary_function<T,T, T> 
    {
    public:
      /**
       * @param maxWeight maximum weighting value: 1->x
       */
      LinearMultiple (T maxWeight) :
		      m_maxWeight(maxWeight),
		      m_multipler(1)
      {
	// empty
      }
      T operator() (const T &checksum, const T &symbolIndex) const 
      {
	if (m_multipler >= m_maxWeight) { 
	  m_multipler = 1; 
	}
	return checksum + (symbolIndex * m_multipler++);	   
      }
    private:
      T m_maxWeight;
      mutable T m_multipler;
  };        
};

namespace baseSymbols {
  // 0 -9
  const int NUM_SYMBOLS = 10; 
  
  // symbols = {0 - 9}
  const char SYMBOL_LOOKUP [NUM_SYMBOLS][2] = {
	  "0", 
	  "1", 
	  "2", 
	  "3", 
	  "4", 
	  "5", 
	  "6", 
	  "7", 
	  "8", 
	  "9"
  };
  // symbol length
  const int SYMBOL_LENGTH = 1;
};
#endif // SHARED_DEFINES_H