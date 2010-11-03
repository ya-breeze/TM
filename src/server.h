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

#ifndef SERVER_H
#define SERVER_H

#include <QMap>
#include <QtNetwork>

#include "Saver.h"

#define MAX_MESSAGE 1000000

typedef QMap<QString, QString> QStringMap;

class Server : public QObject
{
    Q_OBJECT
    
public:
    Server(QWidget *parent = 0);
    
private slots:
    void sendFortune();
    
private:
    QTcpServer *tcpServer;
    Saver       m_Saver;
};

class Connection : public QObject
{
    Q_OBJECT
    
    QTcpSocket *p_ClientConnection;
    bool        is_WaitingHeaders;
    QStringList m_Headers;
    QStringMap  m_RequestHeaders;
    QBuffer     m_Buffer;
    Saver       &m_Saver;
    QString     str_ClientUuid;
    int         m_BodyLength;
    int         m_Timer;
    bool        was_NetData;
    
    enum States {
        WAITING_UUID,
        WAITING_UPLOAD,
        WAITING_DOWNLOAD,
        FINISHED
    };
    States      m_State;

public:
    Connection(QObject *_parent, QTcpSocket *_clientConnection, Saver &_saver);
    ~Connection();

protected slots:
    void disconnected();
    void readyRead();
    bool onEmptyLine();

protected:
    void timerEvent( QTimerEvent * event );
    
    /// Process /get_updates request
    void processGetUpdates(Saver& _saver);
    /// Process /get_uuid request. Returns remote uuid
    QString processGetUuid(Saver& _saver);
    QString getRemoteUuid();
    /// Returns start of update interval for remote host
    time_t getRemoteLastUpdated();
    QStringMap getHeaders(const QStringList& _headers);
    
    /// \return true on empty line - i.e. false mean that not all headers were readed
    bool readHeaders(QTcpSocket *_sock, QStringList& _headers);
    /// \return bytes to read for body
    int readBody(QTcpSocket *_sock, QBuffer& _body, int _length);
    /// Clear request data
    void clear();
};

#endif
