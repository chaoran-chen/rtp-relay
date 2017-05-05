#include "rtppacket.h"
#include <QDebug>
#include <QJsonArray>
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
    QJsonArray csrcList;
    for (quint32 csrc : csrcList_) {
        csrcList.append(static_cast<qint64>(csrc));
    }
    QJsonObject obj
    {
        {"byteLength", byteLength_},
        {"padding", padding_},
        {"extension", extension_},
        {"csrcCount", csrcCount_},
        {"marker", marker_},
        {"payloadType", payloadTypes[payloadType_]},
        {"sequenceNumber", sequenceNumber_},
        {"timestamp", static_cast<qint64>(timestamp_)},
        {"ssrc", static_cast<qint64>(ssrc_)},
        {"csrcList", csrcList},
        {"numberExtensions", numberExtensions_}
    };
    return obj;
}

RtpPacket RtpPacket::fromQByteArray(QByteArray bytes) {

    RtpPacket p;
    p.byteLength_ = bytes.size();
    auto data = reinterpret_cast<unsigned char*>(bytes.data());

    // Parse bytes 0 - 12
    p.padding_ = data[0] & 0x20;
    p.extension_ = data[0] & 0x10;
    p.csrcCount_ = data[0] & 0x0f;
    p.marker_ = data[1] & 0x80;
    p.payloadType_ = data[1] & 0x7f;
    p.sequenceNumber_ = (data[2] << 8) | data[3];
    p.timestamp_ = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
    p.ssrc_ = (data[8] << 24) | (data[9] << 16) | (data[10] << 8) | data[11];

    // Parse CSRC List
    for (quint16 i = 0; i < p.csrcCount_; i++) {
        quint16 offset = 12 + 4 * i;
        p.csrcList_.append(
                (data[offset] << 24) |
                (data[offset + 1] << 16) |
                (data[offset + 2] << 8) |
                data[offset + 3]);
    }

    // Skip Extension Headers
    p.numberExtensions_ = 0;
    unsigned char *payload = data + 12 + 4 * p.csrcCount_;
    if (p.extension_) {
        quint16 extensionType = *reinterpret_cast<quint16*>(payload);
        quint16 numberHeaders = *reinterpret_cast<quint16*>(payload + 2);
        p.numberExtensions_ = numberHeaders;

        unsigned char *currentExtension = payload + 4;
        for (quint16 i = 0; i < numberHeaders; i++) {
            quint8 numberDataBytes;
            if(extensionType == 0xBEDE) {
                numberDataBytes = (currentExtension[0] & 0x0f) + 1;
            } else {
                numberDataBytes = currentExtension[1];
            }

            currentExtension += extensionType + numberDataBytes;
        }
    }

    return p;
}
