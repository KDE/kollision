/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/


#include <KAboutData>

#include <QApplication>
#include <KLocalizedString>
#include <QCommandLineParser>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    KAboutData aboutData("kollision", i18n("Kollision"),
                         "0.1", i18n("KDE collision game"), KAboutLicense::GPL,
                         i18n("(c) 2007 Paolo Capriotti"), "http://games.kde.org/kollision");
    aboutData.addAuthor(i18n("Paolo Capriotti"), QString(), "p.capriotti@gmail.com");
    aboutData.addAuthor(i18n("Dmitry Suzdalev"), QString(), "dimsuz@gmail.com");
    aboutData.addCredit(i18n("Matteo Guarnieri"), i18n("Original idea"));
    aboutData.addCredit(i18n("Brian Croom"), i18n("Port to use KGameRenderer"));

    QApplication app(argc, argv);
    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);


    MainWindow* window = new MainWindow;
    window->show();
    
    return app.exec();
}
