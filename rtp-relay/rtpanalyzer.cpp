#include "rtpanalyzer.h"
#include <QDebug>

RtpAnalyzer::RtpAnalyzer(QObject *parent) : QObject(parent)
{

}

void RtpAnalyzer::analyzePacket(QNetworkDatagram packet)
{
    qDebug() << "received packet, size is" << packet.data().size();
}
