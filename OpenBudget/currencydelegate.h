#ifndef CURRENCYDELEGATE_H
#define CURRENCYDELEGATE_H

#include <QStyledItemDelegate>

// Custom delegate for formatting the "Amount" column
class CurrencyDelegate : public QStyledItemDelegate
{
public:
    CurrencyDelegate(QWidget *parent = nullptr);

    QString displayText(const QVariant &value, const QLocale &locale) const override;

    //    void paint(QPainter *painter,
    //               const QStyleOptionViewItem &option,
    //               const QModelIndex &index) const override;
};

#endif // CURRENCYDELEGATE_H
