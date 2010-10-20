/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QMessageBox>

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
    emit startSyncro();

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    try
    {
        Saver saver;
        saver.startTransaction();
        
        QString clientUuid = processGetUuid(clientConnection, saver);
        processGetUpdates(clientConnection, saver);
        
        connect(clientConnection, SIGNAL(disconnected()),
                clientConnection, SLOT(deleteLater()));
        
        
        saver.commit();
    } catch (std::runtime_error& _ex) {       
        DEBUG("ERROR: Sync error " << _ex.what())
        QMessageBox::critical(NULL, tr("Error"), _ex.what());
    }

    clientConnection->disconnectFromHost();
    clientConnection->deleteLater();
    
    emit stopSyncro();
}


QString Server::getRemoteUuid(QTcpSocket *clientConnection)
{
    QStringList lines = readLines(clientConnection);

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

QStringList Server::readLines(QTcpSocket *clientConnection) {
    TRACE;
    uint currentSize = 0;
    QStringList result;
    QByteArray req;
    
    do 
    {
        Q_ASSERT(QThread::currentThread() == clientConnection->thread());
        if( clientConnection->bytesAvailable()==0 && !clientConnection->waitForReadyRead() )
            throw std::runtime_error("Timeout on reading");
        req = clientConnection->readLine(MAX_MESSAGE);
        result << req;
//        DEBUG("Line is readed, size - " << req.length());
//        DEBUG("Line is readed - " << QString(req));
        
        currentSize += req.length();
        if( currentSize>MAX_MESSAGE )
            throw std::runtime_error("Client message is too big");
    } while( req.length() && req!="\r\n" );

    return result;
}

/// Process /get_uuid request. Returns remote uuid
QString Server::processGetUuid(QTcpSocket *clientConnection, Saver& _saver) {
    TRACE;
    QString clientUuid = getRemoteUuid(clientConnection);
    DEBUG("Remote UUID - " << clientUuid);
    emptyInputStream(clientConnection);
    
    QString body;
    {
        QTextStream out(&body);        
        out << "{\"uuid\":" << _saver.getLocalUuid()
            << ",\"lastUpdated\":" << _saver.getLastUpdated(clientUuid)
            << "}";
    }
    QString data;
    QTextStream out(&data);
    out << "HTTP/1.1 200 OK\n"
        << "Content-length:" << body.length() << "\n"
        << "Connection: Keep-Alive\n"
        << "Content-Type: text/plain\n"
        << "\n"
        << body
        << "\n";
    
    
    clientConnection->write(data.toUtf8());
    
    return clientUuid;
}

/// Process /get_updates request
void Server::processGetUpdates(QTcpSocket *clientConnection, Saver& _saver) {
    TRACE;
    time_t fromTime = getRemoteLastUpdated(clientConnection);
    DEBUG("Will transfer entities from time " << fromTime);
    emptyInputStream(clientConnection);

    QString body;
    {
        QTextStream out(&body);        
        out << "{\"tasks\":{},"
            << ",\"activities\":{}"
            << "}";
    }
    QString data;
    QTextStream out(&data);
    out << "HTTP/1.1 200 OK\n"
        << "Content-Length:" << body.length()
        << "\n"
        << body
        << "\n";
    
    
    clientConnection->write(data.toUtf8());
}

/// Returns start of update interval for remote host
time_t Server::getRemoteLastUpdated(QTcpSocket *clientConnection) {
    TRACE;
    QStringList lines = readLines(clientConnection);
    TRACE;

    if( lines.size()<2 )
        throw std::runtime_error("Wrong HTTP request");                    

    // Req line
    QStringList tokens = lines[0].split(" ");
    if( tokens[1]!="/get_updates" )
        throw std::runtime_error("Wrong HTTP request - incorrect sequence - " + tokens[1].toStdString());                    

    // Headers
    for(int i=1; i<lines.size(); ++i) {
        QStringList tokens = lines[i].split(":");
        if( !tokens[0].compare("fromTime", Qt::CaseInsensitive) ) {
            QString result = tokens[1];
            if( result.isEmpty() )
                throw std::runtime_error("'fromTime' is empty");                    
            
            time_t tm = 0;
            bool ok;
            tm = result.toInt(&ok);
            return tm;
        }
    }
    
    throw std::runtime_error("'fromTime' is not specified");
}

/// does readAll
void Server::emptyInputStream(QTcpSocket *clientConnection) {
//    clientConnection->readAll();
}
