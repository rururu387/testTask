#ifndef VIEWEDITQTREEWIDGET_H
#define VIEWEDITQTREEWIDGET_H

#include <QTreeWidget>
#include <QAbstractItemView>
#include <QMenu>
#include "edit_delete_q_tree_widget_item.h"

enum TreeMode
{
    viewTree = 0,
    editTree
};

class ViewEditQTreeWidget : public QTreeWidget
{
    Q_OBJECT
    TreeMode mode;
    void setTreeEditable(QTreeWidgetItem* item, bool isEditable);
public:
    ViewEditQTreeWidget(TreeMode _mode);
    void setMode(TreeMode mode);
    void setViewMode();
    TreeMode widgetStatus() { return mode; };

    //This funcion assumes that item is a descendant of item
    QSet<QTreeWidgetItem*> allItemDescendants(QTreeWidgetItem* item);
    void deleteSelectedItems();
    void addSiblingItem();
    void addChildItem();
    void addTopItem();
    void setChildrenExpanded(QTreeWidgetItem* item, bool boolExpanded);
    void setExpandedItems();
    void setCollapsedItems();
public slots:
    void itemChanged();
};

#endif // VIEWEDITQTREEWIDGET_H
