// link_button.cpp
#include "linkbutton.h"

/**
 * @brief Button formatted to look like a hyperlink.
 * 
 * @param text QString containing the text to display on the button.
 * @param parent QWidget pointer to parent widget.
 */
LinkButton::LinkButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    // Set the button's style sheet to look like a hyperlink.
    setStyleSheet("QPushButton {"
                  "    border: none;"
                  "    color: #0066cc;"             // Set the link color
                  "    text-decoration: underline;" // Add underline
                  "}"
                  "QPushButton:hover {"
                  "    color: #003366;" // Set the color when hovered
                  "}");
}
// link_button.h
