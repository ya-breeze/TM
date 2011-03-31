#ifndef DIAGRAMM_H
#define DIAGRAMM_H

#include <QWidget>
#include <QList>
#include <QMouseEvent>

#include "Convertor.h"
#include "diagrammitem.h"
#include "ListBuilder.h"

class Diagramm: public QWidget {
Q_OBJECT
public:
    explicit Diagramm(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void resizeEvent ( QResizeEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );
protected:
public slots:
    void setItems(const DiagrammItemList & _items);
    Convertor* getConvertor() const;
    void setConvertor(Convertor *_convertor);
    void setSelectedItem(const QUuid& _id);

private:
    void calculateScale();
    QPoint findItem();

    typedef QList<QRectF> RectList;
    RectList m_Rects;
    InternalList m_Items;
    double m_MouseX;
    double m_MouseY;
    double m_Radius;
    Convertor *m_Convertor;
    QUuid   m_SelectedItem;
};

#endif // DIAGRAMM_H
