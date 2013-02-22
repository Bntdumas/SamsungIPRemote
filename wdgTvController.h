#ifndef WDGTVCONTROLLER_H
#define WDGTVCONTROLLER_H

#include <QWidget>

namespace Ui {
class wdgTvController;
}

class tvNetworkControl;


/**
 * @brief Little gui utility to test the Samsung TV IP controller.
 */

class wdgTvController : public QWidget
{
    Q_OBJECT
    
public:
    explicit wdgTvController(QWidget *parent = 0);
    ~wdgTvController();

private slots:
    void on_ctlIP_returnPressed();
    void on_btConnect_clicked();
    void on_btSendID_clicked();


    void on_btSendKey_clicked();

private:
    Ui::wdgTvController *ui;
    tvNetworkControl *m_tvController;
};

#endif // WDGTVCONTROLLER_H
