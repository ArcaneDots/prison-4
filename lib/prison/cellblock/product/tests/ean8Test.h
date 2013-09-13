 
#ifndef TEST_EAN8
#define TEST_EAN8

#include <QtCore/QObject>
#include <QtTest/QTest>

 #include "../ean8engine.h"
 
 
 class TestEan8: public QObject
 {
   Q_OBJECT
 
   
 private slots:
   void defaultConstructor();
 };
 
 #endif // TEST_EAN8