#include "view_edit_q_tree_widget.h"

ViewEditQTreeWidget::ViewEditQTreeWidget(TreeMode _mode)
{
    mode = _mode;
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void ViewEditQTreeWidget::setTreeEditable(QTreeWidgetItem* item, bool isEditable)
{
    for(int i = 0; i < item->childCount(); i++)
    {
        if (isEditable)
        {
            item->setFlags(item->flags().setFlag(Qt::ItemIsEnabled, true).setFlag(Qt::ItemIsEditable, true));
        }
        else
        {
            item->setFlags(item->flags().setFlag(Qt::ItemIsEditable, false));
        }
        setTreeEditable(item->child(i), isEditable);
    }
}

void ViewEditQTreeWidget::setExpandedItems()
{
    auto selectedItems = this->selectedItems();
    for (auto it = selectedItems.begin(); it != selectedItems.end(); it++)
    {
        QTreeWidgetItem* selectedItem = *it;
        selectedItem->setExpanded(true);
        setChildrenExpanded(selectedItem, true);
    }
}

void ViewEditQTreeWidget::setCollapsedItems()
{
    auto selectedItems = this->selectedItems();
    for (auto it = selectedItems.begin(); it != selectedItems.end(); it++)
    {
        QTreeWidgetItem* selectedItem = *it;
        selectedItem->setExpanded(false);
        setChildrenExpanded(selectedItem, false);
    }
}

void ViewEditQTreeWidget::setChildrenExpanded(QTreeWidgetItem* item, bool boolExpanded)
{
    auto allDescendants = this->allItemDescendants(item);
    allDescendants.remove(item);
    for (auto itDescendants = allDescendants.begin(); itDescendants != allDescendants.end(); itDescendants++)
    {
        (*itDescendants)->setExpanded(boolExpanded);
        setChildrenExpanded(*itDescendants, boolExpanded);
    }
}

void ViewEditQTreeWidget::setMode(TreeMode mode)
{
    switch(mode)
    {
    case editTree:
    {
        setTreeEditable(this->invisibleRootItem(), true);
        this->setEditTriggers(QAbstractItemView::DoubleClicked);
        this->mode = editTree;
        break;
    }
    case viewTree:
    {
        setTreeEditable(this->invisibleRootItem(), false);
        this->setEditTriggers(QAbstractItemView::NoEditTriggers);
        this->mode = viewTree;
        break;
    }
    default:
    {
        throw std::invalid_argument("No such mode defined in ViewEditTree.");
        break;
    }
    }
}

QSet<QTreeWidgetItem*> ViewEditQTreeWidget::allItemDescendants(QTreeWidgetItem* item)
{
    QSet<QTreeWidgetItem*> descendants;
    descendants.insert(item);
    for (int i = 0; i < item->childCount(); i++)
    {
        QSet<QTreeWidgetItem*> childDescendants = allItemDescendants(item->child(i));
        for (auto itChildDescendants = childDescendants.begin(); itChildDescendants != childDescendants.end(); itChildDescendants++)
        {
            descendants.insert(*itChildDescendants);
        }
    }
    return descendants;
}

void ViewEditQTreeWidget::deleteSelectedItems()
{
    //TODO save changes.
    auto selectedItemsList = this->selectedItems();
    QSet<QTreeWidgetItem*> deletedItemsSet;

    for (auto itSelected = selectedItemsList.begin(); itSelected != selectedItemsList.end(); itSelected++)
    {
        if (deletedItemsSet.find(*itSelected) == deletedItemsSet.end())
        {
            auto toBeDeleted = allItemDescendants(*itSelected);
            for (auto itToBeDeleted = toBeDeleted.begin(); itToBeDeleted != toBeDeleted.end(); itToBeDeleted++)
            {
                deletedItemsSet.insert(*itToBeDeleted);
            }
            delete *itSelected;
        }
    }
}

void ViewEditQTreeWidget::addSiblingItem()
{
    if (this->selectedItems().size() != 1)
        throw("Error in selection. Only one item must be selected!");

    QTreeWidgetItem* selectedItem = *(this->selectedItems().begin());
    QTreeWidgetItem* selectedItemParent = selectedItem->parent();

    QTreeWidgetItem* newItem = new QTreeWidgetItem(selectedItemParent);

    //For some reason newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEnabled) doesn't work properly here
    newItem->setFlags(newItem->flags().setFlag(Qt::ItemIsEditable).setFlag(Qt::ItemIsEnabled));
    newItem->setText(0, "Enter your text here");

    if (selectedItemParent != nullptr)
    {
        selectedItemParent->insertChild(selectedItemParent->indexOfChild(selectedItem), newItem);
    }
    else
    {
        this->insertTopLevelItem(this->indexOfTopLevelItem(selectedItem) + 1, newItem);
    }
}

void ViewEditQTreeWidget::addChildItem()
{
    if (this->selectedItems().size() != 1)
        throw("Error in selection. Only one item must be selected!");
    QTreeWidgetItem* selectedItem = *(this->selectedItems().begin());
    QTreeWidgetItem* newItem = new QTreeWidgetItem(selectedItem);
    newItem->setFlags(newItem->flags().setFlag(Qt::ItemIsEditable).setFlag(Qt::ItemIsEnabled));
    newItem->setText(0, "Enter your text here");
    selectedItem->insertChild(selectedItem->childCount() + 1, newItem);
    selectedItem->setExpanded(true);
}

void ViewEditQTreeWidget::addTopItem()
{
    QTreeWidgetItem* newItem = new QTreeWidgetItem(this->invisibleRootItem());
    newItem->setFlags(newItem->flags().setFlag(Qt::ItemIsEditable).setFlag(Qt::ItemIsEnabled));
    newItem->setText(0, "Enter your text here");
    this->invisibleRootItem()->insertChild(this->invisibleRootItem()->childCount() + 1, newItem);
    this->invisibleRootItem()->setExpanded(true);
}

void ViewEditQTreeWidget::itemChanged()
{}
