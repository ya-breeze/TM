#ifndef DIAGRAMMITEM_H
#define DIAGRAMMITEM_H

#include <QUuid>
#include <QString>
#include <QList>

class DiagrammItem {
        QUuid   m_id;
        QString m_name;
        QUuid   m_parent;
        uint    m_value;

    public:
        DiagrammItem(const QString& _name = "", const QUuid& _id = QUuid::createUuid(), uint _value = 0 );

        void setId(const QUuid& _uuid);
        void setName(const QString& _name);
        void setParent(const QUuid& _parent);
        void setValue(uint _value);
        QUuid getId() const;
        QString getName() const;
        QUuid getParent() const;
        uint getValue() const;

        bool operator==(const DiagrammItem& _rh) const;
};

typedef QList<DiagrammItem> DiagrammItemList;

#endif // DIAGRAMMITEM_H
