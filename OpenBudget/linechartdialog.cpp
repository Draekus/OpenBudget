#include "linechartdialog.h"

/**
 * @brief Displays a line chart of the transaction data the user was currently viewing.
 * @param parent The parent widget.
 * @param transactionTableWidget The transaction table containing the data to chart.
 */
LineChartDialog::LineChartDialog(QTableWidget *transactionTableWidget,
                                 QString currentCategory,
                                 QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle("Transaction Data Analysis");

    // Resize the window
    resize(1080, 720);

    // Create a chart and a view for the chart
    chart = new QChart();
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Create a line series for the data
    lineSeries = new QLineSeries();

    // Calculate column numbers for date and balance
    int dateColumn = 0;
    int balanceColumn = transactionTableWidget->columnCount() - 2;
    // Iterate over transactionTableWidget rows and add data to the line series
    for (int row = 0; row < transactionTableWidget->rowCount(); ++row) {
        // Extract the date and balance from the table
        QTableWidgetItem *dateItem = transactionTableWidget->item(row, dateColumn);
        QTableWidgetItem *balanceItem = transactionTableWidget->item(row, balanceColumn);
        // If the date and balance are not NULL
        if (dateItem && balanceItem) {
            // Convert the date string to a QDateTime object
            QDateTime date = QDateTime::fromString(dateItem->text(),
                                                   "MM/dd/yyyy"); // Adjust the date format as needed
            // Convert the balance string to a double
            double balance = balanceItem->text().toDouble();
            // Add the date and balance to the line series
            lineSeries->append(date.toMSecsSinceEpoch(), balance);
        }
    }

    // Set up the chart axes
    QDateTimeAxis *dateAxis = new QDateTimeAxis;
    dateAxis->setFormat("MM/dd"); // Customize the date format as needed
    dateAxis->setTitleText("Date");
    QValueAxis *balanceAxis = new QValueAxis;
    balanceAxis->setTitleText("Balance");

    // Add axes to the chart
    chart->addAxis(dateAxis, Qt::AlignBottom);
    chart->addAxis(balanceAxis, Qt::AlignLeft);

    // Attach the series to the axes
    chart->addSeries(lineSeries);
    lineSeries->attachAxis(dateAxis);
    lineSeries->attachAxis(balanceAxis);

    // Set the chart title
    chart->setTitle(currentCategory + ": Balance Over Time Chart");

    // Set up the layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(chartView);

    setLayout(mainLayout);
}
