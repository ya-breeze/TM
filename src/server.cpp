
#include <QMessageBox>
#include <QAbstractSocket>

#include <stdexcept>

#include "server.h"
#include "utils.h"

Server::Server(QWidget *parent)
    : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen( QHostAddress::Any, 9090) ) {
        QMessageBox::critical(NULL, tr("Error"),
                              tr("Unable to start the network server: %1.")
                              .arg(tcpServer->errorString()));
        return;
    }
    
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(sendFortune()));
}

void Server::sendFortune()
{
    DEBUG("Accepted new connection");
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    Connection *connection = new Connection(this, clientConnection, m_Saver);
}

Connection::Connection(QObject *_parent, QTcpSocket *_clientConnection, Saver &_saver) 
    : QObject(_parent), p_ClientConnection(_clientConnection), m_State(WAITING_UUID), m_Saver(_saver) {

    TRACE;
    m_Buffer.open(QBuffer::ReadWrite);
    connect(p_ClientConnection, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(p_ClientConnection, SIGNAL(disconnected()), this, SLOT(disconnected()));

    m_Saver.startTransaction();
}

void Connection::disconnected() {
    DEBUG("Connection is closed");
    if( m_State!=FINISHED )
        m_Saver.rollback();
    
    this->deleteLater();
}

void Connection::readyRead() {
    TRACE;
    QByteArray data = p_ClientConnection->readAll();
    m_Buffer.write(data);
//    DEBUG("readed " << data.size() << " bytes");
//    DEBUG(data.data());
    
    if( data.contains("\r\n\r\n") )
        onEmptyLine();    
}

void Connection::onEmptyLine() {
    DEBUG("Empty line detected. Current state - " << m_State);

    try
    {
        switch(m_State) {
            case WAITING_UUID :
                str_ClientUuid = processGetUuid(m_Saver);
                m_State = WAITING_DOWNLOAD;
                DEBUG("Current state - " << m_State);
            break;
            case WAITING_DOWNLOAD :
                processGetUpdates(m_Saver);
                m_State = WAITING_UPLOAD;
                DEBUG("Current state - " << m_State);
            break;
            case WAITING_UPLOAD :
                m_State = FINISHED;
                m_Saver.commit();
                DEBUG("Current state - " << m_State);
            break;
        }
    } catch (std::runtime_error& _ex) {       
        m_Saver.rollback();
        DEBUG("ERROR: Sync error " << _ex.what())
        QMessageBox::critical(NULL, tr("Error"), _ex.what());
        deleteLater();
    }
}


QString Connection::getRemoteUuid()
{
    QStringList lines = getHeaders();

    if( lines.size()<2 )
        throw std::runtime_error("Wrong HTTP request");                    

    // Req line
    QStringList tokens = lines[0].split(" ");
    if( tokens[1]!="/get_uuid" )
        throw std::runtime_error("Wrong HTTP request - incorrect sequence - " + tokens[1].toStdString());                    

    // Headers
    for(int i=1; i<lines.size(); ++i) {
        QStringList tokens = lines[i].split(":");
        if( !tokens[0].compare("uuid", Qt::CaseInsensitive) ) {
            QString result = tokens[1];
            if( result.isEmpty() )
                throw std::runtime_error("Client UUID is empty");                    
            return result;
        }
    }
    
    throw std::runtime_error("Client UUID is not specified");
}

/// Process /get_uuid request. Returns remote uuid
QString Connection::processGetUuid(Saver& _saver) {
    TRACE;
    QString clientUuid = getRemoteUuid();
    DEBUG("Remote UUID - " << clientUuid);
    
    QString body;
    {
        QTextStream out(&body);        
        out << "{\"uuid\":" << _saver.getLocalUuid()
            << ",\"lastUpdated\":" << _saver.getLastUpdated(clientUuid)
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
        << body
        << "\r\n";
    
    
    p_ClientConnection->write(data.toUtf8());
    
    return clientUuid;
}

/// Process /get_updates request
void Connection::processGetUpdates(Saver& _saver) {
    TRACE;
//    time_t fromTime = getRemoteLastUpdated(clientConnection);
//    DEBUG("Will transfer entities from time " << fromTime);
//    emptyInputStream(clientConnection);

//    QString body;
//    {
//        QTextStream out(&body);        
//        out << "{\"tasks\":{},"
//            << ",\"activities\":{}"
//            << "}";
//    }
//    QString data;
//    QTextStream out(&data);
//    out << "HTTP/1.1 200 OK\n"
//        << "Content-Length:" << body.length()
//        << "\n"
//        << body
//        << "\n";
    
    
//    clientConnection->write(data.toUtf8());
}

/// Returns start of update interval for remote host
time_t Connection::getRemoteLastUpdated() {
    TRACE;
//    QStringList lines = readLines(clientConnection);
//    TRACE;

//    if( lines.size()<2 )
//        throw std::runtime_error("Wrong HTTP request");                    

//    // Req line
//    QStringList tokens = lines[0].split(" ");
//    if( tokens[1]!="/get_updates" )
//        throw std::runtime_error("Wrong HTTP request - incorrect sequence - " + tokens[1].toStdString());                    

//    // Headers
//    for(int i=1; i<lines.size(); ++i) {
//        QStringList tokens = lines[i].split(":");
//        if( !tokens[0].compare("fromTime", Qt::CaseInsensitive) ) {
//            QString result = tokens[1];
//            if( result.isEmpty() )
//                throw std::runtime_error("'fromTime' is empty");                    
            
//            time_t tm = 0;
//            bool ok;
//            tm = result.toInt(&ok);
//            return tm;
//        }
//    }
    
    throw std::runtime_error("'fromTime' is not specified");
}

QStringList Connection::getHeaders() {
    TRACE;
    QString str = m_Buffer.data();
    m_Buffer.close();
    m_Buffer.open(QBuffer::ReadWrite);

    QStringList result = str.split("\r\n", QString::SkipEmptyParts, Qt::CaseInsensitive);
    for(int i=0; i<result.size(); ++i)
        DEBUG(result[i]);
    
    return result;
}
