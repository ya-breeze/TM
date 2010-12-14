#include "utils.h"

#include <QString>
#include <QDir>
#include <QTextStream>

bool createDirFromFile( const char *_fname )
{
	QString path(_fname);
	QDir dir;
	return dir.mkpath( path.left(path.lastIndexOf("/")) );
}

bool createDirFromFile( const QString& _fname )
{
	return createDirFromFile(_fname.toUtf8().data());
}

/// \brief Формирует строку для отображения времени начала задачи
///
/// Выводит дату _date относительно переданной даты _now в более простом для человека виде.
/// Например: "завтра", "послезавтра", "на неделе" и т.д.
QString prettyDate( const QDateTime& _now, const QDateTime& _date)
{
	QString res;

	int days = _now.daysTo(_date);
	if( days>0 )
	{
		// Будущее
		if( days==1 )
			res = QObject::tr("Tomorrow");
		else if( days==2 )
			res = QObject::tr("After tomorrow");
		else if( _now.date().year()==_date.date().year() )
		{
			if( _now.date().weekNumber()==_date.date().weekNumber() )
				res = QObject::tr("This week");
			else if( _now.date().weekNumber()+1==_date.date().weekNumber() )
				res = QObject::tr("Next week");
			else if( _now.date().month()==_date.date().month() )
				res = QObject::tr("This month");
			else if( _now.date().month()+1==_date.date().month() )
				res = QObject::tr("Next month");
		}
		else
			res = _date.toString("yyyy.MM.dd H:mm");
	}
	else if( days<0)
	{
		// Прошлое
		if( days==-1 )
			res = QObject::tr("Yesterday");
		else if( days==-2 )
			res = QObject::tr("Before yesterday");
		else
			res = _date.toString("yyyy.MM.dd H:mm");
	}
	else
	{
		// Сегодня
		res = QObject::tr("Today at ") + _date.toString("H:mm");
	}

	return res;
}

/// Переводит секунды в вид часы:минуты
QString sec2str(int _secs) {
    QString res;

    QTextStream s(&res);
    int hours = _secs / 3600;
    int mins = (_secs-hours*3600)/60;

    s << hours << ":";
    s.setFieldWidth(2);
    s.setPadChar('0');
    s << mins;

    return res;
}

QString escapeJson(const QString& _value) {
    QString res;

    int sz = _value.size();
    for(int i=0; i<sz; ++i) {
	if( _value[i]=='"' )
	    res += "\\\"";
	else if( _value[i]=='\\' )
	    res += "\\\\";
	else if( _value[i]=='/' )
	    res += "/";
	else if( _value[i]=='\b' )
	    res += "\\b";
	else if( _value[i]=='\f' )
	    res += "\\f";
	else if( _value[i]=='\n' )
	    res += "\\n";
	else if( _value[i]=='\r' )
	    res += "\\r";
	else if( _value[i]=='\t' )
	    res += "\\t";
	else
	    res += _value[i];
    }

    return res;
}
QString unescapeJson(const QString& _value) {
    QString res;
    int sz = _value.size();
    for(int i=0; i<sz; ++i) {
	if( _value[i]=='\\' ) {
	    ++i;
	    if( _value[i]=='"' )
		res += "\"";
	    else if( _value[i]=='\\' )
		res += "\\";
	    else if( _value[i]=='/' )
		res += "/";
	    else if( _value[i]=='b' )
		res += "\b";
	    else if( _value[i]=='f' )
		res += "\f";
	    else if( _value[i]=='n' )
		res += "\n";
	    else if( _value[i]=='r' )
		res += "\r";
	    else if( _value[i]=='t' )
		res += "\t";
	} else
	    res += _value[i];
    }

    return res;
}
