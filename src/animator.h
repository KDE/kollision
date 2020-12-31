/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "animation.h"
#include <QTimer>
#include <QElapsedTimer>

class Animator : public AnimationGroup
{
Q_OBJECT
public:
    Animator();
    
    void startTimer();
    void stopTimer();
    void add(Animation* a);
public Q_SLOTS:
    void tick();
private:
    QTimer m_timer;
    QElapsedTimer m_time;
};

#endif // ANIMATOR_H

