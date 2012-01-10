#include "prison.h"

#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include "prison/BarcodeItem"
#include "prison/BarcodeWidget"
#include "prison/DataMatrixBarcode"
#include "prison/QRCodeBarcode"
#include "prison/UpcEBarcode"
#include "prison/UpcABarcode"
#include "prison/Ean8Barcode"
#include "prison/Ean13Barcode"
#include <QtGui/QHBoxLayout>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QSplitter>

void main_window::data_changed() {
  QString result = m_lineedit->text();
  m_dmi->setData(result);
  m_dmw->setData(result);
  m_qri->setData(result);
  m_qrw->setData(result);
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
  
}

main_window::main_window() {
  QHBoxLayout* lay = new QHBoxLayout();
  m_lineedit = new QLineEdit(this);
  QPushButton* but = new QPushButton(this);
  connect(but,SIGNAL(clicked()),SLOT(data_changed()));
  lay->addWidget(m_lineedit);
  lay->addWidget(but);

  QVBoxLayout* mainlay = new QVBoxLayout();

  m_dmw = new prison::BarcodeWidget(new prison::DataMatrixBarcode(),this);
  m_qrw = new prison::BarcodeWidget(new prison::QRCodeBarcode(),this);
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
  QGraphicsScene* scene = new QGraphicsScene(this);

  m_dmi = new prison::BarcodeItem(new prison::DataMatrixBarcode());
  m_qri = new prison::BarcodeItem(new prison::QRCodeBarcode());
  m_upcai = new prison::BarcodeItem(new prison::UpcABarcode());
  m_ean13i = new prison::BarcodeItem(new prison::Ean13Barcode());

  m_nulli = new prison::BarcodeItem();
  m_nullw = new prison::BarcodeWidget();

  scene->addItem(m_dmi);
  m_dmi->setPos(0,0);
  scene->addItem(m_qri);
  m_qri->setPos(200,200);
  scene->addItem(m_ean13i);
  m_ean13i->setPos(0,400);
  scene->addItem(m_nulli);
  m_nulli->setPos(0,200);

  prison::BarcodeItem* with_value = new prison::BarcodeItem();
  with_value->setBarcode(new prison::QRCodeBarcode());
  with_value->setData("hello");
  scene->addItem(with_value);
  with_value->setPos(200,0);
  
  QGraphicsView* v = new QGraphicsView(this);
  v->setScene(scene);

  QSplitter* splitter = new QSplitter(Qt::Vertical);
  splitter->addWidget(v);
  splitter->addWidget(m_dmw);
  splitter->addWidget(m_qrw);
  splitter->addWidget(m_dmw);
  splitter->addWidget(m_upcew);
  splitter->addWidget(m_upcaw);
  splitter->addWidget(m_ean8w);
  splitter->addWidget(m_ean13w);
  splitter->addWidget(m_dmcolor);
  splitter->addWidget(m_qrcolor);
  splitter->addWidget(m_nullw);

  mainlay->addLayout(lay);
  mainlay->addWidget(splitter);
  setLayout(mainlay);

  m_lineedit->setText("AOEUIAOEUIAOEUI");
  data_changed();
}

#include "prison.moc"
