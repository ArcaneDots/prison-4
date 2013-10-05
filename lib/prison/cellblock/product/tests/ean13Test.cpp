#include <QtTest/QTest>

#include "ean13Test.h"

using namespace product;

QStringList TestEan13::encodedBlock(QList< shared::Symbol > symbolList,
				    QString encoding)
{
  QStringList temp;
  if (symbolList.isEmpty() || m_encodingMap.value(encoding).isEmpty())  {
    qDebug() << "input symbols are empty";
    return temp;
  }
  QList<shared::Symbol>::const_iterator _first = symbolList.begin();
  QList<shared::Symbol>::const_iterator _last = symbolList.end();
  QStringList encodings = m_encodingMap.value(encoding);
  qDebug() << "encoding list used: " << encodings;
  while (_first != _last) {
    temp << encodings.at(_first->getIndex());
    _first++;
  }
  return temp;
}



void TestEan13::initTestCase()
{
  QStringList encodingLE, encodingLO, encodingR;
  for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
    encodingLE.append(upc_common::LEFT_EVEN_ENCODE_TABLE[i]);
    encodingLO.append(upc_common::LEFT_ODD_ENCODE_TABLE[i]);
    encodingR.append(upc_common::RIGHT_HAND_ENCODE_TABLE[i]);
  }

  for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
    m_parity13WidthEncoding.append(ean13::PARITY_13[i]);
  }

  m_encodingMap.insert("E", encodingLE);
  m_encodingMap.insert("O", encodingLO);
  m_encodingMap.insert("R", encodingR);
}

 void TestEan13::defaultConstructor()
 {
   QFETCH(QString, inputString);
   QFETCH(QString, parsedString);
   QFETCH(QString, finalString);

   Ean13Engine l_ean13(inputString);
   /// raw input
   QCOMPARE(l_ean13.userInput(), inputString);
   /// simple parsing
   QCOMPARE(l_ean13.parsedSymbolList(), parsedString);
   /// final valid barcode (includes extended block)
   QCOMPARE(l_ean13.finalSymbolList(), finalString);
   int validLength = finalString.size();
   QStringList formatedString;
   switch(validLength) {
     case 13:
       formatedString.append(finalString.at(0));
       formatedString.append(finalString.mid(1, 6));
       formatedString.append(finalString.mid(7, 6));
       formatedString.append("");
       formatedString.append("");
       break;
     case 15:
     case 18:
       formatedString.append(finalString.at(0));
       formatedString.append(finalString.mid(1, 6));
       formatedString.append(finalString.mid(7, 6));
       formatedString.append("");
       formatedString.append(finalString.mid(13));
       break;
     default:
       formatedString.append("");
       formatedString.append("");
       formatedString.append("");
       formatedString.append("");
       formatedString.append("");
       break;
   }
   qDebug() << "format" << l_ean13.formatedSymbols() << " " << formatedString;
   QCOMPARE(l_ean13.formatedSymbols(), formatedString);
   /** 	final valid barcode split up into three encoded blocks
    * 	block1 - required
    * 	block2 - required
    * 	extended - empty unless extended block present
    */
   QString errorEncoding("1010101,1010101,1010101,1010101,1010101,1010101");
   QList<shared::Symbol> symbols = shared::Symbol().parse(finalString);
   QList<QStringList> local_encode;
   QStringList extEncoding;

   int symbolLength = symbols.size();

   QVERIFY2(m_parity13WidthEncoding.size() == upc_common::SYMBOL_TABLE_SIZE,
	    "Encoding missing");

   // empty codes
   if (symbolLength == 0) {
     local_encode.append(QStringList(errorEncoding.split(",")));
     local_encode.append(QStringList(errorEncoding.split(",")));
     local_encode.append(QStringList());
   } else {
     // valid codes
     QVERIFY2(!QString(finalString.at(0)).isEmpty(), "input symbols are empty");
     int systemDigit = finalString.at(0).digitValue();
     QVERIFY2((systemDigit >= 0 && systemDigit <= m_parity13WidthEncoding.size()),
	      "system digit is empty");

     QString l_encoding(m_parity13WidthEncoding.at(systemDigit));
     QVERIFY2(!l_encoding.isEmpty(), "encoding is empty");
     qDebug() << "encoding is " << l_encoding;
     QList<shared::Symbol> symbolList(symbols.mid(1, 6));
     QVERIFY2(!symbolList.isEmpty(), "no symbols");

     QStringList l_block1;
     for(int i = 0; i < upc_common::ENCODE_BLOCK_SIZE; i++) {
       l_block1 << symbolList.at(i).encoding(l_encoding.at(i));
     }
     local_encode.append(l_block1);
     local_encode.append(encodedBlock(symbols.mid(7, 6),"R"));

    
      if (symbolLength == 14)
      {
	// calculate checksum = (tens + one) % 4
	QList<shared::Symbol> extendedSymbols(symbols.mid(12));
	int extChecksum = extendedSymbols.at(0).getIndex() * 10 + extendedSymbols.at(1).getIndex();
	int extRemainder = extChecksum % 4;
	extEncoding.append(extendedSymbols.at(0).encoding((extRemainder < 3)? "O": "E"));
	extEncoding.append(extendedSymbols.at(0).encoding((extRemainder % 2 == 0)? "O": "E"));
	//local_encode.append(extEncoding);
      }
      else if(symbolLength ==  17) {
	int oddParity = 3;
	int evenParity = 9;
	int extChecksum = 0;
	// calculate checksum = (odd * 3 + evem * 9) % 10
	QList<shared::Symbol> extendedSymbols(symbols.mid(12));
	for (int i = 1; i < 6; i++) {
	  extChecksum += extendedSymbols.at(i - 1) * ((i % 2 == 0) ? evenParity : oddParity);
	}
	int extRemainder = extChecksum % 10;
	QString specificEncoding(ean5::PARITY_5[extRemainder]);
	for (int i = 0; i < 5; i++) {
	  extEncoding.append(extendedSymbols.at(i).encoding(specificEncoding.at(i)));
	}
	//local_encode.append(extEncoding);
      }
      local_encode.append(extEncoding);
   }

   QList<QStringList> testEncode(l_ean13.encoded());
   qDebug() << "output hardcode: 1" << local_encode.at(0);
   qDebug() << "output hardcode: 2" << local_encode.at(1);
   qDebug() << "output hardcode: 3" << local_encode.at(2);
   qDebug() << "output testobjt: 1" << testEncode.at(0);
   qDebug() << "output testobjt: 2" << testEncode.at(1);
   qDebug() << "output testobjt: 3" << testEncode.at(2);
//    QCOMPARE(testEncode, local_encode);
   QCOMPARE(testEncode.at(0), local_encode.at(0));
   QCOMPARE(testEncode.at(1), local_encode.at(1));
   QCOMPARE(testEncode.at(2), local_encode.at(2));
 }


void TestEan13::defaultConstructor_data()
{
  QTest::addColumn<QString>("inputString");
  QTest::addColumn<QString>("parsedString");
  QTest::addColumn<QString>("finalString");


  QTest::newRow("empty") 	     << "" << "" << "";
  QTest::newRow("letters")    	     << "sjkdhf" << "" << "";
  QTest::newRow("6_numbers") 	     << "546544" << "546544" << "";
  QTest::newRow("no_check1")	     << "123456789012"  << "123456789012"  << "1234567890128";
  QTest::newRow("no_check2")	     << "374654425559"  << "374654425559"  << "3746544255595";
  QTest::newRow("no_check3")	     << "075678164123"  << "075678164123"  << "0756781641238";
  QTest::newRow("w_correct_check")    << "5465442555252" << "5465442555252" << "5465442555252";
  QTest::newRow("w_incorrect_check")  << "5465442555236" << "5465442555236" << "5465442555238";
  QTest::newRow("w_incorrect_check2") << "1234567890189" << "1234567890189" << "1234567890180";
//   QTest::newRow("w_incorrect_ext2")   << "12345678901822"    << "12345678901822"   << "12345678901222";
//   QTest::newRow("w_incorrect_ext5")   << "27938472382878912" << "27938472382878912"<< "27938472382378912";
}

 QTEST_MAIN(TestEan13);
 #include "ean13Test.moc" 
