#include "httpprocessor.h"

#include <stdexcept>

#include <qjson/parser.h>

HttpProcessor::HttpProcessor(Saver &_saver)
    : m_Saver(_saver)
{
}

void HttpProcessor::process(const QString& _url, const QStringMap& _headers, const QBuffer& _body, QTcpSocket *_clientConnection) {
    Transaction<Saver> t(m_Saver);

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
QString HttpProcessor::processGetUuid(const QStringMap& _headers, const QBuffer& _body, QTcpSocket *_clientConnection) {
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

void HttpProcessor::processSendUpdates(const QStringMap& _headers, const QBuffer& _body, QTcpSocket *_clientConnection) {
    DEBUG(__PRETTY_FUNCTION__);
    bool ok;
    int sz = _headers["content-length"].toInt(&ok);

    // Парсим
    {
	DEBUG(QString::fromUtf8(_body.data()));
	QJson::Parser parser;
	QByteArray in;
	in.append(QString::fromUtf8(_body.data()));
	QVariantList json = parser.parse(in, &ok).toList();
	if (!ok)
	    throw std::runtime_error("error occured during parsing JSON");
	DEBUG("Received updated objects: "<< json.size());

	// Обрабаываем
	foreach (QVariant object, json) {
	    QVariantMap obj = object.toMap();
	    obj = obj.begin().value().toMap();
	    QString type = obj["type"].toString().toLower();
	    QString status = obj["status"].toString().toLower();
	    DEBUG("Object - " << type << ":" << status);

	    if( type=="task" ) {

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
void HttpProcessor::processGetUpdates(const QStringMap& _headers, const QBuffer& _body, QTcpSocket *_clientConnection) {
    TRACE;
    time_t fromTime = getRemoteLastUpdated(_headers);

    QString body;
    {
	QTextStream out(&body);
	out << "{\"tasks\":[" << getTasks() << "],"
	    << "\"activities\":[" << getActivities() << "]"
	    << "}";
    }
    QString data;
    QTextStream out(&data);
    out << "HTTP/1.1 200 OK\r\n"
	<< "Content-Length:" << body.length() << "\r\n\r\n"
	<< body;

    DEBUG("Will transfer entities from time " << fromTime << ", body size " << body.length());
//    DEBUG(getTasks());
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
    QString result;
    QTextStream ss(&result);

    Saver::TaskMap tasks = m_Saver.restoreDbTasks();
    if( !tasks.empty()) {
	for(Saver::TaskMap::iterator it=tasks.begin();;) {
	    ss << "{";

	    QString id = it->second.getId().toString();
	    QString parentId = it->second.getParentId().toString();
	    id = id.mid(1, id.length()-2);
	    parentId = parentId.mid(1, parentId.length()-2);

	    ss  << "\"uuid\":\"" << id << "\", "
		<< "\"localUpdated\":1287384070856, "
		<< "\"globalUpdated\":1287384070856, "
		<< "\"parentUuid\":\"" << parentId << "\", "
//                << "\"title\":\"" << (*it)->getName().replace("\"", "\\\"") << "\", "
//                << "\"notes\":\"" << (*it)->getNotes() << "\"";
		<< "\"title\":\"\", "
		<< "\"notes\":\"\"";
	    ss << "}";

	    ++it;
	    if( it==tasks.end() )
		break;
	    ss << ", ";
	}
    }
    return result;
}
QString HttpProcessor::getActivities() const {
    QString result;
    return result;
}
