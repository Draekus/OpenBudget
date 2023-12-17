#include <QDateTime>
#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

class LineChartDialog : public QDialog
{
    Q_OBJECT

public:
    LineChartDialog(QTableWidget *transactionTableWidget,
                    QString currentCategory = "",
                    QWidget *parent = nullptr);

private:
    QChart *chart = nullptr;
    QChartView *chartView = nullptr;
    QLineSeries *lineSeries = nullptr;
};
