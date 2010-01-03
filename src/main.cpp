#include <QtGui>
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QtSingleApplication>

#include "mainwindow.h"
#include "TabletWindow.h"

#include "utils.h"
int main( int argc, char *argv[] )
{
	QtSingleApplication app( argc, argv );
	app.setWindowIcon( QIcon(":/images/MainIcon") );
	if( app.isRunning() )
	{
		QMessageBox::critical(NULL, QObject::tr("Unable run tm"), QObject::tr("Another one instance of tm is running"));
		return 0;
	}

	QCoreApplication::setOrganizationName( "Breeze" );
	QCoreApplication::setOrganizationDomain( "ya-breeze.ya.ru" );
	QCoreApplication::setApplicationName( "TM" );

	QTranslator qtTranslator;
	qtTranslator.load( "qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath) );
	app.installTranslator( &qtTranslator );

	QTranslator myappTranslator;
	if( !myappTranslator.load( "tm_" + QLocale::system().name(),
			"/usr/share/translations" ) )
		myappTranslator.load( "tm_" + QLocale::system().name() );
	app.installTranslator( &myappTranslator );

#ifdef TABLET
	TabletWindow w;
	w.show();
#else
	TM w;
	w.show();
#endif

	return app.exec();
}
