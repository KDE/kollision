/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "message.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QPalette>
#include <QTextOption>
#include <QTextDocument>

Message::Message(const QString& text, const QFont& font, int maxwidth)
: QGraphicsTextItem(text)
, m_velocity(0.0, 0.0)
{
    qApp->installEventFilter(this);

    setFont(font);
    setDefaultTextColor(QApplication::palette().color(QPalette::WindowText));
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

bool Message::eventFilter(QObject *watched, QEvent *e)
{
    if (watched == scene() && e->type() == QEvent::PaletteChange) {
        setDefaultTextColor(scene()->palette().color(QPalette::WindowText));
    }
    return QGraphicsTextItem::eventFilter(watched, e);
}
