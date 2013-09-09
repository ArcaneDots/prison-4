#include <QtTest/QTest>

#include "ean8Test.h"
#include <iostream>
using namespace product;

 void TestEan8::defaultConstructor()
 {
   Ean8Engine * Ean8 = new Ean8Engine();
   
   QStringList parsedSymbols( QString("1,2,3,4,5,6,7,8").split(",") );
   QStringList finalSymbols ( QString("1,2,3,4,5,6,7,0").split(",") );
   QStringList formatedSymbols( QString("< 1234 5670").split(" ") ); 
   QString encodedBlocks;   
   
   encodedBlocks = QString("000000001010011001001001101111010100011010");
   encodedBlocks += QString("10100111010100001000100111001010100000000");
   shared::BarPositionsMap barLocations;
   
   QCOMPARE(Ean8->userInput(), QString(""));
   QCOMPARE(Ean8->productCode(), upc_common::PS__EAN_8);
   QCOMPARE(Ean8->codeDefault().join(" "), ean8::DEFAULT_VALUE);
   QCOMPARE(Ean8->parsedSymbolList(), parsedSymbols); 
   QCOMPARE(Ean8->finalSymbolList(), finalSymbols); 
  // QCOMPARE(Ean8->formatedSymbols(), formatedSymbols);
   QCOMPARE(Ean8->getEncodedSymbols(barLocations), encodedBlocks);
   
   delete Ean8;
 }

 QTEST_MAIN(TestEan8);
 #include "ean8Test.moc"
 
 