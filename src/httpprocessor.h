#ifndef HTTPPROCESSOR_H
#define HTTPPROCESSOR_H

#include <QBuffer>
#include <QTcpSocket>

#include "utils.h"
#include "Saver.h"

/// Класс обрабытывающий приходящие запросы синхронизации
class HttpProcessor
{
    Saver &m_Saver;
    QString     str_ClientUuid;

public:
    HttpProcessor(Saver &_saver);
    void process(const QString& _url, const QStringMap& _headers, const QBuffer& _body, QTcpSocket *_clientConnection);

protected:
    /// Process /get_updates request
    void processGetUpdates(const QStringMap& _headers, const QBuffer& _body, QTcpSocket *_clientConnection);
    /// Process /send_updates request
    void processSendUpdates(const QStringMap& _headers, const QBuffer& _body, QTcpSocket *_clientConnection);
    /// Process /get_uuid request. Returns remote uuid
    QString processGetUuid(const QStringMap& _headers, const QBuffer& _body, QTcpSocket *_clientConnection);
    QString getRemoteUuid(const QStringMap& _headers);
    /// Returns start of update interval for remote host
    time_t getRemoteLastUpdated(const QStringMap& _headers);

    QString getTasks() const;
    QString getActivities() const;

};

#endif // HTTPPROCESSOR_H
