#ifndef RTPPACKET_H
#define RTPPACKET_H

#include <QtGlobal>
#include <QString>
#include <QVector>
#include <QJsonObject>

class RtpPacket
{
public:
    QString toString();
    QJsonObject toJsonObject();
    static RtpPacket fromQByteArray(QByteArray bytes);

    quint16 byteLength;
    bool padding;
    bool extension;
    quint8 csrcCount;
    bool marker;
    quint8 payloadType;
    quint16 sequenceNumber;
    quint32 timestamp;
    quint32 ssrc;
    QVector<quint32> csrcList;
    quint16 numberExtensions;

private:
    RtpPacket();
};

#endif // RTPPACKET_H
