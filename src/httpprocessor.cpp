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
	    QString id = obj["id"].toString().toLower();
	    QString status = obj["status"].toString().toLower();
	    DEBUG("Object - " << type << ":" << status);

	    if( type=="task" ) {
		QVariantMap data = obj["data"].toMap();
		Task task;
		task.setId( id );
		if( status=="deleted") {
		    // TODO Check remote timestamp is smaller than saver.updated
//		    m_Saver.removeTask(task);
		} else if( status=="updated" ) {
		    task.setParentId( QUuid(data["parentUuid"].toString()) );
		    task.setName(data["title"].toString());
		    task.setNotes(data["notes"].toString());
		    task.setCreated( QDateTime::fromTime_t(data["created"].toInt()) );
		    int dt = data["started"].toInt();
		    if( dt )
			task.setStarted( QDateTime::fromTime_t(dt) );
		    dt = data["finished"].toInt();
		    if( dt )
			task.setFinished( QDateTime::fromTime_t(dt) );
		    task.setPlannedTime(data["planned"].toString());
		    task.setParentIndex(data["parentIndex"].toInt());
		    task.setCategories(data["categories"].toString().split(";"));
		    task.setIconName(data["iconName"].toString());

		    // should change updated last
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
	out << "{\"tasks\":" << getTasks(fromTime) << ","
	    << "\"activities\":[" << getActivities(fromTime) << "]"
	    << "} ";
    }
    QString data;
    QTextStream out(&data);
    out << "HTTP/1.1 200 OK\r\n"
	<< "content-type: text/plain; charset=UTF8\r\n"
	<< "Content-Length:" << body.toUtf8().length() << "\r\n\r\n"
	<< body;

    DEBUG("Will transfer entities from time " << fromTime << ", body size " << body.toUtf8().length() << " not " << body.length());
//    DEBUG(body);
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

QString HttpProcessor::getTasks(time_t _from) const {
    QVariantList list;

    // List of updates
    Saver::ChangeLogList updates = m_Saver.getUpdatesList(_from);

    // Iterate through updates
    QListIterator<Saver::ChangeLogItem> it(updates);
    while( it.hasNext() ) {
	Saver::ChangeLogItem item = it.next();
	if( item.type=="task" ) {
	    QVariantMap object;
	    QString id = item.id;
	    id = id.mid(1, id.length()-2);
	    object["id"] = id;
	    object["type"] = item.type;
	    object["status"] = item.status==Saver::ST_DELETED ? "deleted" : "updated";
	    object["timestamp"] = (int)item.timestamp;
	    QVariantMap data;

	    // This is task, and it was updated
	    if( item.status==Saver::ST_UPDATED ){
		Task task = m_Saver.restoreDbTask(item.id);
		if( task.getId().isNull() ) {
		    DEBUG("ERROR: Task " << task.getId().toString() << " present in ChangeLog but absent in Tasks table");
		    continue;
		}

		QString parentId = task.getParentId().toString();
		parentId = parentId.mid(1, parentId.length()-2);
		data["parentUuid"] = parentId;
		data["updated"] = (int)task.getUpdated().toTime_t();
		data["title"] = task.getName();
		data["notes"] = task.getNotes().isEmpty() ? QString(" ") : task.getNotes();
		data["created"] = (int)task.getCreated().toTime_t();
		data["started"] = task.getStarted().isNull() ? 0 : (int)task.getStarted().toTime_t();
		data["finished"] = task.getFinished().isNull() ? 0 : (int)task.getFinished().toTime_t();
		data["planned"] = task.getPlannedTime();
		data["parentIndex"] = task.getParentIndex();
		data["categories"] = task.getCategories().join(";");
		data["iconName"] = task.getIconName();
	    }

	    //
	    object["data"] = data;
	    QVariantMap objects;
	    objects["object"] = object;
	    list << objects;
	}
    }

    QString res = QxtJSON::stringify(list);
//    DEBUG("Local tasks - " << res);

    return res;
}
QString HttpProcessor::getActivities(time_t _from) const {
    QString result;
    return result;
}
