#include <QCoreApplication>

#include "udprelay.h"
#include "rtpanalyzer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UdpRelay relay;
    RtpAnalyzer analyzer;
    QObject::connect(&relay, &UdpRelay::receivedPacket, &analyzer, &RtpAnalyzer::analyzePacket);
    relay.start(7070, QHostAddress::LocalHost, 7071);

    return a.exec();
}
