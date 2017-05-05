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

RtpPacket RtpPacket::fromQByteArray(const QByteArray bytes) {

    RtpPacket p;
    p.byteLength_ = bytes.size();

    // Parse bytes 0 - 12
    p.padding_ = bytes[0] & 0x20;
    p.extension_ = bytes[0] & 0x10;
    p.csrcCount_ = bytes[0] & 0x0f;
    p.marker_ = bytes[1] & 0x80;
    p.payloadType_ = bytes[1] & 0x7f;
    p.sequenceNumber_ = (bytes[2] << 8) | bytes[3];
    p.timestamp_ = (bytes[4] << 24) | (bytes[5] << 16) | (bytes[6] << 8) | bytes[7];
    p.ssrc_ = (bytes[8] << 24) | (bytes[9] << 16) | (bytes[10] << 8) | bytes[11];

    // Parse CSRC List
    for (quint16 i = 0; i < p.csrcCount_; i++) {
        quint16 offset = 12 + 4 * i;
        p.csrcList_.append(
                (bytes[offset] << 24) |
                (bytes[offset + 1] << 16) |
                (bytes[offset + 2] << 8) |
                bytes[offset + 3]);
    }

    // Skip Extension Headers
    p.numberExtensions_ = 0;
    quint16 payloadOffset = 12 + 4 * p.csrcCount_;
    if (p.extension_) {
        quint16 extensionType = (bytes[payloadOffset] == 0xbe) && (bytes[payloadOffset + 1] == 0xde) ? 1 : 0;
        quint16 numberHeaders = (bytes[payloadOffset + 2] << 8) | bytes[payloadOffset + 3];
        payloadOffset += 4;
        for (quint16 i = 0; i < numberHeaders; i++) {
            quint8 numberDataBytes = extensionType == 1 ?
                (bytes[payloadOffset] & 0xf) - 1 :
                bytes[payloadOffset + 1];
            p.numberExtensions_++;
            payloadOffset += extensionType + numberDataBytes;
        }
    }
    if (payloadOffset % 4) {
        payloadOffset = ((payloadOffset >> 2) +1) << 2;
    }

    // Payload
    p.payload_ = bytes.mid(payloadOffset);

    return p;
}
