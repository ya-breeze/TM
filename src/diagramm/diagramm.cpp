#include "diagramm.h"

#include <QPen>
#include <QPainter>
//#include <QDebug>

#include <math.h>

Diagramm::Diagramm(QWidget *parent) :
    QWidget(parent), m_MouseX(0), m_MouseY(0), m_Radius(0), m_Convertor(NULL) {
    setMouseTracking(true);
}

Convertor *Diagramm::getConvertor() const
{
    return m_Convertor;
}

void Diagramm::setConvertor(Convertor *m_Convertor)
{
    this->m_Convertor = m_Convertor;
}

void Diagramm::paintEvent(QPaintEvent *) {
    QPen pen(Qt::black, 2, Qt::SolidLine);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // border
    painter.drawRect(5, 5, width() - 10, height() - 10);

    for (int i = m_Items.size(); i > 0; --i) {
        const DiagrammItemInternalList &list = m_Items.at(i - 1);
        for (int j = 0; j < list.size(); ++j) {
            const DiagrammItemInternal &item = list.at(j);
            if( item.getId()==m_SelectedItem ) {
                painter.setBrush(QBrush(QColor(255, 100, 100)));
            }
            else {
                painter.setBrush(QBrush(QColor(255 * (1 - item.getStartDegree() / 360), 200, 255 * (item.getStartDegree() / 360))));
            }
            painter.drawPie(m_Rects.at(i - 1), item.getStartDegree() * 16, item.getAngle() * 16 - 1);
        }
    }

    // mouse on item
    if (m_MouseX && m_MouseY && m_Convertor) {
        QPoint item = findItem();
        if( item.x()>=0 ) {
            DiagrammItemInternal dgrItem = m_Items.at(item.x()).at(item.y());
            painter.drawText( QPoint(m_Radius*0.2, m_Radius*0.2), dgrItem.getName() + " - " + m_Convertor->convert(dgrItem.getValue()) );
            painter.drawLine(m_Radius*0.2, m_Radius*0.2+5, m_Radius*0.7, m_Radius*0.2+5);
            double angle = (dgrItem.getStartDegree()+dgrItem.getAngle()/2)*M_PI/180;
            double x = ::cos(angle)*m_Radius * (item.x() + 0.5) / m_Items.size();
            double y = ::sin(angle)*m_Radius * (item.x() + 0.5) / m_Items.size();
            painter.drawLine(m_Radius*0.7, m_Radius*0.2+5, width()/2+x, -y+height()/2);
        }
    }
}

void Diagramm::setItems(const DiagrammItemList &_items) {
    ListBuilder builder;
    m_Items = builder.buildList(_items);
    calculateScale();
    repaint();
}

void Diagramm::resizeEvent(QResizeEvent *) {
    calculateScale();
}

void Diagramm::calculateScale() {

    if (m_Items.empty())
        return;
    m_Rects.clear();

    m_Radius = width() > height() ? height() / 2.0 - 10 : width() / 2.0 - 10;

    double centerH = width() / 2.0;
    double centerV = height() / 2.0;

    for (int i = 0; i < m_Items.size(); ++i) {
        QRectF rect(centerH - m_Radius * (i + 1)/m_Items.size(), centerV - m_Radius * (i + 1)/m_Items.size(), m_Radius
                * 2.0 * (i + 1)/m_Items.size(), m_Radius * 2.0 * (i + 1)/m_Items.size());
        m_Rects.append(rect);
    }
}

void Diagramm::mouseMoveEvent(QMouseEvent * event) {
    m_MouseX = event->pos().x();
    m_MouseY = event->pos().y();
    repaint();
}

QPoint Diagramm::findItem() {
    for (int i = 0; i < m_Items.size(); ++i) {
        const DiagrammItemInternalList &list = m_Items.at(i);
        for (int j = 0; j < list.size(); ++j) {
            const DiagrammItemInternal &item = list.at(j);
            double realX = m_MouseX - width() / 2.0;
            double realY = -m_MouseY + height() / 2.0;

            double x = ::cos(item.getStartDegree() * M_PI / 180);
            double y = ::sin(item.getStartDegree() * M_PI / 180);
            double sign = realY * x - realX * y;

            double x2 = ::cos((item.getStartDegree() + item.getAngle()) * M_PI / 180);
            double y2 = ::sin((item.getStartDegree() + item.getAngle()) * M_PI / 180);
            double sign2 = realY * x2 - realX * y2;


            if( (item.getAngle()<180 && sign>0 && sign2<0) || (item.getAngle()>180 && (sign>0 || sign2<0) ) ) {
                // inside the angle - should check radius
                double radius = m_Radius * (i + 1) / m_Items.size();
                radius *= radius;
                if( (realX*realX + realY*realY)<=radius )
                    return QPoint(i, j);
            }
        }
    }

    return QPoint(-1, -1);
}

void Diagramm::setSelectedItem(const QUuid& _id) {
    m_SelectedItem = _id;
    repaint();
}

void Diagramm::clear() {
    setItems(DiagrammItemList());
}
