#include "rtcppacket.h"
#include "utils.h"
#include <QtEndian>

RtcpPacket RtcpPacket::fromByteArray(QByteArray bytes)
{
    auto data = reinterpret_cast<unsigned char*>(bytes.data());
    RtcpPacket p;
    p.padding         = extractBits(data[0], 2, 1);
    p.receptionBlocks = extractBits(data[0], 3, 5);
    p.packetType      = data[1];
    p.length          = qFromBigEndian<quint16>(data + 2);
    p.ssrc            = qFromBigEndian<quint32>(data + 4);
    p.ntpTimestamp    = qFromBigEndian<quint64>(data + 8);
    p.rtpTimesampt    = qFromBigEndian<quint32>(data + 16);
    p.spc             = qFromBigEndian<quint32>(data + 20);
    p.soc             = qFromBigEndian<quint32>(data + 24);

    return p;
}
