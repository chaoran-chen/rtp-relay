#ifndef RTCPPACKET_H
#define RTCPPACKET_H

#include <QByteArray>

class RtcpPacket
{
public:
    RtcpPacket();

    static RtcpPacket fromByteArray(QByteArray bytes);

private:

};

#endif // RTCPPACKET_H
