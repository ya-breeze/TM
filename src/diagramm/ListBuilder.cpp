/*
 * ListBuilder.cpp
 *
 *  Created on: 25.03.2011
 *      Author: ruilko
 */

#include "ListBuilder.h"

#include <QDebug>

InternalList ListBuilder::buildList(const DiagrammItemList & _items) {
    DiagrammItemList list(_items);
    //qDebug() << "-- before --";
    //DiagrammItem item;
    //foreach(item, list)
    //        qDebug() << item.getName() << ":" << item.getValue();
    //qDebug() << "----";
    fillValues(list);
    //qDebug() << "-- after --";
    //foreach(item, list)
    //        qDebug() << item.getName() << ":" << item.getValue();
    //qDebug() << "----";

    InternalList result = createInternalList(list);
    //qDebug() << "----";
    //for (InternalList::const_iterator it = result.begin(); it != result.end(); ++it) {
    //    qDebug() << "next level";
    //    for (DiagrammItemInternalList::const_iterator it2 = it->begin(); it2
    //            != it->end(); ++it2) {
    //        qDebug() << it2->getName() << ":" << it2->getValue() << " - "
    //                << it2->getStartDegree() << ":" << it2->getAngle();
    //    }
    //}

    return result;
}

bool ListBuilder::hasChildren(const DiagrammItemList & _items,
        const DiagrammItem & _id) {
    DiagrammItem item;
    QUuid id = _id.getId();
    foreach(item, _items)
        {
            if (item.getId() == id)
                return true;
        }

    return false;
}

void ListBuilder::recurseAddChildrenValues(DiagrammItemList & _items,
        const QUuid& _item) {
    DiagrammItemList::iterator it = qFind(_items.begin(), _items.end(), DiagrammItem("", _item));
    Q_ASSERT( it!=_items.end() );
    DiagrammItem &item = *it;
    //qDebug() << "filling children for item " << item.getName();
    PtrDiagrammItemList children = getChildren(_items, item);
    //qDebug() << "children size - " << children.size();

    if (!children.empty()) {
        for (PtrDiagrammItemList::iterator it = children.begin(); it
                != children.end(); ++it) {
            //qDebug() << "fill for child " << (*it)->getName();
            recurseAddChildrenValues(_items, (*it)->getId());
        }
        uint value = 0;

        for (PtrDiagrammItemList::iterator it = children.begin(); it
                != children.end(); ++it) {
            value += (*it)->getValue();
        }

        item.setValue(item.getValue() + value);
        //qDebug() << "item new value " << item.getName() << ":" << item.getValue();
    }
}

void ListBuilder::fillValues(DiagrammItemList & _items) {
    for (DiagrammItemList::iterator it = _items.begin(); it != _items.end(); ++it) {
        if (it->getParent().isNull()) {
            //qDebug() << "top item " << it->getName();
            recurseAddChildrenValues(_items, it->getId());
            //qDebug() << "top item " << it->getName() << " value " << it->getValue();
        }
    }
}

PtrDiagrammItemList ListBuilder::getChildren(const DiagrammItemList & _items,
        const DiagrammItem & _parent) {
    PtrDiagrammItemList result;

    QUuid id = _parent.getId();
    for (DiagrammItemList::const_iterator it = _items.begin(); it
            != _items.end(); ++it) {
        if (it->getParent() == id)
            result.append( &(*it) );
    }

    return result;
}

InternalList ListBuilder::createInternalList(const DiagrammItemList & _items) {
    double currentAngle = 0;
    double total = 0;

    // Find total sum
    for (DiagrammItemList::const_iterator it = _items.begin(); it
            != _items.end(); ++it) {
        if (it->getParent().isNull()) {
            total += it->getValue();
        }
    }
    //qDebug() << "total sum " << total;

    // Start creating InternalList
    InternalList result;
    result.append(DiagrammItemInternalList());
    for (DiagrammItemList::const_iterator it = _items.begin(); it
            != _items.end(); ++it) {
        if (it->getParent().isNull()) {
            double angle = it->getValue() / total * 360;
            DiagrammItemInternal internal(*it, 0, currentAngle, angle);
            result[0].push_back(internal);

            appendChildren(result, _items, 1, currentAngle, total, internal);

            //qDebug() << internal.getName() << ":" << internal.getValue()
            //        << " - " << internal.getStartDegree() << ":"
            //        << internal.getAngle() << " at level 0";

            currentAngle += angle;
        }
    }

    return result;
}

void ListBuilder::appendChildren(InternalList &_result,
        const DiagrammItemList &_items, int _level, double _currentAngle,
        double _total, DiagrammItemInternal &_item) const {
    for (DiagrammItemList::const_iterator itChild = _items.begin(); itChild
            != _items.end(); ++itChild) {
        if (itChild->getParent() == _item.getId()) {
            if (_result.size() <= _level)
                _result.append(DiagrammItemInternalList());

            double angle = itChild->getValue() / _total * 360.0;
            DiagrammItemInternal internal(*itChild, 0, _currentAngle, angle);
            _result[_level].push_back(internal);

            //qDebug() << internal.getName() << ":" << internal.getValue()
            //        << " - " << internal.getStartDegree() << ":"
            //        << internal.getAngle() << " at level " << _level;

            for (DiagrammItemList::const_iterator itChild2 = _items.begin(); itChild2
                    != _items.end(); ++itChild2) {
                if (itChild2->getParent() == itChild->getId())
                    appendChildren(_result, _items, _level + 1, _currentAngle,
                            _total, internal);
            }

            _currentAngle += angle;
            Q_ASSERT((int)_currentAngle<=360);
        }
    }
}
