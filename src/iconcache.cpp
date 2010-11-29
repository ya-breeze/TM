#include "iconcache.h"

IconCache::IconCache(Saver& _saver)
    : m_Saver(_saver)
{
}

void IconCache::saveIcon(const QString& _uuid, const QString& _name, const QIcon& _icon) {
    m_Saver.saveIcon(_uuid, _name, _icon);
}

QIcon IconCache::restoreIcon(const QString& _name) {
    if( !_name.isEmpty() ) {
	try {
	    QIcon res = m_Saver.restoreIcon(_name);
	    return res;
	} catch (std::exception& _ex) {
	    DEBUG("Error in restoring icon '" << _ex.what() << "'");
	}
    }

    return QIcon();
}

QStringMap IconCache::getIconList() {
    return m_Saver.getIconList();
}
