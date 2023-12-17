#include "addtransactiondialog.h"
#include <QMessageBox>
#include "database.h"
#include "transaction.h"

/**
 * @brief Allows the user to add a new transaction to the database.
 * 
 * @param parent Pointer to the parent widget.
 * @param userID ID of the user adding the transaction.
 */
AddTransactionDialog::AddTransactionDialog(QWidget *parent, int userID)
    : QDialog{parent}
    , m_userID{userID}
{
    setWindowTitle("Add Transaction");

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Transaction Details Group Box
    QGroupBox *groupBox = new QGroupBox("Transaction Details");
    QFormLayout *formLayout = new QFormLayout();

    // Widgets for the form
    amountLineEdit = new QLineEdit();
    descriptionTextEdit = new QTextEdit();
    dateEdit = new QDateEdit(QDate::currentDate());
    categoryCombo = new QComboBox();
    subcategoryComboBox = new QComboBox();

    // Set input constraints for amountLineEdit
    QValidator *validator = new QDoubleValidator(0.0, 1000000.0, 2, amountLineEdit);
    amountLineEdit->setValidator(validator);

    // Add categories and subcategories to the combo boxes (replace with actual data)
    loadCategories();
    // Disable subcategory combobox until a category is selected
    subcategoryComboBox->setEnabled(false);

    // Add widgets to the form layout
    formLayout->addRow("Amount:", amountLineEdit);
    formLayout->addRow("Description:", descriptionTextEdit);
    formLayout->addRow("Date:", dateEdit);
    formLayout->addRow("Category:", categoryCombo);
    formLayout->addRow("Subcategory:", subcategoryComboBox);

    // Set layout for the group box
    groupBox->setLayout(formLayout);

    // Button Box
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);
    addButton = buttonBox->button(QDialogButtonBox::Ok);

    // Set the text for the add button
    addButton->setText("Add");

    // Disable the add button until the amount and description are not empty and category is valid
    addButton->setEnabled(false);

    // Connect the 'Add' button to a custom slot (you need to implement the slot)
    connect(addButton, &QPushButton::clicked, this, &AddTransactionDialog::addTransaction);
    // Connect the categoryComboBox's currentIndexChanged signal to a slot to enable/disable subcategoryComboBox
    connect(categoryCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &AddTransactionDialog::categoryChanged);
    // Connect the amountLineEdit's textChanged signal to a slot to enable/disable the add button
    connect(amountLineEdit, &QLineEdit::textChanged, this, &AddTransactionDialog::enableAddButton);
    // Connect the descriptionTextEdit's textChanged signal to a slot to enable/disable the add button
    connect(descriptionTextEdit,
            &QTextEdit::textChanged,
            this,
            &AddTransactionDialog::enableAddButton);
    // Connect the buttonBox's rejected signal to a slot to close the dialog
    connect(buttonBox,
            &QDialogButtonBox::rejected,
            this,
            &AddTransactionDialog::cancelAddTransaction);

    // Add the group box and button box to the main layout
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(buttonBox);

    // Set the main layout for the dialog
    setLayout(mainLayout);
}

/**
 * @brief Attempts to insert the user's transaction into the database.
 */
void AddTransactionDialog::addTransaction()
{
    // Get the transaction values from the widgets
    double amount = amountLineEdit->text().toDouble();
    QString description = descriptionTextEdit->toPlainText();
    QString date = dateEdit->date().toString("MM/dd/yyyy");
    int category = categoryCombo->currentData().toInt();
    int subcategory = subcategoryComboBox->currentData().toInt();
    bool isDeposit = false;

    // If category is 1, then it is a deposit
    if (category == 0) {
        isDeposit = true;
    }
    // If subcategory is -1, then set it to 0
    if (subcategory == -1) {
        subcategory = 0;
    }

    // Add the transaction to the database
    Database *db = Database::getInstance();
    Transaction *transaction = db->createTransaction(amount,
                                                     description,
                                                     date,
                                                     category,
                                                     subcategory,
                                                     m_userID,
                                                     isDeposit);
    // If the transaction was added successfully
    if (transaction != nullptr) {
        // Create a message box to notify the user.
        QMessageBox::information(this, "Success", "The transaction was added successfully.");
        // Emit the transactionAdded signal
        emit transactionAdded();
        // Close the dialog
        accept();
    } else {
        // If the transaction was not added successfully, display an error message
        QMessageBox::critical(this,
                              "Error",
                              "The transaction could not be added. Please try again.");
    }
}

/**
 * @brief Close the dialog when the cancel button is clicked.
 */
void AddTransactionDialog::cancelAddTransaction()
{
    // Close the dialog
    reject();
}

/**
 * @brief Retrieves the user's categories from the database and 
 *        adds them to the category combo box.
 */
void AddTransactionDialog::loadCategories()
{
    // Get user's categories from database
    Database *db = Database::getInstance();
    QMap<int, QString> categories = db->getCategoryNames(m_userID);

    // Clear the combo box
    categoryCombo->clear();

    // Add a default item
    categoryCombo->addItem("Select a category", -1);
    // Add deposit category
    categoryCombo->addItem("Deposit", 0);

    // Add the categories and their ID's to the combo box
    for (auto category = categories.begin(); category != categories.end(); ++category) {
        categoryCombo->addItem(category.value(), category.key());
    }
}

/**
 * @brief When the user selects a category, enable/disable the subcategory combo box and add button.
 */
void AddTransactionDialog::categoryChanged()
{
    // Get the category ID from the combo box
    int categoryID = categoryCombo->currentData().toInt();

    // Enable the add button
    enableAddButton();

    // If the index is 0 or less, then the user selected the default item or deposit
    if (categoryID <= 0) {
        // Disable the subcategory combo box
        subcategoryComboBox->setEnabled(false);
    } else {
        // Enable the subcategory combo box
        subcategoryComboBox->setEnabled(true);

        // Load the subcategories for the selected category
        loadSubcategories(categoryID);
    }
}

/**
 * @brief Retrieves the subcategories for the given category ID from the database and
 *        add them to the subcategory combo box.
 *        
 * @param categoryID The ID of the category to get subcategories for.
 */
void AddTransactionDialog::loadSubcategories(int categoryID)
{
    // Get user's subcategories from database
    Database *db = Database::getInstance();
    QMap<int, QString> subcategories = db->getSubcategoryNames(m_userID, categoryID);

    // Clear the combo box
    subcategoryComboBox->clear();

    // Add a default item
    subcategoryComboBox->addItem("None", -1);

    // Add the subcategories and their ID's to the combo box
    for (auto subcategory = subcategories.begin(); subcategory != subcategories.end();
         ++subcategory) {
        subcategoryComboBox->addItem(subcategory.value(), subcategory.key());
    }
}

/**
 * @brief Enables the add button if the amount and description are not empty and category is valid.
 */
void AddTransactionDialog::enableAddButton()
{
    // Get the values from the widgets
    QString description = descriptionTextEdit->toPlainText();
    int category = categoryCombo->currentData().toInt();

    // Enable the add button if the amount and description are not empty and category is valid
    if (!amountLineEdit->text().isEmpty() && !description.isEmpty() && category >= 0) {
        addButton->setEnabled(true);
    } else {
        addButton->setEnabled(false);
    }
}
