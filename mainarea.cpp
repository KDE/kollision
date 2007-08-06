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
#include "kollisionconfig.h"

#include <kdebug.h>
#include <klocalizedstring.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <phonon/audioplayer.h>
#include <QGraphicsSceneMouseEvent>

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
    setSceneRect(0, 0, m_size, m_size);

    srand(time(0));

    m_renderer = new Renderer;
    m_renderer->resize(QSize(28, 28));

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));

    m_welcome_msg = new QGraphicsTextItem;
    QFont font = m_welcome_msg->font();
    font.setPointSize(15);
    m_welcome_msg->setFont(font);
    m_welcome_msg->setHtml( i18n("Welcome to Kollision<br>Click to start a game") );
    addItem(m_welcome_msg);
    m_welcome_msg->setPos( m_size/2 - m_welcome_msg->boundingRect().width()/2,
                           m_size/2 - m_welcome_msg->boundingRect().height()/2 );


    QImage tmp(m_size, m_size, QImage::Format_ARGB32_Premultiplied);
    {
        // draw gradient
        QPainter p(&tmp);
        QLinearGradient grad(QPointF(0, 0), QPointF(0, height()));
        grad.setColorAt(0, QColor(240, 240, 240));
        grad.setColorAt(1, QColor(180, 180, 180));
        p.fillRect(QRect(0, 0, m_size, m_size), grad);
    }
    m_background = QPixmap::fromImage(tmp);

    // setup audio player
    m_player = 0;
//    enableSounds();
}

void MainArea::enableSounds()
{
    if (KollisionConfig::enableSounds()) {
        if (!m_player) {
            m_player = new Phonon::AudioPlayer(Phonon::GameCategory);
            m_player->load(KStandardDirs::locate("appdata", "sounds/") + "/collision.wav");
        }
    }
    else {
        delete m_player;
        m_player = 0;
    }
}
/* TODO msg
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
    m_welcome_msg.clear();
    foreach (QString text, lines) {
        m_welcome_msg.append(
            KSharedPtr<Message>(new Message(this, text, m_msg_font)));
    }
    // TODO msg just use setPos to center it
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
*/

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

    /* TODO msg
    m_welcome_msg.clear();
    */

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

    /* TODO msg
    writeMessage(i18n("4 balls"));
    */

    emit changeGameTime(0);
    emit starting();
}

QPointF MainArea::randomPoint() const
{
    double x = (double)rand() * (m_size - radius() * 2) / RAND_MAX;
    double y = (double)rand() * (m_size - radius() * 2) / RAND_MAX;
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
            if (collide(pos, ball->pos(), radius() * 2.0, tmp)) {
                done = false;
                break;
            }
        }
    }

    Ball* ball = new Ball;
    ball->setPos(pos);
    ball->setPixmap(m_renderer->render(id));
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

    ball->setOpacity(0.0);
    ball->show();
    m_fading.push_back(ball);

    // update statusbar
    emit changeBallNumber(m_balls.size() + m_fading.size());

    return ball;
}

bool MainArea::collide(const QPointF& a, const QPointF& b, double diam, Collision& collision)
{
    QPointF aCenter = QPointF(a.x()+radius(), a.y()+radius());
    QPointF bCenter = QPointF(b.x()+radius(), b.y()+radius());
    collision.line = bCenter - aCenter;
    collision.square_distance = collision.line.x() * collision.line.x()
                              + collision.line.y() * collision.line.y();
    return collision.square_distance <= diam * diam;
}

void MainArea::onCollision()
{
    if (m_player) {
        m_player->play();
    }
}

void MainArea::abort()
{
    if (m_man) {
        m_death = true;

        m_man->setVelocity(QPointF(0, 0));
        m_balls.push_back(m_man);
        m_man = 0;
        //setCursor(QCursor()); TODO cursor

        foreach (Ball* fball, m_fading) {
            fball->setOpacity(1.0);
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
        (*it)->setOpacity((*it)->opacity() + t * 0.0005);
        if ((*it)->opacity() >= 1.0) {
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
                ball->pos(),
                m_man->pos(),
                radius() * 2, collision)) {
            onCollision();
            abort();
            break;
        }
    }

    // integrate
    foreach (Ball* ball, m_balls) {
        // position
        ball->setPos(ball->pos() +
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
        QPointF pos = ball->pos();

        // handle collisions with borders
        if (pos.x() <= 0) {
            vel.setX(fabs(vel.x()));
            pos.setX(0);
        }
        if (pos.x() >= m_size - 2*radius()) {
            vel.setX(-fabs(vel.x()));
            pos.setX(m_size - 2*radius());
        }
        if (pos.y() <= 0) {
            vel.setY(fabs(vel.y()));
            pos.setY(0);
        }
        if (!m_death) {
            if (pos.y() >= m_size - 2*radius()) {
                vel.setY(-fabs(vel.y()));
                pos.setY(m_size - 2*radius());
            }
        }

        // handle collisions with next balls
        for (int j = i + 1; j < m_balls.size(); j++) {
            Ball* other = m_balls[j];

            QPointF other_pos = other->pos();

            if (collide(pos, other_pos, radius() * 2, collision)) {
                onCollision();
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
                other->setPos(other_pos + adj);
            }

        }

        ball->setPos(pos);
        ball->setVelocity(vel);
    }

    for (QList<Ball*>::iterator it = m_balls.begin();
         it != m_balls.end(); ) {
        Ball* ball = *it;
        QPointF pos = ball->pos();

        if (m_death && pos.y() >= height() + radius() + 10) {
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
        /* TODO msg
        writeMessage(i18n("%1 balls", m_balls.size() + 1));
        */
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
        /* TODO msg
        Animation* a = writeText(text);
        connect(this, SIGNAL(starting()), a, SLOT(stop()));
        */
    }
}


void MainArea::setManPosition(const QPointF& p)
{
    Q_ASSERT(m_man);

    QPointF pos = p;

    if (pos.x() <= 0) pos.setX(0);
    if (pos.x() >= m_size - radius()*2) pos.setX(m_size - radius()*2);
    if (pos.y() <= 0) pos.setY(0);
    if (pos.y() >= m_size - radius()*2) pos.setY(m_size - radius()*2);

    m_man->setPos(pos);
}

void MainArea::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    if (!m_death || m_game_over) {
        if (!m_man) {
            m_man = new Ball;
            m_man->setPixmap(m_renderer->render("blue_ball"));
            setManPosition(e->scenePos()-QPointF(radius(),radius()));
            addItem(m_man);
            m_man->show();

            m_event_time.restart();
            start();
            /* TODO cursor
            setCursor(Qt::BlankCursor);
            */
        }
    }
}

void MainArea::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
    if (!m_death && m_man) {
        setManPosition(e->scenePos()-QPointF(radius(),radius()));
    }
}


void MainArea::drawBackground(QPainter* p, const QRectF&)
{
    p->drawPixmap(0, 0, m_background);
}

#include "mainarea.moc"


