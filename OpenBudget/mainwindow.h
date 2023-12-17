#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBoxLayout>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include "addcategorydialog.h"
#include "addsubcategorydialog.h"
#include "addtransactiondialog.h"
#include "deletetransactiondialog.h"
#include "linechartdialog.h"
#include "logindialog.h"
#include "user.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void showMainWindow(User *user); // Show the main window.
    void addCategory();              // Show the add category dialog.
    void addSubcategory();           // Show the add subcategory dialog.
    void addTransaction();           // Show the add transaction dialog.
    void deleteTransaction();        // Show the delete transaction dialog.
    void viewLineChart();            // Show the line chart dialog.
    void loadTransactions();         // Load transactions.
    void loadTransactionsByCategory(); // Load transactions by category.

private:
    LoginDialog *loginDialog = nullptr;
    AddCategoryDialog *addCategoryDialog = nullptr;
    AddSubcategoryDialog *addSubcategoryDialog = nullptr;
    AddTransactionDialog *addTransactionDialog = nullptr;
    DeleteTransactionDialog *deleteTransactionDialog = nullptr;
    LineChartDialog *lineChartDialog = nullptr;

    QLabel *headerLabel = nullptr;
    QTableWidget *transactionTableWidget = nullptr;
    QLabel *welcomeLabel = nullptr;
    QComboBox *categoryCombo = nullptr;

    QPushButton *viewCategoryButton = nullptr;
    QPushButton *addCategoryButton = nullptr;
    QPushButton *addSubcategoryButton = nullptr;
    QPushButton *addTransactionButton = nullptr;
    QPushButton *deleteTransactionButton = nullptr;
    QPushButton *lineChartButton = nullptr;

    User *m_user = nullptr; // Pointer to the user object.

private:
    void createTransactionTable();
    void setupLayout();
    void loadCategories();
};
#endif // MAINWINDOW_H
