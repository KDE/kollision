/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "mainarea.h"

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QPainter>

#include <KDebug>
#include <KLocalizedString>
#include <KMessageBox>
#include <KStandardDirs>
#include <Phonon/MediaObject>

#include "renderer.h"
#include "ball.h"
#include "kollisionconfig.h"

// for rand
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

struct Collision
{
    double square_distance;
    QPointF line;
};

MainArea::MainArea()
: m_man(0)
, m_death(false)
, m_game_over(false)
{
    m_size = 500;
    QRect rect(0, 0, m_size, m_size);
    setSceneRect(rect);
    
    srand(time(0));
    
    m_renderer = new Renderer;
    m_renderer->resize(QSize(28, 28));
    
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    
    m_msg_font = QApplication::font();
    m_msg_font.setPointSize(15);
    
    QImage tmp(rect.size(), QImage::Format_ARGB32_Premultiplied);
    {
        // draw gradient
        QPainter p(&tmp);
        QLinearGradient grad(QPointF(0, 0), QPointF(0, height()));
        grad.setColorAt(0, QColor(240, 240, 240));
        grad.setColorAt(1, QColor(180, 180, 180));
        p.fillRect(rect, grad);
    }
    m_background = QPixmap::fromImage(tmp);
    
    m_welcome_msg.append(MessagePtr(
        new Message(i18n("Welcome to Kollision"), m_msg_font)));
    m_welcome_msg.append(MessagePtr(
        new Message(i18n("Click to start a game"), m_msg_font)));
    displayMessages(m_welcome_msg);
    
    // setup audio player
    enableSounds();
}

void MainArea::enableSounds()
{
    m_player.setActive(KollisionConfig::enableSounds());
}

Animation* MainArea::writeMessage(const QString& text)
{
    Message* message = new Message(text, m_msg_font);
    message->setPosition(QPointF(m_size, m_size) / 2.0);
    addItem(message);
    message->setOpacityF(0.0);
    
    SpritePtr sprite(message);
    
    AnimationGroup* move = new AnimationGroup;
    move->add(new FadeAnimation(sprite, 1.0, 0.0, 1500));
    move->add(new MovementAnimation(sprite, 
            sprite->position(),
            QPointF(0, -0.1),
            1500));
    AnimationSequence* sequence = new AnimationSequence;
    sequence->add(new PauseAnimation(200));
    sequence->add(new FadeAnimation(sprite, 0.0, 1.0, 1000));
    sequence->add(new PauseAnimation(500));
    sequence->add(move);
    
    m_animator.add(sequence);
    
    return sequence;
}

Animation* MainArea::writeText(const QStringList& lines)
{
    m_welcome_msg.clear();
    foreach (QString text, lines) {
        m_welcome_msg.append(
            KSharedPtr<Message>(new Message(text, m_msg_font)));
    }
    displayMessages(m_welcome_msg);
        
    AnimationGroup* anim = new AnimationGroup;
    foreach (KSharedPtr<Message> message, m_welcome_msg) {
        message->setOpacityF(0.0);
        anim->add(new FadeAnimation(
            SpritePtr::staticCast(message), 0.0, 1.0, 1000));
    }
    
    m_animator.add(anim);
    
    return anim;
}

void MainArea::displayMessages(const QList<KSharedPtr<Message> >& messages)
{
    const int step = 45;
    QPointF pos(m_size / 2.0, (m_size - step * messages.size()) / 2.0);
    
    for (int i = 0; i < messages.size(); i++) {
        KSharedPtr<Message> msg = messages[i];
        msg->setPosition(pos);
        addItem(msg.data());
        
        pos.ry() += step;
    }
}

double MainArea::radius() const
{
    return m_renderer->size().width() / 2.0;
}

void MainArea::start()
{
    m_death = false;
    m_game_over = false;

    switch (KollisionConfig::gameDifficulty()) {
    case 0:
        m_ball_timeout = 30;
        break;
    case 1:
        m_ball_timeout = 25;
        break;
    case 2:
    default:
        m_ball_timeout = 20;
        break;
    }

    m_welcome_msg.clear();

    addBall("red_ball");
    addBall("red_ball");
    addBall("red_ball");
    addBall("red_ball");
    
    m_time.restart();
    m_global_time.restart();
    m_last_time = 0;
    m_last_game_time = 0;
    
    int timeout = (100 - KollisionConfig::smoothness()) / 5;
    m_timer.start(timeout);
    
    writeMessage(i18n("4 balls"));
    
    emit changeGameTime(0);
    emit starting();
    m_player.play(AudioPlayer::START);
}

QPointF MainArea::randomPoint() const
{
    double x = (double)rand() * (m_size - radius() * 2) / RAND_MAX + radius();
    double y = (double)rand() * (m_size - radius() * 2) / RAND_MAX + radius();
    return QPointF(x, y);
}

QPointF MainArea::randomDirection(double val) const
{
    double angle = (double)rand() * 2 * M_PI / RAND_MAX;
    return QPointF(val * sin(angle), val * cos(angle));
}

Ball* MainArea::addBall(const QString& id)
{
    QPoint pos;
    for (bool done = false; !done; ) {
        Collision tmp;
        
        done = true;
        pos = randomPoint().toPoint();
        foreach (Ball* ball, m_fading) {
            if (collide(pos, ball->position(), ball->radius() * 2.0, tmp)) {
                done = false;
                break;
            }
        }
    }

    Ball* ball = new Ball(m_renderer, id);
    ball->setPosition(pos);
    addItem(ball);
    
    // speed depends of game difficulty
    double speed;
    switch (KollisionConfig::gameDifficulty())
    {
    case 0:
        speed = 0.2;
        break;
    case 1:
        speed = 0.28;
        break;
    case 2:
    default:
        speed = 0.4;
        break;
    }
    ball->setVelocity(randomDirection(speed));
    
    ball->setOpacityF(0.0);
    ball->show();
    m_fading.push_back(ball);
    
    // update statusbar
    emit changeBallNumber(m_balls.size() + m_fading.size());
    
    return ball;
}

bool MainArea::collide(const QPointF& a, const QPointF& b, double diam, Collision& collision)
{
    collision.line = b - a;
    collision.square_distance = collision.line.x() * collision.line.x()
                              + collision.line.y() * collision.line.y();
    return collision.square_distance <= diam * diam;
}

void MainArea::abort()
{
    if (m_man) {
        m_death = true;
        
        m_man->setVelocity(QPointF(0, 0));
        m_balls.push_back(m_man);
        m_man = 0;
        emit showCursor(true);
        
        foreach (Ball* fball, m_fading) {
            fball->setOpacityF(1.0);
            fball->setVelocity(QPointF(0.0, 0.0));
            m_balls.push_back(fball);
        }
        m_fading.clear();
    }
}

void MainArea::tick()
{
    int t = m_time.elapsed() - m_last_time;
    m_last_time = m_time.elapsed();

    // compute game time && update statusbar
    if (m_time.elapsed() / 1000 > m_last_game_time) {
        m_last_game_time = m_time.elapsed() / 1000;
        emit changeGameTime(m_last_game_time);
    }
    
    Collision collision;
    
    // handle fade in
    for (QList<Ball*>::iterator it = m_fading.begin();
        it != m_fading.end(); ) {
        (*it)->setOpacityF((*it)->opacityF() + t * 0.0005);
        if ((*it)->opacityF() >= 1.0) {
            m_balls.push_back(*it);
            it = m_fading.erase(it);
        }
        else {
            ++it;
        }
    }
    
    // handle deadly collisions
    foreach (Ball* ball, m_balls) {
        if (m_man && collide(
                ball->position(),
                m_man->position(), 
                radius() * 2, collision)) {
            m_player.play(AudioPlayer::YOU_LOSE);
            abort();            
            break;
        }
    }
    
    // integrate
    foreach (Ball* ball, m_balls) {
        // position
        ball->setPosition(ball->position() +
            ball->velocity() * t);
            
        // velocity
        if (m_death) {
            ball->setVelocity(ball->velocity() +
                QPointF(0, 0.001) * t);
        }
    }
    
    for (int i = 0; i < m_balls.size(); i++) {
        Ball* ball = m_balls[i];
        
        QPointF vel = ball->velocity();
        QPointF pos = ball->position();
       
        // handle collisions with borders
        bool hit_wall = false;
        if (pos.x() <= radius()) {
            vel.setX(fabs(vel.x()));
            pos.setX(2 * radius() - pos.x());
            hit_wall = true;
        }
        if (pos.x() >= m_size - radius()) {
            vel.setX(-fabs(vel.x()));
            pos.setX(2 * (m_size - radius()) - pos.x());
            hit_wall = true;
        }
        if (pos.y() <= radius()) {
            vel.setY(fabs(vel.y()));
            pos.setY(2 * radius() - pos.y());
            hit_wall = true;
        }
        if (!m_death) {
            if (pos.y() >= m_size - radius()) {
                vel.setY(-fabs(vel.y()));
                pos.setY(2 * (m_size - radius()) - pos.y());
                hit_wall = true;
            }
        }
        if (hit_wall) {
            m_player.play(AudioPlayer::HIT_WALL);
        }

        // handle collisions with next balls
        for (int j = i + 1; j < m_balls.size(); j++) {
            Ball* other = m_balls[j];
            
            QPointF other_pos = other->position();
            
            if (collide(pos, other_pos, radius() * 2, collision)) {
//                 onCollision();
                QPointF other_vel = other->velocity();
                
                // compute the parallel component of the
                // velocity with respect to the collision line
                double v_par = vel.x() * collision.line.x() 
                             + vel.y() * collision.line.y();
                double w_par = other_vel.x() * collision.line.x()
                             + other_vel.y() * collision.line.y();
                             
                // swap those components
                QPointF drift = collision.line * (w_par - v_par) /
                                    collision.square_distance;
                vel += drift;
                other->setVelocity(other_vel - drift);
                
                // adjust positions, reflecting along the collision
                // line as much as the amount of compenetration
                QPointF adj = collision.line * 
                    (2.0 * radius() / 
                    sqrt(collision.square_distance) 
                        - 1);
                pos -= adj;
                other->setPosition(other_pos + adj);
            }
            
        }
        
        ball->setPosition(pos);
        ball->setVelocity(vel);
    }
    
    for (QList<Ball*>::iterator it = m_balls.begin();
         it != m_balls.end(); ) {
        Ball* ball = *it;
        QPointF pos = ball->position();
        
        if (m_death && pos.y() >= height() + radius() + 10) {
            m_player.play(AudioPlayer::BALL_LEAVING);
            delete ball;
            it = m_balls.erase(it);
        }
        else {
            ++it;
        }
    }
    
    if (!m_death && m_global_time.elapsed() >= m_ball_timeout * 1000) {
        m_global_time.restart();
        
        addBall("red_ball");
        writeMessage(i18n("%1 balls", m_balls.size() + 1));
    }
    
    if (m_death && m_balls.isEmpty() && m_fading.isEmpty()) {
        m_game_over = true;
        m_timer.stop();
        int time = m_time.restart() / 1000;
        QStringList text;
        text << i18n("GAME OVER")
             << i18np("You survived for %1 second", "You survived for %1 seconds", time)
             << i18n("Click to restart");
        emit gameOver(time);
        Animation* a = writeText(text);
        connect(this, SIGNAL(starting()), a, SLOT(stop()));
    }
}


void MainArea::setManPosition(const QPointF& p)
{
    Q_ASSERT(m_man);
    
    QPointF pos = p;
    
    if (pos.x() <= radius()) pos.setX((int) radius());
    if (pos.x() >= m_size - radius()) pos.setX(m_size - (int)radius());
    if (pos.y() <= radius()) pos.setY((int) radius());
    if (pos.y() >= m_size - radius()) pos.setY(m_size - (int) radius());
    
    m_man->setPosition(pos);
}

void MainArea::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    if (!m_death || m_game_over) {
        if (!m_man) {
            m_man = new Ball(m_renderer, "blue_ball");
            setManPosition(e->scenePos());            
            addItem(m_man);
            
            m_event_time.restart();
            start();
            emit showCursor(false);
        }
    }
}

void MainArea::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    if (!m_death && m_man) {
        setManPosition(e->scenePos());
    }
}

void MainArea::drawBackground(QPainter* painter, const QRectF& rect)
{
    painter->drawPixmap(rect, m_background, rect);
}

