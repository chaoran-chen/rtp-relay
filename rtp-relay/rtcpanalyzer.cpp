#include "rtcpanalyzer.h"
#include <QDebug>

RtcpAnalyzer::RtcpAnalyzer(QObject *parent) : QObject(parent)
{
    counter_ = 10;
}

void RtcpAnalyzer::analyzePacket(QNetworkDatagram packet)
{
    QByteArray data = packet.data();

    qDebug() << "received packet, size is" << data.size();
    counter_--;
}
