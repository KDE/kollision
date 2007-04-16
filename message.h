/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <sprite.h>
#include <kgamecanvas.h>

class Message : public SpriteMixin<KGameCanvasText>
{
public:
    Message(KGameCanvasAbstract* parent, const QString& text, const QFont& font);
};

#endif // MESSAGE_H
