#include "prison.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QSplitter>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include "prison/BarcodeItem"
#include "prison/BarcodeWidget"
#include "prison/DataMatrixBarcode"
#include "prison/QRCodeBarcode"
#include "prison/Code39Barcode"
#include "prison/Code93Barcode"
#include "prison/UpcEBarcode"
#include "prison/UpcABarcode"
#include "prison/Ean8Barcode"
#include "prison/Ean13Barcode"

void main_window::data_changed() {
  QString result = m_lineedit->text();
  m_dmi->setData(result);
  m_dmw->setData(result);
  m_qri->setData(result);
  m_qrw->setData(result);
  
  m_39i->setData(result);
  m_39w->setData(result);
  m_93i->setData(result);
  m_93w->setData(result);

  m_upcew->setData(result);
  m_upcai->setData(result);
  m_upcaw->setData(result);
  m_ean8w->setData(result);
  m_ean13i->setData(result);
  m_ean13w->setData(result);
  
  m_nulli->setData(result);
  m_nullw->setData(result);
  
  m_dmcolor->setData(result);
  m_qrcolor->setData(result);
  m_39color->setData(result);
  m_93color->setData(result);
  
}

main_window::main_window() {
  QHBoxLayout* inputLay = new QHBoxLayout();
  m_lineedit = new QLineEdit(this);
  QPushButton* but = new QPushButton(this);
  connect(but,SIGNAL(clicked()),SLOT(data_changed()));
  inputLay->addWidget(m_lineedit);
  inputLay->addWidget(but);

  QVBoxLayout* mainlay = new QVBoxLayout();

  m_dmw = new prison::BarcodeWidget(new prison::DataMatrixBarcode(),this);
  m_qrw = new prison::BarcodeWidget(new prison::QRCodeBarcode(),this);
  
  m_39w = new prison::BarcodeWidget(new prison::Code39Barcode(),this);
  m_93w = new prison::BarcodeWidget(new prison::Code93Barcode(),this);
  
  m_upcew = new prison::BarcodeWidget(new prison::UpcEBarcode(),this);
  m_upcaw = new prison::BarcodeWidget(new prison::UpcABarcode(),this);
  m_ean8w = new prison::BarcodeWidget(new prison::Ean8Barcode(),this);
  m_ean13w = new prison::BarcodeWidget(new prison::Ean13Barcode(),this);
  
  m_dmcolor = new prison::BarcodeWidget(this);
  {
    prison::DataMatrixBarcode* dmcolorcode = new prison::DataMatrixBarcode();
    dmcolorcode->setForegroundColor(Qt::red);
    dmcolorcode->setBackgroundColor(Qt::darkBlue);
    m_dmcolor->setBarcode(dmcolorcode);
  }
  m_qrcolor = new prison::BarcodeWidget(this);
  {
    prison::QRCodeBarcode* qrcolorcode =  new prison::QRCodeBarcode();
    qrcolorcode->setForegroundColor(Qt::red);
    qrcolorcode->setBackgroundColor(Qt::darkBlue);
    m_qrcolor->setBarcode(qrcolorcode);
  }
  m_39color = new prison::BarcodeWidget(this);
  {
    prison::Code39Barcode* c39colorcode =  new prison::Code39Barcode();
    c39colorcode->setForegroundColor(Qt::red);
    c39colorcode->setBackgroundColor(Qt::darkBlue);
    m_39color->setBarcode(c39colorcode);
  }
  m_93color = new prison::BarcodeWidget(this);
  {
    prison::Code93Barcode* c93colorcode =  new prison::Code93Barcode();
    c93colorcode->setForegroundColor(Qt::red);
    c93colorcode->setBackgroundColor(Qt::darkBlue);
    m_93color->setBarcode(c93colorcode);
  }
  
  QGraphicsScene* scene = new QGraphicsScene(this);

  m_dmi = new prison::BarcodeItem(new prison::DataMatrixBarcode());
  m_qri = new prison::BarcodeItem(new prison::QRCodeBarcode());
  
  m_39i = new prison::BarcodeItem(new prison::Code39Barcode());
  m_93i = new prison::BarcodeItem(new prison::Code93Barcode());
  
  m_upcai = new prison::BarcodeItem(new prison::UpcABarcode());
  m_ean13i = new prison::BarcodeItem(new prison::Ean13Barcode());
  
  m_nulli = new prison::BarcodeItem();
  m_nullw = new prison::BarcodeWidget();

  scene->addItem(m_dmi);
  m_dmi->setPos(0,0);
  scene->addItem(m_qri);
  m_qri->setPos(200,200);
  
//   scene->addItem(m_ean13i);
//   m_ean13i->setPos(0,400);
  
  scene->addItem(m_39i);
  m_39i->setPos(0,400);
  scene->addItem(m_93i);
  m_93i->setPos(200,400);
  scene->addItem(m_nulli);
  m_nulli->setPos(0,200);

  prison::BarcodeItem* with_value = new prison::BarcodeItem();
  with_value->setBarcode(new prison::QRCodeBarcode());
  with_value->setData("hello");
  scene->addItem(with_value);
  with_value->setPos(200,0);
  
  QGraphicsView* v = new QGraphicsView(this);
  v->setScene(scene);

  
  QHBoxLayout* scenelay = new QHBoxLayout();
  scenelay->addWidget(v);
  
  QHBoxLayout* splitterlay = new QHBoxLayout();

  QSplitter* d2Splitter = new QSplitter(Qt::Vertical);
  d2Splitter->addWidget(m_dmw);
  d2Splitter->addWidget(m_qrw);
  d2Splitter->addWidget(m_dmcolor);
  d2Splitter->addWidget(m_qrcolor);
  d2Splitter->addWidget(m_nullw);
  
  QSplitter* mainSplitter = new QSplitter(Qt::Vertical);
  mainSplitter->addWidget(m_39w);
  mainSplitter->addWidget(m_93w);
  mainSplitter->addWidget(m_39color);
  mainSplitter->addWidget(m_93color);

  QSplitter* upcSplitter = new QSplitter(Qt::Vertical);
  upcSplitter->addWidget(m_upcew);
  upcSplitter->addWidget(m_upcaw);
  upcSplitter->addWidget(m_ean8w);
  upcSplitter->addWidget(m_ean13w);  
  
  mainlay->addLayout(inputLay);
  mainlay->addLayout(scenelay);
  splitterlay->addWidget(d2Splitter);
  splitterlay->addWidget(mainSplitter);
  splitterlay->addWidget(upcSplitter);
  mainlay->addLayout(splitterlay);
  setLayout(mainlay);

  m_lineedit->setText("AOEUIAOEUIAOEUI");
  data_changed();
}

#include "prison.moc"
