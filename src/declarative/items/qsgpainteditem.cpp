// Commit: ac5c099cc3c5b8c7eec7a49fdeb8a21037230350
/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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

#include "qsgpainteditem_p.h"
#include "qsgpainteditem_p_p.h"
#include "private/qsgpainternode_p.h"

#include <private/qsgcontext_p.h>
#include <private/qsgadaptationlayer_p.h>

QT_BEGIN_NAMESPACE

QSGPaintedItemPrivate::QSGPaintedItemPrivate()
    : QSGItemPrivate()
    , geometryDirty(false)
    , contentsDirty(false)
    , opaquePainting(false)
{
}

QSGPaintedItem::QSGPaintedItem(QSGItem *parent)
    : QSGItem(*(new QSGPaintedItemPrivate), parent)
{
    setFlag(ItemHasContents);
}

QSGPaintedItem::QSGPaintedItem(QSGPaintedItemPrivate &dd, QSGItem *parent)
    : QSGItem(dd, parent)
{
    setFlag(ItemHasContents);
}

QSGPaintedItem::~QSGPaintedItem()
{
}

void QSGPaintedItem::update()
{
    Q_D(QSGPaintedItem);
    d->contentsDirty = true;
    QSGItem::update();
}

void QSGPaintedItem::update(const QRect &)
{
    Q_D(QSGPaintedItem);
    d->contentsDirty = true;
    QSGItem::update();
}

bool QSGPaintedItem::opaquePainting() const
{
    Q_D(const QSGPaintedItem);
    return d->opaquePainting;
}

void QSGPaintedItem::setOpaquePainting(bool o)
{
    Q_D(QSGPaintedItem);

    if (d->opaquePainting == o)
        return;

    d->opaquePainting = o;
    QSGItem::update();
}

QSize QSGPaintedItem::contentsSize() const
{
    // XXX todo
    return QSize();
}

void QSGPaintedItem::setContentsSize(const QSize &)
{
    // XXX todo
}

void QSGPaintedItem::resetContentsSize()
{
    // XXX todo
}

qreal QSGPaintedItem::contentsScale() const
{
    // XXX todo
    return 1;
}

void QSGPaintedItem::setContentsScale(qreal)
{
    // XXX todo
}

int QSGPaintedItem::pixelCacheSize() const
{
    // XXX todo
    return 0;
}

void QSGPaintedItem::setPixelCacheSize(int)
{
    // XXX todo
}

bool QSGPaintedItem::smoothCache() const
{
    // XXX todo
    return false;
}

void QSGPaintedItem::setSmoothCache(bool)
{
    // XXX todo
}

QColor QSGPaintedItem::fillColor() const
{
    // XXX todo
    return QColor();
}

void QSGPaintedItem::setFillColor(const QColor&)
{
    // XXX todo
}

void QSGPaintedItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_D(QSGPaintedItem);
    d->geometryDirty = true;
    QSGItem::geometryChanged(newGeometry, oldGeometry);
}

QSGNode *QSGPaintedItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    Q_UNUSED(data);
    Q_D(QSGPaintedItem);

    if (width() <= 0 || height() <= 0) {
        delete oldNode;
        return 0;
    }

    QSGPainterNode *node = static_cast<QSGPainterNode *>(oldNode);
    if (!node)
        node = new QSGPainterNode;

    node->setSize(QSize(d->width, d->height));
    node->setSmoothPainting(d->smooth);
    node->setLinearFiltering(d->smooth);
    node->setOpaquePainting(d->opaquePainting);
    node->update();

    if (d->contentsDirty || d->geometryDirty) {
        node->paint(this);
        d->contentsDirty = false;
        d->geometryDirty = false;
    }

    return node;
}

QT_END_NAMESPACE