/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtNetwork module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
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
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QHTTPMULTIPART_P_H
#define QHTTPMULTIPART_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the Network Access API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/qshareddata.h"
#include "qnetworkrequest_p.h" // for deriving QHttpPartPrivate from QNetworkHeadersPrivate
#include "private/qobject_p.h"

QT_BEGIN_NAMESPACE


class QHttpPartPrivate: public QSharedData, public QNetworkHeadersPrivate
{
public:
    inline QHttpPartPrivate() : bodyDevice(0), headerCreated(false), readPointer(0)
    {
    }
    ~QHttpPartPrivate()
    {
    }


    QHttpPartPrivate(const QHttpPartPrivate &other)
        : QSharedData(other), QNetworkHeadersPrivate(other), body(other.body),
        header(other.header), headerCreated(other.headerCreated), readPointer(other.readPointer)
    {
        bodyDevice = other.bodyDevice;
    }

    inline bool operator==(const QHttpPartPrivate &other) const
    {
        return rawHeaders == other.rawHeaders && body == other.body &&
                bodyDevice == other.bodyDevice && readPointer == other.readPointer;
    }

    void setBodyDevice(QIODevice *device) {
        bodyDevice = device;
        readPointer = 0;
    }
    void setBody(const QByteArray &newBody) {
        body = newBody;
        readPointer = 0;
    }

    // QIODevice-style methods called by QHttpMultiPartIODevice (but this class is
    // not a QIODevice):
    qint64 bytesAvailable() const;
    qint64 readData(char *data, qint64 maxSize);
    qint64 size() const;
    bool reset();

    QByteArray body;
    QIODevice *bodyDevice;

private:
    void checkHeaderCreated() const;

    mutable QByteArray header;
    mutable bool headerCreated;
    qint64 readPointer;
};



class QHttpMultiPartPrivate;

class Q_AUTOTEST_EXPORT QHttpMultiPartIODevice : public QIODevice
{
public:
    QHttpMultiPartIODevice(QHttpMultiPartPrivate *parentMultiPart) :
            QIODevice(), multiPart(parentMultiPart), readPointer(0), deviceSize(-1) {
    }

    ~QHttpMultiPartIODevice() {
    }

    virtual bool atEnd() const {
        return readPointer == size();
    }

    virtual qint64 bytesAvailable() const {
        return size() - readPointer;
    }

    virtual void close() {
        readPointer = 0;
        partOffsets.clear();
        deviceSize = -1;
        QIODevice::close();
    }

    virtual qint64 bytesToWrite() const {
        return 0;
    }

    virtual qint64 size() const;
    virtual bool isSequential() const;
    virtual bool reset();
    virtual qint64 readData(char *data, qint64 maxSize);
    virtual qint64 writeData(const char *data, qint64 maxSize);

    QHttpMultiPartPrivate *multiPart;
    qint64 readPointer;
    mutable QList<qint64> partOffsets;
    mutable qint64 deviceSize;
};



class QHttpMultiPartPrivate: public QObjectPrivate
{
public:

    QHttpMultiPartPrivate();

    ~QHttpMultiPartPrivate()
    {
        delete device;
    }

    QList<QHttpPart> parts;
    QByteArray boundary;
    QHttpMultiPart::ContentType contentType;
    QHttpMultiPartIODevice *device;

};

QT_END_NAMESPACE


#endif // QHTTPMULTIPART_P_H