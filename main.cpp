#include "mainwindow.h"

#include <QtGui>
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>


int main( int argc, char *argv[] )
{
	QApplication app( argc, argv );
	app.setWindowIcon( QIcon( ":/images/MainIcon" ) );
	QCoreApplication::setOrganizationName( "Breeze" );
	QCoreApplication::setOrganizationDomain( "ya-breeze.ya.ru" );
	QCoreApplication::setApplicationName( "TM" );

	QTranslator qtTranslator;
	qtTranslator.load( "qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath ) );
	app.installTranslator( &qtTranslator );

	QTranslator myappTranslator;
	if( !myappTranslator.load( "fly-system-monitor_" + QLocale::system().name(),
			"/usr/share/fly/translations" ) )
		myappTranslator.load( "fly-system-monitor_" + QLocale::system().name() );
	app.installTranslator( &myappTranslator );

	TM w;
	w.show();

	return app.exec();
}
