
#ifndef ABSTRACTENGINE_PRIVATE
#define ABSTRACTENGINE_PRIVATE

#include "abstractbarcodeengine.h"

namespace linearBarcode
{

/**
 * @cond PRIVATE
 */

class AbstractBarcodeEnginePrivate 
{
public:   
  virtual ~AbstractBarcodeEnginePrivate()
  {
    qDebug("AbstractBarcodeEnginePrivate destructor");
  }
   
  /**
   * default string used if user input is invalid
   * 
   * @note product code specific
   */
  QString m_defaultString;    
  /** 
   * barcode's minimum number of symbols 
   * 
   * @note product code specific,
   * @note NOT_FOUND == no minimum
   */ 
  int m_minLength;
  /** 
   * barcode's maximum number of symbols
   * 
   * @note product code specific,
   * @note NOT_FOUND == no maximum
   */ 
  int m_maxLength;
  /**
   * Zero-based index of the product code's check digit
   * 
   * 
   * @note UPC-E: uses checkDigit from uncompressed UPC-A for encoding only
   * @note product code specific,
   * @note "-1" == all check digit appear at end of barcode
   * 
   */
  int m_checkDigitOffset;     
  /**
   * number of check digits
   */
  int m_numCheckDigits;
  /**
   * copy of original user input
   */
  QString m_userInputString;
   /**
   * is user input string valid
   * 
   * @note current behavior: used default string when user input is invalid
   */
  mutable CodeEngine::ErrorCodes m_isValid;
  /**
   * Input processing flags
   * 
   * @note not currently implemented
   */
  CodeEngine::ConstructCodes m_constructionFlags;
  
};
/**
 * @endcond
 */


/**
  * Calculate checksum 
  * 
  * @note used by EAN-8/13 and 2 of 5
  * @note "even" includes '0' 
  * 
  * @param oddMultipler 
  * @param evenMultipler
  */
  class EvenOddChecksum : public std::binary_function<int, Symbol, int>
  {
  public:
    EvenOddChecksum (int oddMultipler,
		    int evenMultipler) :
		    m_oddMultipler(oddMultipler),
		    m_evenMultipler(evenMultipler),
		    m_parity(1) // start "odd"
    {	  
      qDebug("evenOddChecksum constructor() : start");
      Q_ASSERT_X(m_oddMultipler >= 1, "invalid argument",
	"Odd multipler is less then 1");
      Q_ASSERT_X(m_evenMultipler >= 1, "invalid argument",
	"Even multipler is less then 1");
      qDebug("evenOddChecksum constructor() : end");
    }
    
    int operator()(int total, const Symbol & s) const {
      qDebug("evenOddChecksum operator() : start");
      // ignore symbols with no value
      if (s == shared::NOT_FOUND) {
	qDebug("evenOddChecksum operator(missing index) : end");
	return 0;
      }     
      qDebug() << "parity=" << m_parity << " symbolIndex=" << s; 
      int checksum = 0;
      // 1 -> size of array
      // even
      if (m_parity % 2 == 0) {
	checksum = s * m_evenMultipler;
	qDebug() << "Even parity; checksum =" << checksum;
      }
      // odd
      else {
	checksum = s * m_oddMultipler;
	qDebug() << "Odd parity; checksum =" << checksum;
      }	
      m_parity++;      
      qDebug("evenOddChecksum operator() : end");
      return (total + checksum);
    }
  private:
    int m_oddMultipler;
    int m_evenMultipler;
    mutable int m_parity;
  };
  
  /**
  * linear increament multipler
  * 
  * @param T type of numbers 
  */
  class LinearMultiple : public std::binary_function<int, AbstractSymbology, int>
  {
  public:
    /**
    * @param maxWeight maximum weighting value: 1->x
    */
    LinearMultiple (int maxWeight) :
      m_maxWeight(maxWeight),
      m_multipler(1)
    {
      // empty
    }
  int operator() (int total, const Symbol & s) const 
    {
      if (s == shared::NOT_FOUND) { return 0; }
      if (m_multipler >= m_maxWeight) { 
	m_multipler = 1; 
      }
      return (total + (s * m_multipler++));	   
    }
  private:
    int m_maxWeight;
    mutable int m_multipler;
  };      
 
};
  
#endif // ABSTRACTENGINE_PRIVATE