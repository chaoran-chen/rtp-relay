#include "rtpanalyzer.h"
#include "rtppacket.h"
#include <QDebug>

RtpAnalyzer::RtpAnalyzer(QObject *parent) : QObject(parent)
{
}

void RtpAnalyzer::analyzePacket(QNetworkDatagram packet)
{
    QByteArray data = packet.data();

    RtpPacket p = RtpPacket::fromQByteArray(data);
    qDebug() << p.toString();
}
