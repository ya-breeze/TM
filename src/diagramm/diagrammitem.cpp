#include "diagrammitem.h"

DiagrammItem::DiagrammItem(const QString& _name, const QUuid& _id, uint _value)
    : m_id(_id), m_name(_name), m_value(_value) {
}

void DiagrammItem::setId(const QUuid &_uuid)
{
    m_id = _uuid;
}

void DiagrammItem::setName(const QString &_name)
{
    m_name = _name;
}

void DiagrammItem::setParent(const QUuid &_parent)
{
    m_parent = _parent;
}

void DiagrammItem::setValue(uint _value)
{
    m_value = _value;
}

QUuid DiagrammItem::getId() const
{
    return m_id;
}

QString DiagrammItem::getName() const
{
    return m_name;
}

QUuid DiagrammItem::getParent() const
{
    return m_parent;
}

uint DiagrammItem::getValue() const
{
    return m_value;
}
