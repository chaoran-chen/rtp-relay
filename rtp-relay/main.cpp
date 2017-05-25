#include <QCoreApplication>

#include "udprelay.h"
#include "rtpanalyzer.h"
#include "rtcpanalyzer.h"

#include "utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UdpRelay rtpRelay;
    RtpAnalyzer rtpAnalyzer("log.txt");
    QObject::connect(&rtpRelay, &UdpRelay::receivedPacket, &rtpAnalyzer, &RtpAnalyzer::analyzePacket);
    rtpRelay.start(7070, QHostAddress::LocalHost, 7080);

    UdpRelay rtcpRelay;
    RtcpAnalyzer rtcpAnalyzer;
    QObject::connect(&rtcpRelay, &UdpRelay::receivedPacket, &rtcpAnalyzer, &RtcpAnalyzer::analyzePacket);
    rtcpRelay.start(7071, QHostAddress::LocalHost, 7081);

    return a.exec();
}
