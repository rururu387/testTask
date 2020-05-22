#include "goose_button.h"

GooseButton::GooseButton(QString text) : QPushButton(text)
{
    this->setFixedSize(100, 40);
}
