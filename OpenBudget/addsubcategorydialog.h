#ifndef ADDSUBCATEGORYDIALOG_H
#define ADDSUBCATEGORYDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class AddSubcategoryDialog : public QDialog
{
    Q_OBJECT

public:
    AddSubcategoryDialog(int userID, QWidget *parent = nullptr);

signals:
    void subcategoryAdded(); // Signal to indicate that a category has been added.

private slots:
    void addSubcategory(); // Add the category when the add button is clicked.

private:
    QLineEdit *subcategoryNameLineEdit = nullptr;
    QComboBox *categoryCombo = nullptr;
    QPushButton *addButton = nullptr;

private:
    void loadCategories(); // Load categories from the database.

private:
    int m_userID;
};

#endif // ADDSUBCATEGORYDIALOG_H
