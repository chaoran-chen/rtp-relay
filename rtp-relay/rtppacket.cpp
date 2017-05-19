#include "rtppacket.h"
#include "utils.h"
#include <QDebug>
#include <QJsonArray>
#include <QtEndian>
#include <QJsonDocument>


const QMap<quint8, QString> payloadTypes =
{
    {0, "PCMU"},
    {31, "H261"},
    {32, "MPV"},
    {33, "MP2T"},
    {34, "H263"}
};

RtpPacket::RtpPacket()
{

}

QString RtpPacket::toString() {
    QJsonObject obj = toJsonObject();
    QJsonDocument doc(obj);
    QString json(doc.toJson(QJsonDocument::Compact));
    return json;
}

QJsonObject RtpPacket::toJsonObject() {
    QJsonArray jsonCsrcList;
    for (quint32 csrc : csrcList) {
        jsonCsrcList.append(static_cast<qint64>(csrc));
    }
    QJsonObject obj
    {
        {"byteLength", byteLength},
        {"padding", padding},
        {"extension", extension},
        {"csrcCount", csrcCount},
        {"marker", marker},
        {"payloadType", payloadTypes[payloadType]},
        {"sequenceNumber", sequenceNumber},
        {"timestamp", static_cast<qint64>(timestamp)},
        {"ssrc", static_cast<qint64>(ssrc)},
        {"csrcList", jsonCsrcList},
        {"numberExtensions", numberExtensions}
    };
    return obj;
}

RtpPacket RtpPacket::fromQByteArray(QByteArray bytes) {

    RtpPacket p;
    p.byteLength = bytes.size();
    auto data = reinterpret_cast<unsigned char*>(bytes.data());

    // Parse bytes 0 - 12
    p.padding = extractBits(data[0], 2, 1);
    p.extension = extractBits(data[0], 3, 1);
    p.csrcCount = extractBits(data[0], 4, 4);
    p.marker = extractBits(data[1], 0, 1);
    p.payloadType = extractBits(data[1], 1, 7);
    p.sequenceNumber = qFromBigEndian<quint16>(data + 2);
    p.timestamp = qFromBigEndian<quint32>(data + 4);
    p.ssrc = qFromBigEndian<quint32>(data + 8);

    // Parse CSRC List
    for (quint16 i = 0; i < p.csrcCount; i++) {
        quint16 offset = 12 + 4 * i;
        p.csrcList.append(
                (data[offset] << 24) |
                (data[offset + 1] << 16) |
                (data[offset + 2] << 8) |
                data[offset + 3]);
    }

    // Skip Extension Headers
    p.numberExtensions = 0;
    unsigned char *payload = data + 12 + 4 * p.csrcCount;
    if (p.extension) {
        quint16 extensionType = qFromBigEndian<quint16>(payload);
        quint16 numberHeaders = qFromBigEndian<quint16>(payload + 2);
        p.numberExtensions = numberHeaders;

        unsigned char *currentExtension = payload + 4;
        for (quint16 i = 0; i < numberHeaders; i++) {
            quint8 numberDataBytes;
            if(extensionType == 0xBEDE) {
                numberDataBytes = extractBits(currentExtension[0], 4, 4) + 1;
            } else {
                numberDataBytes = currentExtension[1];
            }

            currentExtension += extensionType + numberDataBytes;
        }
    }

    return p;
}
