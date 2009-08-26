#include "mainwindow.h"

#include <QtGui>
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QtSingleApplication>


int main( int argc, char *argv[] )
{
	QtSingleApplication app( argc, argv );
	app.setWindowIcon( QIcon(":/images/MainIcon") );
	if( app.isRunning() )
	{
		QMessageBox::critical(NULL, app.tr("Unable run tm"), app.tr("Another one instance of tm is running"));
		return 0;
	}

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
