#include "wdgTvController.h"
#include "ui_wdgTvController.h"

#include "tvNetworkControl.h"

#include <QFile>

wdgTvController::wdgTvController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wdgTvController)
{
    ui->setupUi(this);
    m_tvController = new tvNetworkControl(this);
    connect(m_tvController, SIGNAL(logText(QString)), ui->txtEditInformations, SLOT(appendHtml(QString)));

    QFile file(":/samsungKeyCodes.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))   {
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            //make sure we don't have spacing characters that would break the base64 code
            QString lineStr(line);
            lineStr.remove(' ');
            lineStr.remove('\n');
            ui->ctlSendKey->addItem(lineStr);
        }
    }
}

wdgTvController::~wdgTvController()
{
    delete ui;
}

void wdgTvController::on_ctlIP_returnPressed()
{
    m_tvController->connectToTV(ui->ctlIP->text());
}

void wdgTvController::on_btConnect_clicked()
{
    m_tvController->connectToTV(ui->ctlIP->text());
}

void wdgTvController::on_btSendID_clicked()
{
    m_tvController->sendIDPacket(ui->ctlRemoteIP->text(),
                                 ui->ctlRemoteID->text(),
                                 ui->ctlRemoteName->text());

}

void wdgTvController::on_btSendKey_clicked()
{
    m_tvController->sendKey(ui->ctlSendKey->currentText());
}
