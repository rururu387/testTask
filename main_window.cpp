#include "main_window.h"
#include <iostream>

bool MainWindow::cursorOnWidget(QWidget* widget)
{
    QPoint widgetCursorPos = mapFromGlobal(QCursor::pos());
    if (widgetCursorPos.x() < widget->x() + widget->width() && widgetCursorPos.x() > widget->x() &&
            widgetCursorPos.y() < widget->y() + widget->height() && widgetCursorPos.y() > widget->y())
    {
        return 1;
    }
    return 0;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->mainWidget = new QWidget(this);
    mainLayout = new QGridLayout();
    buttonsLayout = new QVBoxLayout();
    editButton = new GooseButton("Edit");
    viewButton = new GooseButton("View");
    foldButton = new GooseButton("Fold");
    unfoldButton = new GooseButton("Unfold all");
    saveButton = new GooseButton("Save");
    exitButton = new GooseButton("Exit");
    companyDataTree = new ViewEditQTreeWidget(viewTree);
    defaultSignals();
}

void MainWindow::handleEditButton()
{
    if(this->cursorOnWidget(editButton))
    {
        editButton->hide();
        viewButton->show();
        companyDataTree->setMode(editTree);
    }
}

void MainWindow::handleViewButton()
{
    if(this->cursorOnWidget(viewButton))
    {
        viewButton->hide();
        editButton->show();
        companyDataTree->setMode(viewTree);
    }
}

void MainWindow::handleFoldButton()
{
    if(this->cursorOnWidget(foldButton))
    {
        this->companyDataTree->collapseAll();
    }
}

void MainWindow::handleUnfoldButton()
{
    if(this->cursorOnWidget(unfoldButton))
    {
        this->companyDataTree->expandAll();
    }
}

void MainWindow::handleSaveButton()
{
    if(this->cursorOnWidget(saveButton))
    {
        treeToXml("../outputFile.xml", this->companyDataTree);
    }
}

void MainWindow::handleExitButton()
{
    if(this->cursorOnWidget(exitButton))
        QApplication::quit();
}

void MainWindow::handleTreeContextMenu(QPoint mousePos)
{
    QMenu *menu = new QMenu(this);

    //Never mind. Does nothing. Used to let you know there is a context menu when nothing selected.
    QAction* honkAction = new QAction("Honk!");
    menu->addAction(honkAction);

    if(companyDataTree->selectedItems().size() > 0)
    {
        QAction* unfoldItemAction = new QAction("Unfold");
        connect(unfoldItemAction, &QAction::triggered, companyDataTree, &ViewEditQTreeWidget::setExpandedItems);
        menu->addAction(unfoldItemAction);

        QAction* foldItemAction = new QAction("Fold");
        connect(foldItemAction, &QAction::triggered, companyDataTree, &ViewEditQTreeWidget::setCollapsedItems);
        menu->addAction(foldItemAction);
    }

    if (this->companyDataTree->widgetStatus() == editTree)
    {
        if (companyDataTree->topLevelItemCount() == 0)
        {
            QAction* insertFirstItemAction = new QAction("Add item");
            connect(insertFirstItemAction, &QAction::triggered, companyDataTree, &ViewEditQTreeWidget::addTopItem);
            menu->addAction(insertFirstItemAction);
        }
        else
        {
            QAction* deleteAction = new QAction("Delete");
            connect(deleteAction, &QAction::triggered, companyDataTree, &ViewEditQTreeWidget::deleteSelectedItems);
            menu->addAction(deleteAction);
        }

        if (companyDataTree->selectedItems().size() == 1)
        {
            QAction* insertBrotherAction = new QAction("Add sibling");
            connect(insertBrotherAction, &QAction::triggered, companyDataTree, &ViewEditQTreeWidget::addSiblingItem);
            menu->addAction(insertBrotherAction);

            QAction* insertChildAction = new QAction("Add child");
            connect(insertChildAction, &QAction::triggered, companyDataTree, &ViewEditQTreeWidget::addChildItem);
            menu->addAction(insertChildAction);
        }
    }

    menu->popup(companyDataTree->viewport()->mapToGlobal(mousePos));
}

MainWindow::~MainWindow()
{
    delete this->editButton;
    delete this->viewButton;
    delete this->exitButton;
    delete this->companyDataTree;
    delete this->mainWidget;
    //delete ui;
}


void MainWindow::defaultSignals()
{
    connect(editButton, SIGNAL(released()), this, SLOT(handleEditButton()));
    connect(viewButton, SIGNAL(released()), this, SLOT(handleViewButton()));
    connect(foldButton, SIGNAL(released()), this, SLOT(handleFoldButton()));
    connect(unfoldButton, SIGNAL(released()), this, SLOT(handleUnfoldButton()));
    connect(saveButton, SIGNAL(released()), this, SLOT(handleSaveButton()));
    connect(exitButton, SIGNAL(released()), this, SLOT(handleExitButton()));
    companyDataTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(companyDataTree, SIGNAL(customContextMenuRequested(QPoint)), SLOT(handleTreeContextMenu(QPoint)));
}

void MainWindow::defaultLookout()
{
    mainWidget->setMinimumSize(800, 600);
    mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    viewButton->hide();
    this->setMinimumSize(800, 600);
    this->setMaximumSize(1920, 1080);

    buttonsLayout->addWidget(this->foldButton, 0);
    buttonsLayout->addWidget(this->unfoldButton, 1, Qt::AlignTop);
    std::cout.flush();
    buttonsLayout->addWidget(this->editButton, 2, Qt::AlignBottom);
    buttonsLayout->addWidget(this->viewButton, 2, Qt::AlignBottom);
    buttonsLayout->addWidget(this->saveButton, 3, Qt::AlignBottom);
    buttonsLayout->addWidget(this->exitButton, 4);
    mainLayout->addWidget(companyDataTree, 0, 0, 3, 1);
    mainLayout->addLayout(buttonsLayout, 0, 2, Qt::AlignBottom);

    this->mainWidget->setLayout(mainLayout);
    companyDataTree->headerItem()->setText(0, "Arrows");
    companyDataTree->headerItem()->setText(1, "Tags");
    companyDataTree->headerItem()->setText(2, "Attributes");
    companyDataTree->headerItem()->setText(3, "Text");
    companyDataTree->setColumnCount(4);
    companyDataTree->setColumnWidth(0, 100);
    companyDataTree->setColumnWidth(1, 100);
    companyDataTree->setColumnWidth(2, 200);

    std::string fileName = "../inputFile.xml";
    QFile file("../inputFile.xml");
    if (file.open(QIODevice::ReadOnly) == 0)
    {
        errorNotification(fileName, "XML file does not exist.");
    }
    else
    {
        QXmlStreamReader reader(&file);
        bool isVellFormed = true;
        while (!reader.atEnd())
        {
            reader.readNext();

            if (reader.error())
            {
                errorNotification(fileName, "XML file may be formed unwell.");
                isVellFormed = false;
            }
        }
        if (isVellFormed)
        {
            xmlToTree(fileName, companyDataTree);
            companyDataTree->setMode(viewTree);
        }
    }
    this->setCentralWidget(mainWidget);

    return;
}
