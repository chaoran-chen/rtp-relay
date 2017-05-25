#include "rtpanalyzer.h"
#include "rtppacket.h"
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QIODevice>


RtpAnalyzer::RtpAnalyzer(QString logFileName, QObject *parent) : QObject(parent), logFile_(logFileName)
{
    logFile_.open(QIODevice::WriteOnly);
    qDebug() << "Log file path: " << QDir::currentPath();
}

void RtpAnalyzer::analyzePacket(QNetworkDatagram packet)
{
    QByteArray data = packet.data();

    RtpPacket p = RtpPacket::fromQByteArray(data);
     qDebug() << p.toString();

    // Measure packages per second
    qint64 currentSecsSinceEpoch = QDateTime::currentDateTime().toSecsSinceEpoch();
    if (currentSecsSinceEpoch != lastSecsSinceEpoch_) {
        printNumberPackages_(lastSecsSinceEpoch_, numberPackagesInLastSec_);
        lastSecsSinceEpoch_ = currentSecsSinceEpoch;
        numberPackagesInLastSec_ = 0;
    }
    ++numberPackagesInLastSec_;
}


void RtpAnalyzer::printNumberPackages_(qint64 secsSinceEpoch, quint32 numberPackages) {
    QTextStream(&logFile_) << secsSinceEpoch * 1000 << "," << numberPackages << "\n";
}
