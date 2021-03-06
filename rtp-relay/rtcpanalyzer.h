#ifndef RTCPANALYZER_H
#define RTCPANALYZER_H

#include <QObject>
#include <QNetworkDatagram>

class RtcpAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit RtcpAnalyzer(QObject *parent = 0);

    void analyzePacket(QNetworkDatagram packet);

};

#endif // RTCPANALYZER_H
