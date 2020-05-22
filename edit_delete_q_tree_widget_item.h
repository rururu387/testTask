#ifndef EDIT_DELETE_Q_TREE_WIDGET_ITEM_H
#define EDIT_DELETE_Q_TREE_WIDGET_ITEM_H


#include <QTreeWidgetItem>

class EditDeleteQTreeWidgetItem : public QTreeWidgetItem
{
    QTreeWidgetItem* item;
public:
    EditDeleteQTreeWidgetItem();
    QTreeWidgetItem* getItem() { return item; };
    void setItem(QTreeWidgetItem* _item) { item = _item; };
};

#endif // EDIT_DELETE_Q_TREE_WIDGET_ITEM_H
