/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "message.h"
#include <QTextOption>
#include <QTextDocument>

Message::Message(const QString& text, const QFont& font, int maxwidth)
: QGraphicsTextItem(text)
, m_velocity(0.0, 0.0)
{
    setFont(font);
    setDefaultTextColor(Qt::black);
    setAcceptHoverEvents(false);
    document()->setTextWidth(maxwidth);
    QTextOption opts = document()->defaultTextOption();
    opts.setAlignment(Qt::AlignHCenter);
    document()->setDefaultTextOption(opts);
    
    // translate so that the origin is the center
    setTransform(QTransform::fromTranslate(-document()->size().width() / 2, -document()->size().height() / 2), true);
}

void Message::setOpacityF(qreal opacity)
{
    QGraphicsItem::setOpacity(opacity);
}

qreal Message::opacityF() const
{
    return QGraphicsItem::opacity();
}

void Message::setVelocity(const QPointF& vel)
{
    m_velocity = vel;
}

QPointF Message::velocity() const
{
    return m_velocity;
}

void Message::setPosition(const QPointF& pos)
{
    QGraphicsTextItem::setPos(pos);
}

QPointF Message::position() const
{
    return QGraphicsTextItem::pos();
}

int Message::height() const
{
    return document()->size().height();
}
