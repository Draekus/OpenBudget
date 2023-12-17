#include "mainwindow.h"
#include <QApplication>
#include <QGroupBox>
#include <QHeaderView>
#include <QSqlError>
#include <QSqlQuery>
#include "database.h"
#include "logindialog.h"
#include "user.h"

/**
 * @brief Main window show's the user's transactions.
 * @param parent QWidget pointer to parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    // Create the widgets
    createTransactionTable();
    // Setup the layout
    setupLayout();
    // Hide the main window.
    hide();
    // Create a login dialog.
    loginDialog = new LoginDialog(this);
    // Show the login dialog.
    loginDialog->show();

    /* Dialog Connections */
    // If a user successfully logs in, show the main window.
    connect(loginDialog, &LoginDialog::userLoggedInSignal, this, &MainWindow::showMainWindow);
    // If the login dialog is closed, quit the application.
    connect(loginDialog, &LoginDialog::finished, this, &QCoreApplication::quit);

    /* Button Connections */
    // If the view category button is clicked, load transactions by category.
    connect(viewCategoryButton,
            &QPushButton::clicked,
            this,
            &MainWindow::loadTransactionsByCategory);
    // If the add category button is clicked, show the add category dialog.
    connect(addCategoryButton, &QPushButton::clicked, this, &MainWindow::addCategory);
    // If the add subcategory button is clicked, show the add subcategory dialog.
    connect(addSubcategoryButton, &QPushButton::clicked, this, &MainWindow::addSubcategory);
    // If the add transaction button is clicked, add a transaction to the database.
    connect(addTransactionButton, &QPushButton::clicked, this, &MainWindow::addTransaction);
    // If the delete transaction button is clicked, delete a transaction from the database.
    connect(deleteTransactionButton, &QPushButton::clicked, this, &MainWindow::deleteTransaction);
    // If the line chart button is clicked, show the line chart dialog.
    connect(lineChartButton, &QPushButton::clicked, this, &MainWindow::viewLineChart);
}

/**
 * @brief Show the main window when the user has successfully logged in.
 * 
 * @param user Pointer to the user object.
 */
void MainWindow::showMainWindow(User *user)
{
    // Set the welcome label.
    welcomeLabel->setText("Welcome, " + user->firstName() + " " + user->lastName() + "!");
    // Store the user info of the logged in user.
    m_user = user;
    // Load transactions.
    loadTransactions();
    // Load categories.
    loadCategories();
    // Show the main window.
    show();
}

/**
 * @brief Add a new category to the database.
 */
void MainWindow::addCategory()
{
    // Create an add category dialog.
    addCategoryDialog = new AddCategoryDialog(m_user->userID(), this);
    // Show the add category dialog.
    addCategoryDialog->show();
    // If the add category dialog is accepted, load categories.
    connect(addCategoryDialog, &AddCategoryDialog::accepted, this, &MainWindow::loadCategories);
    // If the add category dialog is rejected, delete the dialog.
    connect(addCategoryDialog,
            &AddCategoryDialog::rejected,
            addCategoryDialog,
            &QObject::deleteLater);
}

/**
 * @brief Add a new subcategory to the database.
 */
void MainWindow::addSubcategory()
{
    // Create an add subcategory dialog.
    addSubcategoryDialog = new AddSubcategoryDialog(m_user->userID(), this);
    // Show the add subcategory dialog.
    addSubcategoryDialog->show();
    // If the add subcategory dialog is accepted, load categories.
    connect(addSubcategoryDialog,
            &AddSubcategoryDialog::accepted,
            this,
            &MainWindow::loadCategories);
    // If the add subcategory dialog is rejected, delete the dialog.
    connect(addSubcategoryDialog,
            &AddSubcategoryDialog::rejected,
            addSubcategoryDialog,
            &QObject::deleteLater);
}

/**
 * @brief Retrieve all of the user's transactions from the database and populate the table widget.
 */
void MainWindow::loadTransactions()
{
    // Get database instance.
    Database *db = Database::getInstance();

    // Get the list of transactions.
    QVector<Transaction *> transactions = db->getTransactions(m_user->userID());
    // Clear the table.
    transactionTableWidget->clearContents();
    // Set the number of columns in the table widget.
    transactionTableWidget->setColumnCount(7);
    // Set the final column to be hidden from user.
    transactionTableWidget->setColumnHidden(5, false);
    transactionTableWidget->setColumnHidden(6, true);
    // Set the table headers.
    transactionTableWidget->setHorizontalHeaderLabels(
        {"Date", "Description", "Category", "Subcategory", "Amount", "Balance"});
    // Set the number of rows.
    transactionTableWidget->setRowCount(transactions.size());

    // Get the category names.
    QMap<int, QString> categoryNames = db->getCategoryNames(m_user->userID());

    // Populate the table.
    for (int i = 0; i < transactions.size(); i++) {
        // Get the current transaction.
        Transaction *transaction = transactions.at(i);

        // Set the transaction date.
        QTableWidgetItem *dateItem = new QTableWidgetItem(transaction->date());
        transactionTableWidget->setItem(i, 0, dateItem);

        // Set the transaction description.
        QTableWidgetItem *descriptionItem = new QTableWidgetItem(transaction->description());
        transactionTableWidget->setItem(i, 1, descriptionItem);

        // Set the transaction category name.
        QTableWidgetItem *categoryItem = new QTableWidgetItem(
            categoryNames[transaction->categoryID()]);
        transactionTableWidget->setItem(i, 2, categoryItem);

        // Get the subcategory name.
        QString subcategoryName = db->getSubcategoryName(transaction->categoryID(),
                                                         transaction->subcategoryID());
        // Set the transaction subcategory name.
        QTableWidgetItem *subcategoryItem = new QTableWidgetItem(subcategoryName);
        transactionTableWidget->setItem(i, 3, subcategoryItem);

        // Set the transaction amount.
        QTableWidgetItem *amountItem = new QTableWidgetItem(
            QString::number(transaction->amount(), 'f', 2));
        transactionTableWidget->setItem(i, 4, amountItem);

        // Set the transaction balance.
        QTableWidgetItem *balanceItem = new QTableWidgetItem(
            QString::number(transaction->balance(), 'f', 2));
        transactionTableWidget->setItem(i, 5, balanceItem);

        // Set the transaction ID.
        QTableWidgetItem *transactionIDItem = new QTableWidgetItem(
            QString::number(transaction->transactionID()));
        transactionTableWidget->setItem(i, 6, transactionIDItem);
    }
}

/**
 * @brief Retrieve the transactions for a specific category.
 */
void MainWindow::loadTransactionsByCategory()
{
    // Get the selected category ID.
    int categoryID = categoryCombo->currentData().toInt();

    // If the selected category is the default option, load all transactions.
    if (categoryID == -1) {
        loadTransactions();
        return;
    }

    // Get database instance.
    Database *db = Database::getInstance();

    // Get the list of transactions.
    QVector<Transaction *> transactions = db->getTransactionsByCategory(m_user->userID(),
                                                                        categoryID);
    // Clear the table.
    transactionTableWidget->clearContents();
    // Set the number of columns in the table widget.
    transactionTableWidget->setColumnCount(6);
    // Set the final column to be hidden from user.
    transactionTableWidget->setColumnHidden(5, true);
    // Set the table headers.
    transactionTableWidget->setHorizontalHeaderLabels(
        {"Date", "Description", "Subcategory", "Amount", "Balance"});
    // Set the number of rows.
    transactionTableWidget->setRowCount(transactions.size());

    // Populate the table.
    for (int i = 0; i < transactions.size(); i++) {
        // Get the current transaction.
        Transaction *transaction = transactions.at(i);

        // Set the transaction date.
        QTableWidgetItem *dateItem = new QTableWidgetItem(transaction->date());
        transactionTableWidget->setItem(i, 0, dateItem);

        // Set the transaction description.
        QTableWidgetItem *descriptionItem = new QTableWidgetItem(transaction->description());
        transactionTableWidget->setItem(i, 1, descriptionItem);

        // Get the subcategory name.
        QString subcategoryName = db->getSubcategoryName(transaction->categoryID(),
                                                         transaction->subcategoryID());
        // Set the transaction subcategory name.
        QTableWidgetItem *subcategoryItem = new QTableWidgetItem(subcategoryName);
        transactionTableWidget->setItem(i, 2, subcategoryItem);

        // Set the transaction amount.
        QTableWidgetItem *amountItem = new QTableWidgetItem(
            QString::number(transaction->amount(), 'f', 2));
        transactionTableWidget->setItem(i, 3, amountItem);

        // Set the transaction balance.
        QTableWidgetItem *balanceItem = new QTableWidgetItem(
            QString::number(transaction->balance(), 'f', 2));
        transactionTableWidget->setItem(i, 4, balanceItem);

        // Set the transaction ID as a hidden item.
        QTableWidgetItem *transactionIDItem = new QTableWidgetItem(
            QString::number(transaction->transactionID()));
        transactionTableWidget->setItem(i, 5, transactionIDItem);
    }
}

/**
 * @brief Add a transaction to the database.
 */
void MainWindow::addTransaction()
{
    // Create the add transaction dialog.
    addTransactionDialog = new AddTransactionDialog(this, m_user->userID());
    // Show the add transaction dialog.
    addTransactionDialog->show();
    // If the add transaction dialog is closed, delete the dialog.
    connect(addTransactionDialog,
            &AddTransactionDialog::finished,
            addTransactionDialog,
            &QObject::deleteLater);
    connect(addTransactionDialog,
            &AddTransactionDialog::transactionAdded,
            this,
            &MainWindow::loadTransactions);
}

/**
 * @brief Delete a transaction from the database.
 */
void MainWindow::deleteTransaction()
{
    // Get the selected row.
    int row = transactionTableWidget->currentRow();
    // If no row is selected, return.
    if (row == -1) {
        return;
    }

    // Calculate column numbers
    int transactionIDColumn = transactionTableWidget->columnCount() - 1;
    int amountColumn = transactionTableWidget->columnCount() - 3;

    // Get the transaction ID.
    int transactionID = transactionTableWidget->item(row, transactionIDColumn)->text().toInt();
    // Get the transaction date.
    QString date = transactionTableWidget->item(row, 0)->text();
    // Get the transaction description.
    QString description = transactionTableWidget->item(row, 1)->text();
    // Get the transaction amount.
    QString amount = transactionTableWidget->item(row, amountColumn)->text();

    // Create the delete transaction dialog.
    deleteTransactionDialog = new DeleteTransactionDialog(transactionID,
                                                          date,
                                                          description,
                                                          amount,
                                                          this);
    // Show the delete transaction dialog.
    deleteTransactionDialog->show();
    // If the delete transaction dialog is closed, delete the dialog.
    connect(deleteTransactionDialog,
            &DeleteTransactionDialog::finished,
            deleteTransactionDialog,
            &QObject::deleteLater);
    connect(deleteTransactionDialog,
            &DeleteTransactionDialog::transactionDeleted,
            this,
            &MainWindow::loadTransactions);
}

/**
 * @brief View a line chart of the current budget data.
 */
void MainWindow::viewLineChart()
{
    // Create the line chart dialog.
    QString currentCategory = categoryCombo->currentText();
    lineChartDialog = new LineChartDialog(transactionTableWidget, currentCategory, this);
    // Show the line chart dialog.
    lineChartDialog->show();
    // If the line chart dialog is closed, delete the dialog.
    connect(lineChartDialog, &LineChartDialog::finished, lineChartDialog, &QObject::deleteLater);
}

/**
 * @brief Create the transaction table widget.
 */
void MainWindow::createTransactionTable()
{
    // Create transaction table widget
    transactionTableWidget = new QTableWidget(this);
    // Set the column headers to stretch to fit size of text.
    transactionTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Set the table to be read only.
    transactionTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // Set the selection mode to select rows.
    transactionTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

/**
 * @brief Setup the layout for the main window UI.
 */
void MainWindow::setupLayout()
{
    // Set window title
    setWindowTitle("Open Budget");

    // Set the default width and height
    setFixedSize(700, 500);

    // Create layouts
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Welcome Label
    welcomeLabel = new QLabel(this);
    welcomeLabel->setFont(QFont("Arial", 16, QFont::Bold)); // Adjust the font and size as needed
    welcomeLabel->setAlignment(Qt::AlignCenter);

    // Add the welcome label to the main layout
    mainLayout->addWidget(welcomeLabel);

    // Group Box for Transaction Table and Category Combo
    QGroupBox *groupBox = new QGroupBox("Transactions");
    QVBoxLayout *groupBoxLayout = new QVBoxLayout();

    // Add Category Combo and View Category Button
    QHBoxLayout *categoryLayout = new QHBoxLayout();

    // Combo Box for Categories
    categoryCombo = new QComboBox();

    // View Category Button
    viewCategoryButton = new QPushButton("View Category");

    // Horizontal spacer for the category layout
    QSpacerItem *categorySpacer = new QSpacerItem(0,
                                                  0,
                                                  QSizePolicy::Expanding,
                                                  QSizePolicy::Minimum);
    categoryLayout->addWidget(categoryCombo);
    categoryLayout->addWidget(viewCategoryButton);
    categoryLayout->addItem(categorySpacer);

    // Add the category layout to the main layout
    groupBoxLayout->addLayout(categoryLayout);

    // Transaction Table
    groupBoxLayout->addWidget(transactionTableWidget);

    // Set the layout for the group box
    groupBox->setLayout(groupBoxLayout);

    // Add group box to the main layout
    mainLayout->addWidget(groupBox);

    // Button Box for Action Buttons
    QGroupBox *buttonBox = new QGroupBox("Actions");
    QHBoxLayout *buttonBoxLayout = new QHBoxLayout();

    // Add Category Button
    addCategoryButton = new QPushButton("Add Category");
    buttonBoxLayout->addWidget(addCategoryButton);

    // Add Subcategory Button
    addSubcategoryButton = new QPushButton("Add Subcategory");
    buttonBoxLayout->addWidget(addSubcategoryButton);

    // Add Transaction Button
    addTransactionButton = new QPushButton("Add Transaction");
    buttonBoxLayout->addWidget(addTransactionButton);

    // Delete Transaction Button
    deleteTransactionButton = new QPushButton("Delete Transaction");
    buttonBoxLayout->addWidget(deleteTransactionButton);

    // View Chart Button
    lineChartButton = new QPushButton("View Chart");
    buttonBoxLayout->addWidget(lineChartButton);

    // Add buttons to the button box
    buttonBox->setLayout(buttonBoxLayout);
    mainLayout->addWidget(buttonBox);

    // Set the main layout for the central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

/**
 * @brief Retrieve user's categories from the database and add them to the category combo box.
 *        Allows user to switch between table views of different categories.
 */
void MainWindow::loadCategories()
{
    // Get user's categories from database
    Database *db = Database::getInstance();
    QMap<int, QString> categories = db->getCategoryNames(m_user->userID());

    // Clear the combo box
    categoryCombo->clear();

    // Add the default option to the combo box
    categoryCombo->addItem("All Categories", -1);

    // Add the Deposit option to the combo box
    categoryCombo->addItem("Deposit", 0);

    // Add the categories and their ID's to the combo box
    for (auto category = categories.begin(); category != categories.end(); ++category) {
        categoryCombo->addItem(category.value(), category.key());
    }
}
