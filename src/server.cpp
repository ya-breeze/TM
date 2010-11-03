
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
//    Connection *connection = 
    new Connection(this, clientConnection, m_Saver);
}

Connection::Connection(QObject *_parent, QTcpSocket *_clientConnection, Saver &_saver) 
    : QObject(_parent), p_ClientConnection(_clientConnection), m_State(WAITING_UUID), m_Saver(_saver),
    is_WaitingHeaders(true) {
    
    TRACE;
    connect(p_ClientConnection, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(p_ClientConnection, SIGNAL(disconnected()), this, SLOT(disconnected()));

    m_Saver.startTransaction();

    m_Timer = startTimer(5000);
    was_NetData = false;
}
Connection::~Connection() {
    p_ClientConnection->deleteLater();
}

void Connection::disconnected() {
    DEBUG("Connection is closed");
    m_Saver.rollback();   
    this->deleteLater();
}

bool Connection::readHeaders(QTcpSocket *_sock, QStringList& _headers) {
    QByteArray data;
    while( !(data = _sock->readLine()).isEmpty() ) {
//        DEBUG("readed header '" << data.data() << "', size " << data.size());
        if( (data.size()==2 && data.contains("\r\n")) || (data.size()==1 && data.contains("\n")) ) {
            return true;
        } else
            _headers << data;            
    }

    return false;    
}

int Connection::readBody(QTcpSocket *_sock, QBuffer& _body, int _length) {
//    DEBUG("need read body with len " << _length);
    if( !_length )
        return 0;
    
    QByteArray data = _sock->read(_length);
    _body.write(data);
    _length -= data.size();    
//    DEBUG("readed body '" << data.data() << "', size " << data.size() << ", need read " << _length);

    return _length;
}

void Connection::readyRead() {
    TRACE;
    was_NetData = true;

    if( is_WaitingHeaders ) {
        is_WaitingHeaders = ! readHeaders(p_ClientConnection, m_Headers);
        
        if( !is_WaitingHeaders ) {
            // parse headers
            m_RequestHeaders = getHeaders(m_Headers);
            
            // prepare for reading body
            m_Buffer.close();
            m_Buffer.open(QBuffer::ReadWrite);        
            QString len = m_RequestHeaders["content-length"];
            bool ok;
            m_BodyLength = len.toInt(&ok);
            m_BodyLength = readBody(p_ClientConnection, m_Buffer, m_BodyLength);
        }
    } else {
        m_BodyLength = readBody(p_ClientConnection, m_Buffer, m_BodyLength);
    }
    
    if( is_WaitingHeaders==false && m_BodyLength==0 )
        if( !onEmptyLine() )
            p_ClientConnection->close();
}

bool Connection::onEmptyLine() {
    Q_ASSERT(!m_Headers.isEmpty());

    bool res = false;
    DEBUG("Processing request " << m_Headers[0]);
    killTimer(m_Timer);

    try
    {
        // Determine request type
        QStringList tokens = m_Headers[0].split(" ", QString::SkipEmptyParts);
        if( tokens.size()<3 )
            throw std::runtime_error( "Incorrect request line - " + m_Headers[0].toStdString() );
        
        if( tokens[1]=="/get_uuid" )
            str_ClientUuid = processGetUuid(m_Saver);
        else if( tokens[1]=="/get_updates" )
            processGetUpdates(m_Saver);
        else
            throw std::runtime_error( "Unknown request - " + m_Headers[0].toStdString() );
            
        // TODO else if( tokens[1]=="/send_updates" )
        
        // Reading another request
        DEBUG("Request is processed successfully");
        res = true;
    } catch (std::runtime_error& _ex) {       
        DEBUG("ERROR: Sync error " << _ex.what())
        QMessageBox::critical(NULL, tr("Error"), _ex.what());
    }
    clear();            
    
    return res;
}

void Connection::clear() {
    m_Timer = startTimer(5000);
    is_WaitingHeaders = true;
    m_Headers.clear();
    m_RequestHeaders.clear();
    // May be some bytes are in buffer already
    readyRead();    
}


QString Connection::getRemoteUuid()
{
    TRACE;
    for(int i=1; i<m_Headers.size(); ++i) {
        DEBUG(m_Headers[i]);
        QStringList tokens = m_Headers[i].split(":");
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
    time_t fromTime = getRemoteLastUpdated();
    DEBUG("Will transfer entities from time " << fromTime);
//    emptyInputStream(clientConnection);

    QString body;
    {
        QTextStream out(&body);        
        out << "{\"tasks\":{},"
            << "\"activities\":{}"
            << "}";
    }
    QString data;
    QTextStream out(&data);
    out << "HTTP/1.1 200 OK\r\n"
        << "Content-Length:" << body.length() << "\r\n\r\n"
        << body;
    
    
    p_ClientConnection->write(data.toUtf8());
}

/// Returns start of update interval for remote host
time_t Connection::getRemoteLastUpdated() {
    TRACE;

    QString result = m_RequestHeaders["fromtime"];
    if( result.isEmpty() )
        throw std::runtime_error("'fromTime' is empty or not specified");                    
    
    time_t tm = 0;
    bool ok;
    tm = result.toInt(&ok);
    return tm;
}

QStringMap Connection::getHeaders(const QStringList& _headers) {
    QStringMap res;
    
    for( int i=1; i<_headers.size(); ++i ) {
        int idx = _headers[i].indexOf(":");
        if( idx!=-1 ) {
            QString value = _headers[i].mid(idx + 1);
            int cut = value.length()-1;
            if( value.length()>=2 && value[value.length()-2]=='\r')
                --cut;
            res[ _headers[i].left(idx).toLower() ] = value.left(cut);
        } else
            res[ _headers[i].toLower() ] = "";
    }

//    QMapIterator<QString, QString> i(res);
//    while (i.hasNext()) {
//        i.next();
//        DEBUG( "header '" << i.key() << "' : '" << i.value() << "'");
//    }
    
    return res;
}

void Connection::timerEvent( QTimerEvent * ) {
    if( was_NetData==false ) {
        DEBUG("Network timeout - closing connection");
        p_ClientConnection->close();
    }
    was_NetData = false;
}
