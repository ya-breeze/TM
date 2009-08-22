#include "utils.h"

#include <QString>
#include <QDir>

bool createDirFromFile( const char *_fname )
{
	QString path(_fname);
	QDir dir;
	return dir.mkpath(path.left( path.lastIndexOf("/") ));
}

bool createDirFromFile( const QString& _fname )
{
	QString path(_fname);
	QDir dir;
	return dir.mkpath(path.left( path.lastIndexOf("/") ));
}
