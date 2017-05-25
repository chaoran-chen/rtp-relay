#ifndef RTPANALYZER_H
#define RTPANALYZER_H

#include <QObject>
#include <QNetworkDatagram>
#include <QFile>

class RtpAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit RtpAnalyzer(QString logFileName, QObject *parent = 0);

    void analyzePacket(QNetworkDatagram packet);

private:
    qint64 lastSecsSinceEpoch_ {0};

    quint32 numberPackagesInLastSec_ {0};

    QFile logFile_;

    void printNumberPackages_(qint64 secsSinceEpoch, quint32 numberPackages);
};

#endif // RTPANALYZER_H
