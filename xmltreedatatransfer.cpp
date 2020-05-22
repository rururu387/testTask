#include "xmltreedatatransfer.h"

void errorNotification(std::string fileName, std::string what)
{
    std::string errorStr = "Error occurred. File: " + fileName + ". " + what;
    QMessageBox* errorBox = new QMessageBox();
    errorBox->setText(QString::fromUtf8(errorStr.c_str()));
    errorBox->show();
}

void xmlElementToTreeItem(tinyxml2::XMLElement* rootElement, QTreeWidgetItem* rootItem)
{
    rootItem->setText(1, QString::fromUtf8(rootElement->Name()));
    if (rootElement->GetText() != 0)
        rootItem->setText(3, QString::fromUtf8(rootElement->GetText()));
    if (rootElement->Attribute("name") != 0)
    {
        rootItem->setText(2, rootElement->Attribute("name"));
    }

    auto curElement = rootElement->FirstChildElement();

    if (curElement == 0)
        return;

    while (curElement != rootElement->LastChild())
    {
        QTreeWidgetItem* child = new QTreeWidgetItem();
        child->setFlags(child->flags().setFlag(Qt::ItemIsEnabled, true).setFlag(Qt::ItemIsEditable, true));
        rootItem->addChild(child);
        xmlElementToTreeItem(curElement, child);
        curElement = curElement->NextSiblingElement();
    }

    QTreeWidgetItem* child = new QTreeWidgetItem();
    child->setFlags(child->flags().setFlag(Qt::ItemIsEnabled, true).setFlag(Qt::ItemIsEditable, true));
    rootItem->addChild(child);
    xmlElementToTreeItem(curElement, child);
}

void xmlToTree(std::string fileName, QTreeWidget* treeWidget)
{
    tinyxml2::XMLDocument* xmlDocument = new tinyxml2::XMLDocument();

    auto error = xmlDocument->LoadFile(fileName.c_str());
    if (error != tinyxml2::XML_SUCCESS)
    {
        errorNotification(fileName, "Could not load file:  ");
    }

    treeWidget->clear();

    if (xmlDocument->NoChildren())
        return;


    tinyxml2::XMLElement* curTopLevelChid = xmlDocument->FirstChildElement();

    while (curTopLevelChid != xmlDocument->LastChildElement())
    {
        QTreeWidgetItem* topLevelItem = new QTreeWidgetItem();
        topLevelItem->setFlags(topLevelItem->flags().setFlag(Qt::ItemIsEnabled, true).setFlag(Qt::ItemIsEditable, true));
        xmlElementToTreeItem(xmlDocument->FirstChildElement(), topLevelItem);
        treeWidget->addTopLevelItem(topLevelItem);
        curTopLevelChid = curTopLevelChid->NextSiblingElement();
    }

    //Processing last element (xmlDocument->LastChildElement()).
    //Calling lastElement->NextSiblingElement() may lead to segfault
    QTreeWidgetItem* topLevelItem = new QTreeWidgetItem();
    topLevelItem->setFlags(topLevelItem->flags().setFlag(Qt::ItemIsEnabled, true).setFlag(Qt::ItemIsEditable, true));
    xmlElementToTreeItem(xmlDocument->LastChildElement(), topLevelItem);
    treeWidget->addTopLevelItem(topLevelItem);
}

tinyxml2::XMLElement* treeItemToXmlElement(tinyxml2::XMLDocument* document, tinyxml2::XMLElement* rootElement, QTreeWidgetItem* rootItem)
{
    rootElement->SetText(rootItem->text(3).toStdString().c_str());
    rootElement->SetName(rootItem->text(1).toStdString().c_str());

    if (!rootItem->text(2).isEmpty())
    {
        rootElement->SetAttribute("name", rootItem->text(2).toStdString().c_str());
    }

    tinyxml2::XMLElement* curChildElement;

    for (int i = 0; i < rootItem->childCount(); i++)
    {
        curChildElement = document->NewElement("IntValue");
        rootElement->InsertEndChild(treeItemToXmlElement(document, curChildElement, rootItem->child(i)));
        rootElement->LinkEndChild(curChildElement);
    }

    return rootElement;
}

void treeToXml(std::string fileName, QTreeWidget* widget)
{
    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();

    tinyxml2::XMLDeclaration* decl = document->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    document->InsertFirstChild(decl);
    document->LinkEndChild(decl);

    tinyxml2::XMLElement* curRootElement;

    //The correct way to make XML is to set the only root element
    //But both: application and parser set no limits on that. So do I.
    for (int i = 0; i < widget->topLevelItemCount(); i++)
    {
         curRootElement = treeItemToXmlElement(document,
                        document->NewElement(widget->topLevelItem(i)->text(1).toStdString().c_str())->ToElement(), widget->topLevelItem(i));
    }

    document->LinkEndChild(curRootElement);

    tinyxml2::XMLError error = document->SaveFile(fileName.c_str());

    if (error != 0)
    {
        errorNotification(fileName, "Could not save to file:  ");
    }
}
