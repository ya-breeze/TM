/*
 * ListBuilder.h
 *
 *  Created on: 25.03.2011
 *      Author: ruilko
 */

#ifndef LISTBUILDER_H_
#define LISTBUILDER_H_

#include "diagrammiteminternal.h"

typedef QList<DiagrammItemInternal> DiagrammItemInternalList;
typedef QList<DiagrammItemInternalList> InternalList;

class ListBuilder {
public:
    InternalList buildList(const DiagrammItemList& _items);
private:
    bool hasChildren(const DiagrammItemList& _items, const DiagrammItem& _id);
    void fillValues(DiagrammItemList& _items);
    void fillChildValues(DiagrammItemList& _items, const DiagrammItem& _parent);
    DiagrammItemList getChildren(const DiagrammItemList& _items, const DiagrammItem& _parent);
    void fillChildrenList(DiagrammItemList & _items, DiagrammItem& item);
    InternalList createInternalList(const DiagrammItemList& _items);
    void appendChildren(InternalList &_result, const DiagrammItemList &_items, int _level, double _currentAngle, double _total, DiagrammItemInternal &_item) const;
};

#endif /* LISTBUILDER_H_ */
