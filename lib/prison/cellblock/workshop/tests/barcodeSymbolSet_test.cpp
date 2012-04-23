 
#include "../barcodesymbolset.h"
#include "../symboldatum.h"
#include "barcodeSymbolSet_test.h"
 
Test_BarcodeSymbolSet::Test_BarcodeSymbolSet()
{
  digits = QString("0 1 2 3 4 5 6 7 8 9").split(" ");
  Even = QString("E0 E1 E2 E3 E4 E5 E6 E7 E8 E9").split(" ");
  Odd = QString("O0 O1 O2 O3 O4 O5 O6 O7 O8 O9").split(" ");
  Right = QString("R0 R1 R2 R3 R4 R5 R6 R7 R8 R9").split(" ");
  
  //typedef LookupTableEntry<QString> StringEntries;
  EncodableSymbols digitStrings(digits);
  EncodingMap encodingStrings;
  encodingStrings.addRow("Even", Even);
  encodingStrings.addRow("Odd", Odd);
  encodingStrings.addRow("Right", Right);   
  
  
}

Test_BarcodeSymbolSet::~Test_BarcodeSymbolSet()
{

}

 
void Test_BarcodeSymbolSet::constructorD()
{
  EncodingMap encodingStrings;
  encodingStrings.addRow("Even", Even);
  encodingStrings.addRow("Odd", Odd);
  encodingStrings.addRow("Right", Right);   
  BarcodeSymbolSet testSymbolSet(StringTableEntry(digits), encodingStrings);
  const SymbolDatum * two = 0;
  two = testSymbolSet.getSymbol("2");
  QVERIFY(two->getString() == "2");
}

void Test_BarcodeSymbolSet::constructorCopy()
{ 
  EncodingMap encodingStrings;
  encodingStrings.addRow("Even", Even);
  encodingStrings.addRow("Odd", Odd);
  encodingStrings.addRow("Right", Right);   
  BarcodeSymbolSet testSymbolSet1(StringTableEntry(digits), encodingStrings);
  BarcodeSymbolSet testSymbolSet2(testSymbolSet1);
  QVERIFY(testSymbolSet1 == testSymbolSet2);
}


QTEST_MAIN(Test_BarcodeSymbolSet)
#include "barcodeSymbolSet_test.moc"

