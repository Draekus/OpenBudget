#ifndef ADDCATEGORYDIALOG_H
#define ADDCATEGORYDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class AddCategoryDialog : public QDialog
{
    Q_OBJECT

public:
    AddCategoryDialog(int userID, QWidget *parent = nullptr);

signals:
    void categoryAdded(); // Signal to indicate that a category has been added.

private slots:
    void addCategory(); // Add the category when the add button is clicked.

private:
    QLineEdit *categoryNameLineEdit;

private:
    int m_userID;
};

#endif // ADDCATEGORYDIALOG_H
