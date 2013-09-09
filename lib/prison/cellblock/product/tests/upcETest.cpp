#include <QtTest/QTest>

#include "upcETest.h"
#include <iostream>
using namespace product;

 void TestUpcE::defaultConstructor()
 {
   UpcEEngine * UpcE = new UpcAEngine();
   
   QStringList parsedSymbols( QString("0,1,2,3,4,5,6,7,8,9,0,1").split(",") );
   QStringList finalSymbols ( QString("0,1,2,3,4,5,6,7,8,9,0,5").split(",") );
   QStringList formatedSymbols( QString("0 12345 67890 5").split(" ") ); 
   QString encodedBlocks;   
   
   encodedBlocks = QString("00000000101000110100110010010011011110101000110110001010");
   encodedBlocks += QString("1010100001000100100100011101001110010100111010100000000");
   shared::BarPositionsMap barLocations;
   
   QCOMPARE(UpcE->userInput(), QString(""));
   QCOMPARE(UpcE->productCode(), upc_common::PS__UPC_A);
   QCOMPARE(UpcE->codeDefault(), upcA::DEFAULT_VALUE);
   QCOMPARE(UpcE->parsedSymbolList(), parsedSymbols); 
   QCOMPARE(UpcE->finalSymbolList(), finalSymbols); 
   QCOMPARE(UpcE->formatedSymbols(), formatedSymbols);
   QCOMPARE(UpcE->getEncodedSymbols(barLocations), encodedBlocks);
   
   delete UpcE;
 }

 QTEST_MAIN(TestUpcE);
 #include "upcATest.moc"
 //DECLARE_TEST(TestUpcA)