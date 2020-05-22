#ifndef GOOSEBUTTON_H
#define GOOSEBUTTON_H

#include <QPushButton>
#include <QFont>

//This class does nothing. It should have been removed, but it's easier to leave a comment
class GooseButton : public QPushButton
{
    //Geese are cool
    Q_OBJECT
public:
    GooseButton(QString text);
};

#endif // GOOSEBUTTON_H
