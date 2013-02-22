#ifndef TVNETWORKCONTROL_H
#define TVNETWORKCONTROL_H

#include <QObject>
#include <QAbstractSocket>

class QTcpSocket;

/**
 * @brief Control a Samsung Smart TV
 */

class tvNetworkControl : public QObject
{
    Q_OBJECT
public:
    explicit tvNetworkControl(QObject *parent = 0);

    /**
     * @brief Opens the TCP socket to connect to the TV
     * @param The TV's IP
     */
    void connectToTV(const QString & ip);

    /**
     * @brief Sends an identification packet to the TV, these information will be registered in the TV.
     * @param The remote's IP (seems to work with any IP here)
     * @param ID an unique identifier
     * @param The remote's displayed name
     */
    void sendIDPacket(const QString &remoteIP, const QString &ID, const QString &name);

    /**
     * @brief Send a QByteArray to the TV
     * @param data An array of bytes
     */
    void sendRaw(const QByteArray &data);

    /**
     * @brief send a Command to the TV.
     * @param key: the Samsung key (list of possible values are in txt file SamsungKeyCodes.txt)
     */
    void sendKey(const QString &key);

private Q_SLOTS:

    /**
     * @brief Reached when data is available from the TV. emits the recieved frame.
     */
    void sltReadyRead();

    /**
     * @brief Reached when socket is connected. emits "connected" text
     */
    void sltConnected();

    /**
     * @brief Reached when socket is connected. emits "disconnected" text
     */
    void sltDisconnected();

    /**
     * @brief Reached when socket generates an error. emits a text containing the error
     * @param sltError
     */
    void sltError(QAbstractSocket::SocketError sltError);

Q_SIGNALS:
    /**
     * @brief Emitted when the class wants to communicate with the exterior.
     * @param str: a message
     */
    void logText(const QString &str);

private:

    QTcpSocket *m_socket;
    bool m_connected;

    /**
     * @brief Encodes a string in base64 and and prepends the string size.
     * @param str: the string to encode
     * @return the corresponding byteArray
     */
    QByteArray encodeString(const QString &str);

    /**
     * @brief Provides a nice way of visualising recieved datagrams
     * @param The raw array
     * @param needHexConversion true if the array needs to be converted to hexadecimal notation.
     * @return a formatted string
     */
    QString byteArrayToHexaString(const QByteArray &array, bool needHexConversion = true) const;


};

#endif // TVNETWORKCONTROL_H
