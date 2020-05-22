#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QApplication>
#include <QTreeWidget>
#include <QSpacerItem>
#include <QMenu>
#include <QFile>
#include <QXmlSchema>
#include <QXmlStreamReader>
#include <string>
#include "goose_button.h"
#include "view_edit_q_tree_widget.h"
#include "xmltreedatatransfer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QWidget* mainWidget;
    ViewEditQTreeWidget* companyDataTree;

    QGridLayout* mainLayout;
    QVBoxLayout* buttonsLayout;
    QPushButton* editButton;
    QPushButton* viewButton;
    QPushButton* foldButton;
    QPushButton* unfoldButton;
    QPushButton* saveButton;
    QPushButton* exitButton;

public:
    MainWindow(QWidget *parent = nullptr);

    //Placing tems in a window described here
    void defaultLookout();

    //Here are located all connect function calls
    void defaultSignals();

    bool cursorOnWidget(QWidget* widget);
    ~MainWindow();
public slots:
    //Each of those functions calls cursorOnWidget. Button won't be pushed if you swipe
    //mouse away from it without having released
    void handleEditButton();
    void handleViewButton();
    void handleFoldButton();
    void handleUnfoldButton();
    void handleSaveButton();
    void handleExitButton();

    //An order of context menu actions was set randomly
    void handleTreeContextMenu(QPoint mousePos);

private:
    //Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
