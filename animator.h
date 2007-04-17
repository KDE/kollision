/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "animation.h"
#include <QTimer>
#include <QTime>

class Animator : public AnimationGroup
{
Q_OBJECT
    QTimer m_timer;
    QTime m_time;
public:
    Animator();
    
    void startTimer();
    void stopTimer();
    void add(Animation* a);
public slots:
    void tick();
};

#endif // ANIMATOR_H

