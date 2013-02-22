#ifndef TVNETWORKCONTROL_H
#define TVNETWORKCONTROL_H

#include <QObject>
#include <QAbstractSocket>

class QTcpSocket;

class tvNetworkControl : public QObject
{
    Q_OBJECT
public:
    explicit tvNetworkControl(QObject *parent = 0);

    void connectToTV(const QString & ip);
    void sendIDPacket(const QString &remoteIP, const QString &ID, const QString &name);
    void sendRaw(const QByteArray &data);
    void sendKey(const QString &key);
private slots:
    void sltReadyRead();
    void sltConnected();
    void sltDisconnected();
    void sltError(QAbstractSocket::SocketError sltError);
signals:
    void logText(const QString &str);

private:
    QTcpSocket *m_socket;
    bool m_connected;
    QByteArray encodeString(const QString &str);

    QString byteArrayToHexaString(const QByteArray &array, bool needHexConversion = true) const;

};

#endif // TVNETWORKCONTROL_H
