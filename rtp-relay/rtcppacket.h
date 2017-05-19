#ifndef RTCPPACKET_H
#define RTCPPACKET_H

#include <QByteArray>

class RtcpPacket
{
public:
    static RtcpPacket fromByteArray(QByteArray bytes);

    bool padding;
    quint8 receptionBlocks;
    quint8 packetType;
    quint16 length;
    quint32 ssrc;
    quint64 ntpTimestamp;
    quint32 rtpTimesampt;
    quint32 spc;
    quint32 soc;

private:
    RtcpPacket() = default;

};

#endif // RTCPPACKET_H
