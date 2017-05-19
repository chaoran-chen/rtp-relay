#include "rtcppacket.h"


RtcpPacket::RtcpPacket()
{

}

RtcpPacket RtcpPacket::fromByteArray(QByteArray bytes)
{
//    auto data = reinterpret_cast<unsigned char*>(bytes.data());

//    bool padding        = extractBits(data[0], 2, 1);
//    bool extension      = data[0] & (1 << 3);
//    quint8 csrcCount    = data[0] & (0b1111 << 4);
//    bool marker         = data[0] & (1 << 8);
//    quint8 payloadType  = data[0] & (0b1111111 << 9);
}
