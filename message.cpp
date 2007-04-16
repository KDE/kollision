/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "message.h"

Message::Message(KGameCanvasAbstract* parent, const QString& text, const QFont& font)
: SpriteMixin<KGameCanvasText>(parent)
{
    setFont(font);
    setText(text);
    setPositioning(KGameCanvasText::HCenter, 
                   KGameCanvasText::VCenter);
}


