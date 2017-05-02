#ifndef RTPANALYZER_H
#define RTPANALYZER_H

#include <QObject>
#include <QNetworkDatagram>

class RtpAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit RtpAnalyzer(QObject *parent = 0);

    void analyzePacket(QNetworkDatagram packet);
};

#endif // RTPANALYZER_H
