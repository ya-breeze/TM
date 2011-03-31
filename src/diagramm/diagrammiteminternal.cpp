#include "diagrammiteminternal.h"

int DiagrammItemInternal::getLevel() const
{
    return m_Level;
}

double DiagrammItemInternal::getStartDegree() const
{
    return m_StartDegree;
}
void DiagrammItemInternal::setLevel(int m_Level)
{
    this->m_Level = m_Level;
}

double DiagrammItemInternal::getAngle() const
{
    return m_Angle;
}

void DiagrammItemInternal::setAngle(double m_Value)
{
    this->m_Angle = m_Value;
}

void DiagrammItemInternal::setStartDegree(double m_StartDegree)
{
    this->m_StartDegree = m_StartDegree;
}

DiagrammItemInternal::DiagrammItemInternal(const DiagrammItem& _item, int _level, double _startDegree, double _value)
    : DiagrammItem(_item),
    m_Level(_level), m_StartDegree(_startDegree), m_Angle(_value)
{
}
