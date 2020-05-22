#include <QTreeWidget>
#include <QMessageBox>
#include <string>
#include "tinyxml2.h"

void errorNotification(std::string fileName, std::string what);

//Functions below are used to translate data widget to xml
void xmlElementToTreeItem(tinyxml2::XMLElement* rootElement, QTreeWidgetItem* rootItem);
void xmlToTree(std::string fileName, QTreeWidget* rootItem);

//TODO set proper whitespaces for first children
tinyxml2::XMLElement* treeItemToXmlElement(tinyxml2::XMLElement* rootElement, QTreeWidgetItem* rootItem);
void treeToXml(std::string fileName, QTreeWidget* widget);
