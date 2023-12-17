#include "addcategorydialog.h"
#include <QMessageBox>
#include "database.h"

/**
 * @brief Allows user to add a category to the database.
 * 
 * @param userID ID of the user adding the category.
 * @param parent Pointer to the parent widget.
 */
AddCategoryDialog::AddCategoryDialog(int userID, QWidget *parent)
    : QDialog{parent}
    , m_userID{userID}
{
    setWindowTitle("Add Category");

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Category Name Group Box
    QGroupBox *groupBox = new QGroupBox("Category Name");
    QHBoxLayout *layout = new QHBoxLayout();

    // Line edit for category name
    categoryNameLineEdit = new QLineEdit();

    // Add widgets to the layout
    layout->addWidget(categoryNameLineEdit);

    // Set layout for the group box
    groupBox->setLayout(layout);

    // Button Box
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);
    QPushButton *addButton = buttonBox->button(QDialogButtonBox::Ok);

    // Set the text for the add button
    addButton->setText("Add");

    // Connect the 'Add' button to a custom slot (you need to implement the slot)
    connect(addButton, &QPushButton::clicked, this, &AddCategoryDialog::addCategory);
    // Connect the 'Cancel' button to the reject slot of the dialog
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Add the group box and button box to the main layout
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(buttonBox);

    // Set the main layout for the dialog
    setLayout(mainLayout);
}

/**
 * @brief Attempts to insert a new category into the database.
 */
void AddCategoryDialog::addCategory()
{
    // Get the category name from the line edit
    QString categoryName = categoryNameLineEdit->text();

    // Create a database instance
    Database *db;
    db = Database::getInstance();

    // Check if the category name is empty
    if (categoryName.isEmpty()) {
        // Display an error message
        QMessageBox::critical(this, "Error", "Category name cannot be empty.");
        return;
    }

    // Check if the category name already exists
    QMap<int, QString> categories = db->getCategoryNames(m_userID);
    if (categories.values().contains(categoryName)) {
        // Clear the line edit
        categoryNameLineEdit->clear();
        // Display an error message
        QMessageBox::critical(this, "Error", "Category name already exists.");
        return;
    }

    // Add the category to the database
    if (db->createCategory(categoryName, m_userID)) {
        // If the category was added successfully, show a success message
        QMessageBox::information(this, "Success", "Category added.");
        // Emit the categoryAdded signal
        emit categoryAdded();
        // Close the dialog
        accept();
    } else {
        // Display an error message
        QMessageBox::critical(this, "Error", "Category could not be added.");
    }
}
