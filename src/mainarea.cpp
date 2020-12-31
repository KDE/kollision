/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>
    SPDX-FileCopyrightText: 2010 Brian Croom <brian.s.croom@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainarea.h"

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QAction>

#include <KgDifficulty>
#include <KgTheme>
#include <KLocalizedString>
#include <QStandardPaths>

#include "ball.h"
#include "kollisionconfig.h"

#include <cmath>
#include <stdio.h>

struct Collision
{
    double square_distance;
    QPointF line;
};

struct Theme : public KgTheme
{
    Theme() : KgTheme("pictures/theme.desktop")
    {
        setGraphicsPath(QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("pictures/theme.svgz")));
    }
};

MainArea::MainArea()
: m_renderer(new Theme)
, m_man(nullptr)
, m_manBallDiameter(28)
, m_ballDiameter(28)
, m_death(false)
, m_game_over(false)
, m_paused(false)
, m_pauseTime(0)
, m_penalty(0)
, m_soundHitWall(QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("sounds/hit_wall.ogg")))
, m_soundYouLose(QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("sounds/you_lose.ogg")))
, m_soundBallLeaving(QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("sounds/ball_leaving.ogg")))
, m_soundStart(QStandardPaths::locate(QStandardPaths::AppDataLocation, QStringLiteral("sounds/start.ogg")))
, m_pauseAction(nullptr)
, m_random(new QRandomGenerator(QRandomGenerator::global()->generate()))
{

    // Initialize the sound state
    enableSounds(KollisionConfig::enableSounds());

    increaseBallSize(KollisionConfig::increaseBallSize());

    m_size = 500;
    QRect rect(0, 0, m_size, m_size);
    setSceneRect(rect);

    m_timer.setInterval(20);
    connect(&m_timer, &QTimer::timeout, this, &MainArea::tick);

    m_msgFont = QApplication::font();
    m_msgFont.setPointSize(15);

    QPixmap pix(rect.size());
    {
        // draw gradient
        QPainter p(&pix);
        QColor color = palette().color(QPalette::Window);
        QLinearGradient grad(QPointF(0, 0), QPointF(0, height()));
        grad.setColorAt(0, color.lighter(115));
        grad.setColorAt(1, color.darker(115));
        p.fillRect(rect, grad);
    }
    setBackgroundBrush(pix);

    writeText(i18n("Welcome to Kollision\nClick to start a game"), false);

}

void MainArea::increaseBallSize(bool enable)
{
    m_increaseBallSize = enable;
    KollisionConfig::setIncreaseBallSize(enable);
    KollisionConfig::self()->save();
}

void MainArea::enableSounds(bool enabled)
{
    m_soundEnabled = enabled;
    KollisionConfig::setEnableSounds(enabled);
    KollisionConfig::self()->save();
}

Animation* MainArea::writeMessage(const QString& text)
{
    Message* message = new Message(text, m_msgFont, m_size);
    message->setPosition(QPointF(m_size, m_size) / 2.0);
    addItem(message);
    message->setOpacityF(0.0);

    SpritePtr sprite(message);

    AnimationGroup* move = new AnimationGroup;
    move->add(new FadeAnimation(sprite, 1.0, 0.0, 1500));
    move->add(new MovementAnimation(sprite, sprite->position(), QPointF(0, -0.1), 1500));
    AnimationSequence* sequence = new AnimationSequence;
    sequence->add(new PauseAnimation(200));
    sequence->add(new FadeAnimation(sprite, 0.0, 1.0, 1000));
    sequence->add(new PauseAnimation(500));
    sequence->add(move);

    m_animator.add(sequence);

    return sequence;
}

Animation* MainArea::writeText(const QString& text, bool fade)
{
    m_welcomeMsg.clear();
    const QStringList lines = text.split(QLatin1Char('\n'));
    for (const QString &line : lines) {
        m_welcomeMsg.append(
            QExplicitlySharedDataPointer<Message>(new Message(line, m_msgFont, m_size)));
    }
    displayMessages(m_welcomeMsg);

    if (fade) {
        AnimationGroup* anim = new AnimationGroup;
        for (const auto& message : qAsConst(m_welcomeMsg)) {
            message->setOpacityF(0.0);
            anim->add(new FadeAnimation(message, 0.0, 1.0, 1000));
        }

        m_animator.add(anim);

        return anim;
    }
    else {
        return nullptr;
    }
}

void MainArea::displayMessages(const QList<QExplicitlySharedDataPointer<Message> >& messages)
{
    int totalHeight = 0;
    for (const auto& message : messages) {
      totalHeight += message->height();
    }
    QPointF pos(m_size / 2.0, (m_size - totalHeight) / 2.0);

    for (int i = 0; i < messages.size(); i++) {
        QExplicitlySharedDataPointer<Message> msg = messages[i];
        int halfHeight = msg->height() / 2;
        pos.ry() += halfHeight;
        msg->setPosition(pos);
        msg->setZValue(10.0);
        msg->show();
        addItem(msg.data());
        pos.ry() += halfHeight;
    }
}

double MainArea::radius() const
{
    return m_ballDiameter / 2.0;
}

void MainArea::setBallDiameter(int val)
{
    // Limits other balls' maximum diameter to the double of man ball's diameter.
    if (m_ballDiameter < m_manBallDiameter * 2) {
        m_ballDiameter = val;
    }
}

void MainArea::togglePause()
{
    if (!m_man) return;

    if (m_paused) {
        m_paused = false;
        m_timer.start();
        m_welcomeMsg.clear();

        m_pauseTime += m_time.elapsed() - m_lastTime;
        m_lastTime = m_time.elapsed();
    }
    else {
        m_paused = true;
        m_timer.stop();
        QString shortcut = m_pauseAction ?
          m_pauseAction->shortcut().toString() :
          QStringLiteral("P");
        writeText(i18n("Game paused\nClick or press %1 to resume", shortcut), false);

        if(m_lastGameTime >= 5) {
            m_penalty += 5000;
            m_lastGameTime -= 5;
        }
        else {
            m_penalty += m_lastGameTime * 1000;
            m_lastGameTime = 0;
        }

        Q_EMIT changeGameTime(m_lastGameTime);
    }

    m_man->setVisible(!m_paused);
    for (Ball* ball : qAsConst(m_balls)) {
        ball->setVisible(!m_paused);
    }
    for (Ball* ball : qAsConst(m_fading)) {
        ball->setVisible(!m_paused);
    }

    Q_EMIT pause(m_paused);
}

void MainArea::start()
{
    // reset ball size
    m_ballDiameter = m_manBallDiameter;
    m_man->setRenderSize(QSize(m_manBallDiameter, m_manBallDiameter));

    m_death = false;
    m_game_over = false;

    switch (Kg::difficultyLevel()) {
    case KgDifficultyLevel::Easy:
        m_ball_timeout = 30;
        break;
    case KgDifficultyLevel::Medium:
        m_ball_timeout = 25;
        break;
    case KgDifficultyLevel::Hard:
    default:
        m_ball_timeout = 20;
        break;
    }

    m_welcomeMsg.clear();

    addBall(QStringLiteral("red_ball"));
    addBall(QStringLiteral("red_ball"));
    addBall(QStringLiteral("red_ball"));
    addBall(QStringLiteral("red_ball"));

    m_pauseTime = 0;
    m_penalty = 0;
    m_time.restart();
    m_lastTime = 0;
    m_lastGameTime = 0;

    m_timer.start();

    writeMessage(i18np("%1 ball", "%1 balls", 4));

    Q_EMIT changeGameTime(0);
    Q_EMIT starting();

    if(m_soundEnabled)
        m_soundStart.start();
}

void MainArea::setPauseAction(QAction * action)
{
  m_pauseAction = action;
}

QPointF MainArea::randomPoint() const
{
    const double x = m_random->bounded(m_size - radius() * 2) + radius();
    const double y = m_random->bounded(m_size - radius() * 2) + radius();
    return QPointF(x, y);
}

QPointF MainArea::randomDirection(double val) const
{
    const double angle = m_random->bounded(2 * M_PI);
    return QPointF(val * sin(angle), val * cos(angle));
}

Ball* MainArea::addBall(const QString& id)
{
    QPoint pos;
    for (bool done = false; !done; ) {
        Collision tmp;

        done = true;
        pos = randomPoint().toPoint();
        for (Ball* ball : qAsConst(m_fading)) {
            if (collide(pos, ball->position(), m_ballDiameter, m_ballDiameter, tmp)) {
                done = false;
                break;
            }
        }
    }

    Ball* ball = new Ball(&m_renderer, id, static_cast<int>(radius()*2));
    ball->setPosition(pos);
    addItem(ball);

    // speed depends of game difficulty
    double speed;
    switch (Kg::difficultyLevel()) {
    case KgDifficultyLevel::Easy:
        speed = 0.2;
        break;
    case KgDifficultyLevel::Medium:
        speed = 0.28;
        break;
    case KgDifficultyLevel::Hard:
    default:
        speed = 0.4;
        break;
    }
    ball->setVelocity(randomDirection(speed));

    ball->setOpacityF(0.0);
    ball->show();
    m_fading.push_back(ball);

    // update statusbar
    Q_EMIT changeBallNumber(m_balls.size() + m_fading.size());

    return ball;
}

bool MainArea::collide(const QPointF& a, const QPointF& b, double diamA, double diamB, Collision& collision)
{
    collision.line = b - a;
    collision.square_distance = collision.line.x() * collision.line.x()
                              + collision.line.y() * collision.line.y();

    return collision.square_distance <= diamA * diamB;
}

void MainArea::abort()
{
    if (m_man) {
        if (m_paused) {
            togglePause();
        }
        m_death = true;

        m_man->setVelocity(QPointF(0, 0));
        m_balls.push_back(m_man);
        m_man = nullptr;
        Q_EMIT changeState(false);

        for (Ball* fball : qAsConst(m_fading)) {
            fball->setOpacityF(1.0);
            fball->setVelocity(QPointF(0.0, 0.0));
            m_balls.push_back(fball);
        }
        m_fading.clear();
    }
}

void MainArea::tick()
{
    if (!m_death && m_man && !m_paused) {
        setManPosition(views().first()->mapFromGlobal(QCursor().pos()));
    }

    int t = m_time.elapsed() - m_lastTime;
    m_lastTime = m_time.elapsed();

    // compute game time && update statusbar
    if ((m_time.elapsed() - m_pauseTime - m_penalty) / 1000 > m_lastGameTime) {
        m_lastGameTime = (m_time.elapsed() - m_pauseTime - m_penalty) / 1000;
        Q_EMIT changeGameTime(m_lastGameTime);
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
    for (Ball* ball : qAsConst(m_balls)) {
        if (m_man && collide(
                ball->position(),
                m_man->position(),
                m_ballDiameter,
                m_manBallDiameter,
                collision)) {
            if(m_soundEnabled)
                m_soundYouLose.start();
            abort();
            break;
        }
    }

    // integrate
    for (Ball* ball : qAsConst(m_balls)) {
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
        if (hit_wall && m_soundEnabled) {
            m_soundHitWall.start();
        }

        // handle collisions with next balls
        for (int j = i + 1; j < m_balls.size(); j++) {
            Ball* other = m_balls[j];

            QPointF other_pos = other->position();

            if (collide(pos, other_pos, m_ballDiameter, m_ballDiameter, collision)) {
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
            if(m_soundEnabled)
                m_soundBallLeaving.start();
            delete ball;
            it = m_balls.erase(it);
        }
        else {
            ++it;
        }
    }

    if (!m_death && m_time.elapsed() - m_pauseTime >= m_ball_timeout * 1000 *
                                                       (m_balls.size() + m_fading.size() - 3)) {
        if (m_increaseBallSize) {
            //increase ball size by 4 units
            setBallDiameter(m_ballDiameter + 4);
            for (Ball* ball : qAsConst(m_balls)) {
                ball->setRenderSize(QSize(m_ballDiameter, m_ballDiameter));
            }
        }

        addBall(QStringLiteral("red_ball"));
        writeMessage(i18np("%1 ball", "%1 balls", m_balls.size() + 1));
    }

    if (m_death && m_balls.isEmpty() && m_fading.isEmpty()) {
        m_game_over = true;
        m_timer.stop();
        int time = (m_time.restart() - m_pauseTime - m_penalty) / 1000;
        QString text = i18np(
            "GAME OVER\n"
            "You survived for %1 second\n"
            "Click to restart",
            "GAME OVER\n"
            "You survived for %1 seconds\n"
            "Click to restart", time);
        Q_EMIT gameOver(time);
        Animation* a = writeText(text);
        connect(this, &MainArea::starting, a, &Animation::stop);
    }
}

void MainArea::setManPosition(const QPointF& p)
{
    Q_ASSERT(m_man);

    QPointF pos = p;

    if (pos.x() <= radius()) pos.setX(static_cast<int>(radius()));
    if (pos.x() >= m_size - radius()) pos.setX(m_size - static_cast<int>(radius()));
    if (pos.y() <= radius()) pos.setY(static_cast<int>(radius()));
    if (pos.y() >= m_size - radius()) pos.setY(m_size - static_cast<int>(radius()));

    m_man->setPosition(pos);
}

void MainArea::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    if (!m_death || m_game_over) {
        if (m_paused) {
            togglePause();
            setManPosition(e->scenePos());
        }
        else if (!m_man) {
            m_man = new Ball(&m_renderer, QStringLiteral("blue_ball"), static_cast<int>(radius()*2));
            m_man->setZValue(1.0);
            setManPosition(e->scenePos());
            addItem(m_man);

            start();
            Q_EMIT changeState(true);
        }
    }
}

void MainArea::focusOutEvent(QFocusEvent*)
{
    if (!m_paused) {
        togglePause();
    }
}

