#ifndef UDPRELAY_H
#define UDPRELAY_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>
#include <QHostAddress>
#include <QNetworkDatagram>

class UdpRelay : public QObject
{
    Q_OBJECT
public:
    explicit UdpRelay(QObject *parent = 0);

    void start(quint16 listenerPort, QHostAddress targetAddress, quint16 targetPort);

signals:
    void receivedPacket(QNetworkDatagram packet);

private:
    void onReadyRead();

    QUdpSocket readSocket_;
    QUdpSocket writeSocket_;
    QHostAddress target_;
    qint16 targetPort_;
};

#endif // UDPRELAY_H
