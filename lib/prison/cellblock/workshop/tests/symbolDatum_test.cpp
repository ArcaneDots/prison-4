 
#include "symbolDatum_test.h"
#include "../barcodesymbolset.h"
#include "../symboldatum.h"

Test_SymbolDatum::Test_SymbolDatum(): QObject()
{
  //BarcodeSymbolSet
}

void Test_SymbolDatum::constructorD()
{
  SymbolDatum sDatum();
}

void Test_SymbolDatum::constructorCopy()
{

}

void Test_SymbolDatum::constructorEncoded()
{

}

void Test_SymbolDatum::constructorExpanded()
{

}


QTEST_MAIN(Test_SymbolDatum)
#include "symbolDatum_test.moc"