#ifndef RTPPACKET_H
#define RTPPACKET_H

#include <QtGlobal>
#include <QString>
#include <QVector>
#include <QJsonObject>

class RtpPacket
{
public:
    RtpPacket();
    QString toString();
    QJsonObject toJsonObject();

    static RtpPacket fromQByteArray(const QByteArray bytes);

private:
    quint16 byteLength_;
    bool padding_;
    bool extension_;
    quint8 csrcCount_;
    bool marker_;
    quint8 payloadType_;
    quint16 sequenceNumber_;
    quint32 timestamp_;
    quint32 ssrc_;
    QVector<quint32> csrcList_;
    quint16 numberExtensions_;
    QByteArray payload_;
};

#endif // RTPPACKET_H
