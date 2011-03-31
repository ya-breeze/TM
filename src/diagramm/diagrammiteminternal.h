#ifndef DIAGRAMMITEMINTERNAL_H
#define DIAGRAMMITEMINTERNAL_H

#include "diagrammitem.h"

class DiagrammItemInternal : public DiagrammItem
{
	int		m_Level;
	double  m_StartDegree;
	double  m_Angle;
public:
    DiagrammItemInternal(const DiagrammItem& _item, int _level, double _startDegree, double _angle);
    int getLevel() const;
    double getStartDegree() const;
    void setLevel(int m_Level);
    void setStartDegree(double m_StartDegree);
    double getAngle() const;
    void setAngle(double m_Value);
};

#endif // DIAGRAMMITEMINTERNAL_H
