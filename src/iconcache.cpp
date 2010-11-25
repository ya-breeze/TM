#include "iconcache.h"

IconCache::IconCache(Saver& _saver)
    : m_Saver(_saver)
{
}

void IconCache::saveIcon(const QString& _name, const QIcon& _icon) {
    m_Saver.saveIcon(_name, _icon);
}

QIcon IconCache::restoreIcon(const QString& _name) {
    if( _name.isEmpty() )
	return QIcon();

    return m_Saver.restoreIcon(_name);
}

QStringList IconCache::getIconList() {
    return m_Saver.getIconList();
}
