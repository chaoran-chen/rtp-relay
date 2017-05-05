#include "udprelay.h"
#include <QDebug>

UdpRelay::UdpRelay(QObject *parent) : QObject(parent)
{
    connect(&readSocket_, &QUdpSocket::readyRead, this, &UdpRelay::onReadyRead);
}

void UdpRelay::start(quint16 listenerPort, QHostAddress targetAddress, quint16 targetPort)
{
    readSocket_.bind(listenerPort);
    writeSocket_.connectToHost(targetAddress, targetPort);

    qDebug() << "started upd relay from port" << listenerPort
             << "to" << targetAddress.toString() << "port" << targetPort;
}

void UdpRelay::onReadyRead()
{
    int sent;
    while(readSocket_.hasPendingDatagrams()) {
        QNetworkDatagram datagram = readSocket_.receiveDatagram();
        sent = writeSocket_.write(datagram.data());
        receivedPacket(datagram);
        if (sent < 0) {
            qDebug() << "Error due sending packages: " << writeSocket_.errorString();
        }
    }
}
