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
    while(readSocket_.hasPendingDatagrams()) {
        QNetworkDatagram datagram = readSocket_.receiveDatagram();
        writeSocket_.writeDatagram(datagram);
        receivedPacket(datagram);
    }
}
