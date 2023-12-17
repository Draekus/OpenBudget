// link_button.h
#ifndef LINK_BUTTON_H
#define LINK_BUTTON_H

#include <QPushButton>

class LinkButton : public QPushButton
{
    Q_OBJECT

public:
    LinkButton(const QString &text, QWidget *parent = nullptr);
};

#endif // LINK_BUTTON_H
