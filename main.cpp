/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/


#include <KAboutData>
#include <KCrash>

#include <QApplication>
#include <KLocalizedString>
#include <QCommandLineParser>
#include <kdelibs4configmigrator.h>
#include <KDBusService>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kollision");

    Kdelibs4ConfigMigrator migrate(QStringLiteral("kollision"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kollisionrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("kollisionui.rc"));
    migrate.migrate();

    KAboutData aboutData(QStringLiteral("kollision"), i18n("Kollision"),
                         QStringLiteral("0.3"), i18n("KDE collision game"), KAboutLicense::GPL,
                         i18n("(c) 2007 Paolo Capriotti"));
    aboutData.addAuthor(i18n("Paolo Capriotti"), QString(), QStringLiteral("p.capriotti@gmail.com"));
    aboutData.addAuthor(i18n("Dmitry Suzdalev"), QString(), QStringLiteral("dimsuz@gmail.com"));
    aboutData.addCredit(i18n("Matteo Guarnieri"), i18n("Original idea"));
    aboutData.addCredit(i18n("Brian Croom"), i18n("Port to use KGameRenderer"));
    aboutData.setHomepage(QStringLiteral("http://games.kde.org/kollision"));


    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kollision")));

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    KCrash::initialize();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    KDBusService service;

    MainWindow* window = new MainWindow;
    window->show();
    
    return app.exec();
}
