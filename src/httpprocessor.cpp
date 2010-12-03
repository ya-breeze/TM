#include "httpprocessor.h"

#include <stdexcept>

#include <QxtJSON>
#include <qjson/parser.h>
#include <qjson/serializer.h>

HttpProcessor::HttpProcessor(Saver &_saver)
    : m_Saver(_saver)
{
}

void HttpProcessor::process(const QString& _url, const QStringMap& _headers, const QBuffer& _body, QTcpSocket *_clientConnection) {
    Transaction<Saver> t(m_Saver);

    QMapIterator<QString, QString> i(_headers);
    while( i.hasNext() ) {
	i.next();
	DEBUG("header - " << i.key() << ":" << i.value());
    }

    if( _url=="/get_uuid" )
	str_ClientUuid = processGetUuid(_headers, _body, _clientConnection);
    else if( _url=="/get_updates" )
	processGetUpdates(_headers, _body, _clientConnection);
    else if( _url=="/send_updates" )
	processSendUpdates(_headers, _body, _clientConnection);
    else
	throw std::runtime_error( "Unknown request - " + _url.toStdString() );

    DEBUG("Request is processed successfully");
    t.commit();
}

QString HttpProcessor::getRemoteUuid(const QStringMap& _headers)
{
    TRACE;
    QString result = _headers["uuid"];
    if( result.isEmpty() )
	throw std::runtime_error("Client UUID is empty");

    return result;
}

/// Process /get_uuid request. Returns remote uuid
QString HttpProcessor::processGetUuid(const QStringMap& _headers, const QBuffer&, QTcpSocket *_clientConnection) {
    TRACE;
    QString clientUuid = getRemoteUuid(_headers);
    DEBUG("Remote UUID - " << clientUuid);

    QString body;
    {
	QTextStream out(&body);
	out << "{\"uuid\":" << m_Saver.getLocalUuid()
	    << ",\"lastUpdated\":" << m_Saver.getLastUpdated(clientUuid)
	    << "}";
    }
    QString data;
    QTextStream out(&data);
    out << "HTTP/1.1 200 OK\r\n"
	<< "Host: 10.0.2.2:9090\r\n"
	<< "Content-length:" << body.length() << "\r\n"
	<< "Connection: Keep-Alive\r\n"
	<< "Content-Type: text/plain\r\n"
	<< "\r\n"
	<< body;


    _clientConnection->write(data.toUtf8());

    return clientUuid;
}

void HttpProcessor::processSendUpdates(const QStringMap&, const QBuffer& _body, QTcpSocket *_clientConnection) {
    DEBUG(__PRETTY_FUNCTION__);

    // Парсим
    {
	QString body = QString::fromUtf8(_body.data());
	DEBUG(body);
	QVariantList json = QxtJSON::parse(body).toList();
	DEBUG("Received updated objects: "<< json.size());

	// Обрабаываем
	foreach (QVariant object, json) {
	    QVariantMap obj = object.toMap();
	    obj = obj.begin().value().toMap();
	    QString type = obj["type"].toString().toLower();
	    QString status = obj["status"].toString().toLower();
	    DEBUG("Object - " << type << ":" << status);

	    if( type=="task" ) {
		QVariantMap data = obj["data"].toMap();
		Task task;
		task.setId( QUuid(data["uuid"].toString()) );
		if( status=="deleted") {
		    m_Saver.removeTask(task);
		} else if( status=="updated" ) {
		    task.setParentId( QUuid(data["parentUuid"].toString()) );
		    task.setName(data["title"].toString());
		    task.setNotes(data["notes"].toString());
		    task.setCreated( QDateTime::fromTime_t(data["created"].toInt()) );
		    task.setUpdated( QDateTime::fromTime_t(data["updated"].toInt()) );
		    m_Saver.replaceTask(task);
		} else {
		    DEBUG("Unknown status: " << status << " for " << type);
		}

	    } else if( type=="activity" ) {

	    } else {
		DEBUG("Unknown object type: " << type);
	    }
	}
    }

    QString data;
    QTextStream out(&data);
    out << "HTTP/1.1 200 OK\r\n"
	<< "Content-Length: 0\r\n"
	<< "\r\n";
    _clientConnection->write(data.toUtf8());
}

/// Process /get_updates request
void HttpProcessor::processGetUpdates(const QStringMap& _headers, const QBuffer&, QTcpSocket *_clientConnection) {
    TRACE;
    time_t fromTime = getRemoteLastUpdated(_headers);

    QString body;
    {
	QTextStream out(&body);
	out << "{\"tasks\":" << getTasks() << ","
	    << "\"activities\":[" << getActivities() << "]"
	    << "} ";
    }
    QString data;
    QTextStream out(&data);
    out << "HTTP/1.1 200 OK\r\n"
	<< "content-type: text/plain; charset=UTF8\r\n"
	<< "Content-Length:" << body.length() << "\r\n\r\n"
	<< body;

    DEBUG("Will transfer entities from time " << fromTime << ", body size " << body.length());
    DEBUG(body);
    _clientConnection->write(data.toUtf8());
}

/// Returns start of update interval for remote host
time_t HttpProcessor::getRemoteLastUpdated(const QStringMap& _headers) {
    TRACE;

    QString result = _headers["fromtime"];
    if( result.isEmpty() )
	throw std::runtime_error("'fromTime' is empty or not specified");

    time_t tm = 0;
    bool ok;
    tm = result.toInt(&ok);
    return tm;
}

QString HttpProcessor::getTasks() const {
    QVariantList list;
    Saver::TaskMap tasks = m_Saver.restoreDbTasks();
    if( !tasks.empty()) {
	foreach(Task task, tasks.values()) {
	    QVariantMap data;

	    QString id = task.getId().toString();
	    QString parentId = task.getParentId().toString();
	    id = id.mid(1, id.length()-2);
	    parentId = parentId.mid(1, parentId.length()-2);
	    data["uuid"] = id;
	    data["parentUuid"] = parentId;
	    data["localUpdated"] = (int)task.getUpdated().toTime_t();
	    data["title"] = task.getName();
	    data["notes"] = task.getNotes().isEmpty() ? QString(" ") : task.getNotes();
	    data["created"] = (int)task.getCreated().toTime_t();

	    list << data;
	}
    }

    QString res = QxtJSON::stringify(list);
//    DEBUG("Local tasks - " << res);

    return res;
}
QString HttpProcessor::getActivities() const {
    QString result;
    return result;
}
