#include "addsubcategorydialog.h"
#include <QMessageBox>
#include "database.h"

/**
 * @brief Allows the user to add a subcategory to the database.
 * 
 * @param userID ID of the user adding the subcategory.
 * @param parent Pointer to the parent widget.
 */
AddSubcategoryDialog::AddSubcategoryDialog(int userID, QWidget *parent)
    : QDialog{parent}
    , m_userID{userID}
{
    setWindowTitle("Add Category");

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Category Name Group Box
    QGroupBox *groupBox = new QGroupBox("Subcategory Name");
    QHBoxLayout *layout = new QHBoxLayout();

    // Line edit for subcategory name
    subcategoryNameLineEdit = new QLineEdit();

    // Combo box for category names
    categoryCombo = new QComboBox();

    // Add default item to categoryCombo
    categoryCombo->addItem("Select a category", -1);

    // Load categories from the database
    loadCategories();

    // Add widgets to the layout
    layout->addWidget(subcategoryNameLineEdit);
    layout->addWidget(categoryCombo);

    // Set layout for the group box
    groupBox->setLayout(layout);

    // Button Box
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);
    QPushButton *addButton = buttonBox->button(QDialogButtonBox::Ok);

    // Set the text for the add button
    addButton->setText("Add");

    // Connect the 'Add' button to a custom slot (you need to implement the slot)
    connect(addButton, &QPushButton::clicked, this, &AddSubcategoryDialog::addSubcategory);

    // Connect the cancel button to the reject slot
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Add the group box and button box to the main layout
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(buttonBox);

    // Set the main layout for the dialog
    setLayout(mainLayout);
}

/**
 * @brief Attempts to insert a new subcategory into the database.
 */
void AddSubcategoryDialog::addSubcategory()
{
    // Get the subcategory name from the line edit
    QString subcategoryName = subcategoryNameLineEdit->text();

    // Get the category ID from the combo box
    int categoryID = categoryCombo->currentData().toInt();

    // Create a database instance
    Database *db;
    db = Database::getInstance();

    // Check if the category name is empty
    if (subcategoryName.isEmpty()) {
        // Display an error message
        QMessageBox::critical(this, "Error", "Subcategory name cannot be empty.");
        return;
    }

    // Check if the subcategory name already exists
    QMap<int, QString> subcategories = db->getSubcategoryNames(m_userID, categoryID);
    if (subcategories.values().contains(subcategoryName)) {
        // Clear the line edit
        subcategoryNameLineEdit->clear();
        // Display an error message
        QMessageBox::critical(this, "Error", "Subategory name already exists.");
        return;
    }

    // Add the category to the database
    if (db->createSubcategory(subcategoryName, m_userID, categoryID)) {
        // If the category was added successfully, show a success message
        QMessageBox::information(this, "Success", "Category added.");
        // Emit the categoryAdded signal
        emit subcategoryAdded();
        // Close the dialog
        accept();
    } else {
        // Display an error message
        QMessageBox::critical(this, "Error", "Category could not be added.");
    }
}

/**
 * @brief Retrieves the user's categories from the database and 
 *        adds them to the category combo box.
 */
void AddSubcategoryDialog::loadCategories()
{
    // Get user's categories from database
    Database *db = Database::getInstance();
    QMap<int, QString> categories = db->getCategoryNames(m_userID);

    // Clear the combo box
    categoryCombo->clear();

    // Add a default item
    categoryCombo->addItem("Select a category", -1);

    // Add Deposit item
    categoryCombo->addItem("Deposit", 0);

    // Add the categories and their ID's to the combo box
    for (auto category = categories.begin(); category != categories.end(); ++category) {
        categoryCombo->addItem(category.value(), category.key());
    }
}
