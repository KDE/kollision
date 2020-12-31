/*
    SPDX-FileCopyrightText: 2007 Paolo Capriotti <p.capriotti@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/


#include <KAboutData>
#include <KCrash>

#include <QApplication>
#include <KLocalizedString>
#include <QCommandLineParser>
#include <kdelibs4configmigrator.h>
#include <KDBusService>
#include "mainwindow.h"
#include "kollision_version.h"

int main(int argc, char *argv[])
{
    // Fixes blurry icons with fractional scaling
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kollision");

    Kdelibs4ConfigMigrator migrate(QStringLiteral("kollision"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kollisionrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("kollisionui.rc"));
    migrate.migrate();

    KAboutData aboutData(QStringLiteral("kollision"), i18n("Kollision"),
                         QStringLiteral(KOLLISION_VERSION_STRING),
                         i18n("Casual ball game"),
                         KAboutLicense::GPL,
                         i18n("(c) 2007 Paolo Capriotti"));
    aboutData.addAuthor(i18n("Paolo Capriotti"), QString(), QStringLiteral("p.capriotti@gmail.com"));
    aboutData.addAuthor(i18n("Dmitry Suzdalev"), QString(), QStringLiteral("dimsuz@gmail.com"));
    aboutData.addCredit(i18n("Matteo Guarnieri"), i18n("Original idea"));
    aboutData.addCredit(i18n("Brian Croom"), i18n("Port to use KGameRenderer"));
    aboutData.setHomepage(QStringLiteral("https://kde.org/applications/games/org.kde.kollision"));


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
