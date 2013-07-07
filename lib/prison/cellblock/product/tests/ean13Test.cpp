 
//#include "upcATest.h"

#include <QtTest/QTest>

#include "ean13Test.h"

using namespace product;

void TestEan13::defaultConstructor()
 {
   Ean13Engine * Ean13 = new Ean13Engine();
   
   QStringList parsedSymbols( QString("1,2,3,4,5,6,7,8,9,0,1,2,3").split(",") );
   QStringList finalSymbols ( QString("1,2,3,4,5,6,7,8,9,0,1,2,8").split(",") );
   QStringList formatedSymbols( QString("1 234567 890128") .split(" ") ); 
   QString encodedBlocks;   
  
   encodedBlocks = QString("0000000010100100110111101001");
   encodedBlocks += QString("1101011000100001010010001010");
   encodedBlocks += QString("1010010001110100111001011001");
   encodedBlocks += QString("101101100100100010100000000");
   shared::BarPositionsMap barLocations;
   
   QCOMPARE(Ean13->userInput(), QString(""));
   QCOMPARE(Ean13->productCode(), upc_common::PS__EAN_13);
   QCOMPARE(Ean13->codeDefault(), ean13::DEFAULT_VALUE);
   QCOMPARE(Ean13->parsedSymbolList(), parsedSymbols); 
   QCOMPARE(Ean13->finalSymbolList(), finalSymbols); 
   QCOMPARE(Ean13->formatedSymbols(), formatedSymbols);
   QCOMPARE(Ean13->encodedSymbols(barLocations), encodedBlocks);
   
   delete Ean13;
 }

 QTEST_MAIN(TestEan13);
 #include "ean13Test.moc"
 //DECLARE_TEST(TestUpcA)