
#include <QtTest/QTest>
#include "ean8Test.h"
using namespace product;


QStringList TestEan8::encodedBlock(QList< shared::Symbol > symbolList, QString encoding)
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


void TestEan8::initTestCase()
{
  QStringList encodingLE, encodingLO, encodingR;
  for (int i = 0; i < upc_common::SYMBOL_TABLE_SIZE; i++) {
    encodingLE.append(upc_common::LEFT_EVEN_ENCODE_TABLE[i]);
    encodingLO.append(upc_common::LEFT_ODD_ENCODE_TABLE[i]);
    encodingR.append(upc_common::RIGHT_HAND_ENCODE_TABLE[i]);
  }

  m_encodingMap.insert("E", encodingLE);
  m_encodingMap.insert("O", encodingLO);
  m_encodingMap.insert("R", encodingR);
}


 void TestEan8::defaultConstructor()
 {
   QFETCH(QString, inputString);
   QFETCH(QString, parsedString);
   QFETCH(QString, finalString);

   Ean8Engine l_ean8(inputString);
   /// raw input
   QCOMPARE(l_ean8.userInput(), inputString);
   /// simple parsing
   QCOMPARE(l_ean8.parsedSymbolList(), parsedString);
   /// final valid barcode (includes extended block)
   QCOMPARE(l_ean8.finalSymbolList(), finalString);
   int validLength = finalString.size();
   QStringList formatedString;
   switch(validLength) {
     case 12:
       formatedString.append("");
       formatedString.append(finalString.mid(0, 4));
       formatedString.append(finalString.mid(4, 4));
       formatedString.append("");
       formatedString.append("");
       break;
     case 14:
     case 17:
       formatedString.append("");
       formatedString.append(finalString.mid(0, 4));
       formatedString.append(finalString.mid(4, 4));
       formatedString.append("");
       formatedString.append(finalString.mid(5));
       break;
     default:
       formatedString.append("");
       formatedString.append("");
       formatedString.append("");
       formatedString.append("");
       formatedString.append("");
       break;
   }
   QCOMPARE(l_ean8.formatedSymbols(), formatedString);
   /** 	final valid barcode split up into three encoded blocks
    * 	block1 - required
    * 	block2 - required
    * 	extended - empty unless extended block present
    */
   QString errorEncoding("1010101,1010101,1010101,1010101");
   QList<shared::Symbol> symbols = shared::Symbol().parse(finalString);
   QList<QStringList> local_encode;
   QStringList extEncoding;
   int symbolLength = symbols.size();
   switch(symbolLength) {
     case 12:
       local_encode.append(encodedBlock(symbols.mid(0, 4),"O"));
       local_encode.append(encodedBlock(symbols.mid(4, 4),"R"));
       local_encode.append(QStringList());
       break;
     case 14:
       local_encode.append(encodedBlock(symbols.mid(0, 4),"O"));
       local_encode.append(encodedBlock(symbols.mid(4, 4),"R"));
       {
	  // calculate checksum = (tens + one) % 4
	  QList<shared::Symbol> extendedSymbols(symbols.mid(5));
	  int extChecksum = extendedSymbols.at(0).getIndex() * 10 + extendedSymbols.at(1).getIndex();
	  int extRemainder = extChecksum % 4;
	  extEncoding.append(extendedSymbols.at(0).encoding((extRemainder < 3)? "O": "E"));
	  extEncoding.append(extendedSymbols.at(0).encoding((extRemainder % 2 == 0)? "O": "E"));
       }
       local_encode.append(extEncoding);
       break;
     case 17:
       local_encode.append(encodedBlock(symbols.mid(0, 4),"O"));
       local_encode.append(encodedBlock(symbols.mid(4, 4),"R"));
       {
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
       }
       local_encode.append(extEncoding);
       break;
     default:
       local_encode.append(QStringList(errorEncoding.split(",")));
       local_encode.append(QStringList(errorEncoding.split(",")));
       local_encode.append(QStringList());
       break;
   }

   QList<QStringList> testEncode(l_ean8.encoded());
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


void TestEan8::defaultConstructor_data()
{
  QTest::addColumn<QString>("inputString");
  QTest::addColumn<QString>("parsedString");
  QTest::addColumn<QString>("finalString");


  QTest::newRow("empty") 	     << "" << "" << "";
  QTest::newRow("letters")    	     << "sjkdhf" << "" << "";
  QTest::newRow("6_numbers") 	     << "546" << "546" << "";
//   QTest::newRow("no_check1")	      << "12345678901"       << "12345678901"      << "123456789012";
//   QTest::newRow("no_check2")	      << "37465442555"       << "37465442555"      << "374654425558";
//   QTest::newRow("no_check3")	      << "07567816412"       << "07567816412"      << "075678164125";
//   QTest::newRow("w_correct_check")    << "546544255525"      << "546544255525"     << "546544255525";
//   QTest::newRow("w_incorrect_check")  << "546544255523"      << "546544255523"     << "546544255525";
//   QTest::newRow("w_incorrect_check2") << "123456789018"      << "123456789018"     << "123456789012";
//   QTest::newRow("w_incorrect_ext2")   << "12345678901822"    << "12345678901822"   << "12345678901222";
//   QTest::newRow("w_incorrect_ext5")   << "27938472382878912" << "27938472382878912"<< "27938472382378912";
}

 QTEST_MAIN(TestEan8);
 #include "ean8Test.moc"
 
 