#ifndef ICONCACHE_H
#define ICONCACHE_H

#include "Saver.h"

/// Класс кеширующий картинки, чтобы с одной стороны минимизировать общения в базу,
/// а с другой - не сильно в памяти разростаться
/// FIXME Собственно добавить кеш
class IconCache
{
    Saver   &m_Saver;

public:
    IconCache(Saver& _saver);
    QStringMap	getIconList();
    void	saveIcon(const QString& _uuid, const QString& _name, const QIcon& _icon);
    QIcon	restoreIcon(const QString& _uuid);
};

#endif // ICONCACHE_H
