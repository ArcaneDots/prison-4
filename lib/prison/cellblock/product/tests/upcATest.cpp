 
//#include "upcATest.h"

#include <QtTest/QTest>

#include "upcATest.h"
#include <iostream>
using namespace product;

 void TestUpcA::defaultConstructor()
 {
   UpcAEngine * UpcA = new UpcAEngine();
   
   QStringList parsedSymbols( QString("0,1,2,3,4,5,6,7,8,9,0,1").split(",") );
   QStringList finalSymbols ( QString("0,1,2,3,4,5,6,7,8,9,0,5").split(",") );
   QStringList formatedSymbols( QString("0 12345 67890 5").split(" ") ); 
   QString encodedBlocks;   
   
   encodedBlocks = QString("00000000101000110100110010010011011110101000110110001010");
   encodedBlocks += QString("1010100001000100100100011101001110010100111010100000000");
   shared::BarPositionsMap barLocations;
   
   QCOMPARE(UpcA->userInput(), QString(""));
   QCOMPARE(UpcA->productCode(), upc_common::PS__UPC_A);
   QCOMPARE(UpcA->codeDefault(), upcA::DEFAULT_VALUE);
   QCOMPARE(UpcA->parsedSymbolList(), parsedSymbols); 
   QCOMPARE(UpcA->finalSymbolList(), finalSymbols); 
   QCOMPARE(UpcA->formatedSymbols(), formatedSymbols);
   QCOMPARE(UpcA->encodedSymbols(barLocations), encodedBlocks);
   
   delete UpcA;
 }

 QTEST_MAIN(TestUpcA);
 #include "upcATest.moc"
 //DECLARE_TEST(TestUpcA)