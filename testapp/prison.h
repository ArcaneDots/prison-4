#ifndef prison_H
#define prison_H

#include <QWidget>

namespace prison {
  class BarcodeWidget;
  class BarcodeItem;
}

class QLineEdit;
class main_window : public QWidget {
  Q_OBJECT
  public:
    main_window();
  public Q_SLOTS:
    void data_changed();
  private:
    QLineEdit* m_lineedit;
    // 2d
    prison::BarcodeWidget* m_dmw;
    prison::BarcodeWidget* m_qrw;
    // color 2d
    prison::BarcodeWidget* m_dmcolor;
    prison::BarcodeWidget* m_qrcolor;
    // linear
    prison::BarcodeWidget* m_39w;
    prison::BarcodeWidget* m_93w;
    prison::BarcodeWidget* m_39color;
    prison::BarcodeWidget* m_93color;
    // product codes
    //prison::BarcodeWidget* m_upcew;
    prison::BarcodeWidget* m_upcaw;
    prison::BarcodeWidget* m_ean8w;
    prison::BarcodeWidget* m_ean13w;
    
    prison::BarcodeItem* m_dmi;
    prison::BarcodeItem* m_qri;
    prison::BarcodeItem* m_39i;
    prison::BarcodeItem* m_93i;
    
    prison::BarcodeItem* m_ean13i;
    prison::BarcodeItem* m_upcai;
    // null
    prison::BarcodeItem* m_nulli;
    prison::BarcodeWidget* m_nullw;
};

#endif // prison_H
