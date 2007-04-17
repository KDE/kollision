/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef MAINAREA_H
#define MAINAREA_H

#include <QTimer>
#include <QTime>
#include <QList>
#include <kgamecanvas.h>
#include "animator.h"

class Renderer;
class Ball;
class Message;

class MainArea : public KGameCanvasWidget
{
Q_OBJECT
    QTimer m_timer;
    QTime m_time;
    QTime m_global_time;
    QTime m_game_time;
    QTime m_event_time;
    
    int m_size;
    Renderer* m_renderer;
    KGameCanvasPixmap* m_background;
    Animator m_animator;
    QFont m_msg_font;
    
    QList<Ball*> m_balls;
    QList<Ball*> m_fading;
    Ball* m_man;
    QPointF m_last_man_pos;
    bool m_death;
    
    double radius() const;    
    QPointF randomPoint() const;
    QPointF randomDirection(double val) const;
    
    Ball* addBall(const QString& id);
    bool collide(const QPointF& a, const QPointF& b, 
                double diam, struct Collision& collision);
    void writeMessage(const QString& text);
protected:
    virtual void mouseMoveEvent(QMouseEvent* event);
public:
    MainArea(QWidget* parent = 0);
    
    void start();
    
public slots:
    void tick();
};

#endif // MAINAREA_H

