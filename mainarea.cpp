/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "mainarea.h"
#include "renderer.h"
#include "ball.h"
#include "message.h"

#include <kdebug.h>
#include <klocalizedstring.h>
#include <kmessagebox.h>
#include <QMouseEvent>

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

MainArea::MainArea(QWidget* parent)
: KGameCanvasWidget(parent)
, m_man(0)
, m_death(false)
{
    m_background = new KGameCanvasPixmap(this);
    m_background->show();
    
    srand(time(0));
    
    m_renderer = new Renderer;
    m_renderer->resize(QSize(28, 28));
    
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    
    m_msg_font = font();
    m_msg_font.setPointSize(15);
    
    setMouseTracking(true);
    m_size = 500;
    setMinimumSize(QSize(m_size, m_size));
    setMaximumSize(QSize(m_size, m_size));
    
    
    QImage tmp(size(), QImage::Format_ARGB32_Premultiplied);
    {
        // draw gradient
        QPainter p(&tmp);
        QLinearGradient grad(QPointF(0, 0), QPointF(0, height()));
        grad.setColorAt(0, QColor(240, 240, 240));
        grad.setColorAt(1, QColor(180, 180, 180));
        p.fillRect(QRect(QPoint(0, 0), size()), grad);
    }
    m_background->setPixmap(QPixmap::fromImage(tmp));
    
    setCursor(Qt::BlankCursor);
}

Animation* MainArea::writeMessage(const QString& text)
{
    Message* message = new Message(this, text, m_msg_font);
    message->setPosition(QPointF(m_size, m_size) / 2.0);
    message->show();
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
    const int step = 45;
    QPointF pos(m_size / 2.0, (m_size - step * lines.size()) / 2.0);
    
    AnimationGroup* in = new AnimationGroup;
    AnimationGroup* out = new AnimationGroup;
    for (int i = 0; i < lines.size(); i++) {
        Message* message = new Message(this, lines[i], m_msg_font);
        message->setPosition(pos);
        message->show();
        message->setOpacityF(0.0);
        
        SpritePtr sprite(message);
        
        in->add(new FadeAnimation(sprite, 0.0, 1.0, 1000));
        out->add(new FadeAnimation(sprite, 1.0, 0.0, 1000));
        
        pos.ry() += step;
    }
    
    AnimationSequence* sequence = new AnimationSequence;
    sequence->add(in);
    sequence->add(new PauseAnimation(3000));
    sequence->add(out);
    
    m_animator.add(sequence);
    
    return sequence;
}


double MainArea::radius() const
{
    return m_renderer->size().width() / 2.0;
}

void MainArea::start()
{
    addBall("red_ball");
    addBall("red_ball");
    addBall("red_ball");
    addBall("red_ball");
    
    m_time.restart();
    m_global_time.restart();
    m_last_time = 0;
    m_timer.start(0);
    
    writeMessage(i18n("4 balls"));
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

    Ball* ball = new Ball(this, m_renderer, id);
    ball->setPosition(pos);
    ball->setVelocity(randomDirection(0.38));
    ball->setOpacityF(0.0);
    ball->show();
    m_fading.push_back(ball);
    
    return ball;
}

bool MainArea::collide(const QPointF& a, const QPointF& b, double diam, Collision& collision)
{
    collision.line = b - a;
    collision.square_distance = collision.line.x() * collision.line.x()
                              + collision.line.y() * collision.line.y();
    return collision.square_distance <= diam * diam;
}

void MainArea::tick()
{
    int t = m_time.elapsed() - m_last_time;
    m_last_time = m_time.elapsed();
    
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
            m_death = true;
            
            m_man->setVelocity(QPointF(0, 0));
            m_balls.push_back(m_man);
            m_man = 0;
            
            foreach (Ball* fball, m_fading) {
                fball->setOpacityF(1.0);
                fball->setVelocity(QPointF(0.0, 0.0));
                m_balls.push_back(fball);
            }
            m_fading.clear();
            
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
        if (pos.x() <= radius()) {
            vel.setX(fabs(vel.x()));
            pos.setX(2 * radius() - pos.x());
        }
        if (pos.x() >= m_size - radius()) {
            vel.setX(-fabs(vel.x()));
            pos.setX(2 * (m_size - radius()) - pos.x());
        }
        if (pos.y() <= radius()) {
            vel.setY(fabs(vel.y()));
            pos.setY(2 * radius() - pos.y());
        }
        if (!m_death) {
            if (pos.y() >= m_size - radius()) {
                vel.setY(-fabs(vel.y()));
                pos.setY(2 * (m_size - radius()) - pos.y());
            }
        }

        // handle collisions with next balls
        for (int j = i + 1; j < m_balls.size(); j++) {
            Ball* other = m_balls[j];
            
            QPointF other_pos = other->position();
            
            if (collide(pos, other_pos, radius() * 2, collision)) {
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
            delete ball;
            it = m_balls.erase(it);
        }
        else {
            ++it;
        }
    }
    
    if (!m_death && m_global_time.elapsed() >= 20 * 1000) {
        m_global_time.restart();
        
        addBall("red_ball");
        writeMessage(i18n("%1 balls", m_balls.size() + 1));
    }
    
    if (m_death && m_balls.isEmpty() && m_fading.isEmpty()) {
        m_timer.stop();
        QStringList text;
        text << i18n("GAME OVER")
             << i18n("You survived for %1 seconds", m_time.restart() / 1000);
        Animation* a = writeText(text);
        connect(a, SIGNAL(over()), this, SLOT(reset()));
    }
}

void MainArea::reset()
{
    m_death = false;
}


void MainArea::mouseMoveEvent(QMouseEvent* e)
{
    if (!m_death) {
        bool new_man = false;
        if (!m_man) {
            m_man = new Ball(this, m_renderer, "blue_ball");
            m_man->show();
            new_man = true;
            
            m_event_time.restart();
            start();
        }
        
        QPoint pos = e->pos();
        if (pos.x() <= radius()) pos.setX((int) radius());
        if (pos.x() >= m_size - radius()) pos.setX(m_size - (int)radius());
        if (pos.y() <= radius()) pos.setY((int) radius());
        if (pos.y() >= m_size - radius()) pos.setY(m_size - (int) radius());
        
        m_man->setPosition(pos);
        
        if (new_man) {
            m_man->setVelocity(QPointF(0, 0));
            m_last_man_pos = m_man->position();
        }
        else {
            int delta_t = m_event_time.elapsed();
            if (delta_t > 5) {
                m_man->setVelocity((m_man->position() - m_last_man_pos) / delta_t);   
                m_last_man_pos = m_man->position();             
                m_event_time.restart();
            }

        }
        
    }

}


#include "mainarea.moc"


