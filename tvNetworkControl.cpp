#include "tvNetworkControl.h"

#include <QTcpSocket>
#include <QStringList>
#include <QtEndian>

tvNetworkControl::tvNetworkControl(QObject *parent) :
    QObject(parent),
    m_socket(0),
    m_connected(false)
{
}

void tvNetworkControl::connectToTV(const QString & ip)
{
    if (m_socket) {
        delete m_socket;
    }
    m_socket = new QTcpSocket(this);
    m_socket->connectToHost(ip, 55000);
    connect(m_socket, SIGNAL(connected()), this, SLOT(sltConnected()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(sltDisconnected()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(sltError(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(sltReadyRead()));
}

void tvNetworkControl::sltConnected()
{
    emit logText(tr("<b>Connected!</b><br>"));
    m_connected = true;
}

void tvNetworkControl::sltReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;
    QByteArray recievedData = socket->readAll().toHex();
    emit logText(tr("============ <b>Recieved Datagram</b> =============<br>%1").arg(byteArrayToHexaString(recievedData, false)));
}

void tvNetworkControl::sltDisconnected()
{
    emit logText(tr("<b>Disconnected!</b><br>"));
    m_socket->abort();
    m_connected = false;
}

void tvNetworkControl::sltError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    emit logText(tr("<b>Network error: %1</b><br>").arg(m_socket->errorString()));
}

void tvNetworkControl::sendIDPacket(const QString &remoteIP, const QString &ID, const QString &name)
{
    const char packetStart[] = {
        0x00, 0x13, 0x00, 0x69, 0x70, 0x68, 0x6f, 0x6e, 0x65, 0x2e, 0x69, 0x61, 0x70, 0x70, 0x2e, 0x73,
        0x61, 0x6d, 0x73, 0x75, 0x6e, 0x67
    };

    const QByteArray remoteIPEncoded = encodeString(remoteIP);
    const QByteArray remoteIDEncoded = encodeString(ID);
    const QByteArray remoteNameEncoded = encodeString(name);

    const char payLoadStart[] = {
        0x64, 0x00
    };

    QByteArray payLoad;
    payLoad.append(QByteArray::fromRawData(payLoadStart, sizeof(payLoadStart)));
    payLoad.append(QByteArray::fromRawData(remoteIPEncoded.data(), remoteIPEncoded.size()));
    payLoad.append(QByteArray::fromRawData(remoteIDEncoded.data(), remoteIDEncoded.size()));
    payLoad.append(QByteArray::fromRawData(remoteNameEncoded.data(), remoteNameEncoded.size()));
    const qint16 payLoadSize = payLoad.size();

    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.writeRawData(packetStart, sizeof(packetStart));
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << payLoadSize;
    stream.setByteOrder(QDataStream::BigEndian);
    stream.writeRawData(payLoad, payLoadSize);

    if (m_socket) {
        emit logText(tr("=========== <b>Send ID Datagram</b> ===========<br>%1").arg(byteArrayToHexaString(packet)));
        sendRaw(packet);
    }
}

void tvNetworkControl::sendKey(const QString &key)
{

#ifdef QT5
    QByteArray keyByteArray(key.toLatin1());
#else
    QByteArray keyByteArray(key.toAscii());
#endif

    keyByteArray = keyByteArray.toBase64();

    const char rawKeyPacketStart[] = {
        0x00, 0x13, 0x00, 0x69, 0x70, 0x68, 0x6f, 0x6e, 0x65, 0x2e, 0x69, 0x61, 0x70, 0x70, 0x2e, 0x73,
        0x61, 0x6d, 0x73, 0x75, 0x6e, 0x67
    };

    const char payloadStart[] = {0x00, 0x00, 0x00};

    const qint16 keySize = keyByteArray.size();
    const qint16 payloadSize = keyByteArray.size() + 5; // 5 is 3 0x00 bytes + 2 bytes for the key code size

    QByteArray keyPacket;
    QDataStream stream(&keyPacket, QIODevice::WriteOnly);
    stream.writeRawData(rawKeyPacketStart, sizeof(rawKeyPacketStart));
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << payloadSize;
    stream.setByteOrder(QDataStream::BigEndian);
    stream.writeRawData(payloadStart, sizeof(payloadStart));
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << keySize;
    stream.setByteOrder(QDataStream::BigEndian);
    stream.writeRawData(keyByteArray, keyByteArray.size());

    if (m_socket) {
        emit logText(tr("============ <b>Send Datagram</b> =============<br>%1").arg(byteArrayToHexaString(keyPacket)));
        sendRaw(keyPacket);
    }
}

void tvNetworkControl::sendRaw(const QByteArray &data)
{
    if (m_connected) {
        m_socket->write(data, data.size());
    }
}

QString tvNetworkControl::byteArrayToHexaString(const QByteArray &array, bool needHexConversion) const
{
    QString hex;
    if (needHexConversion) {
        hex = QString(array.toHex());
    } else {
        hex = QString(array);
    }

    int byteCount = 0;
    for (int i = 0; i < hex.size() + 1; i++) { //add a space to make groups of bytes
        if ((i+1) % 3) {
            hex.insert(i++, ' ');
            byteCount++;
        }
    }

    // big hack to break lines, ugly but it works
    QStringList byteList = hex.split(' ');
    for (double i = 1; i < (byteCount / 16.0) + 1; i++) {
        byteList.insert(i * 17, "<br>");
    }

    // make it nicer, still super hacky but it does the trick
    hex = byteList.join(" ");
    hex.replace(QString("<br> "), QString("<br>"));
    return hex;
}

QByteArray tvNetworkControl::encodeString(const QString &str)
{
#ifdef QT5
    const QByteArray strBase64 = QByteArray(str.toLatin1()).toBase64();
#else
    const QByteArray strBase64 = QByteArray(str.toAscii()).toBase64();
#endif

    const qint16 strSize = strBase64.size();
    QByteArray encodedStr;
    QDataStream stream(&encodedStr, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << strSize;
    stream.setByteOrder(QDataStream::BigEndian);
    stream.writeRawData(strBase64, strSize);
    return encodedStr;
}
