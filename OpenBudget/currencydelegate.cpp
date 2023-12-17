#include "currencydelegate.h"

CurrencyDelegate::CurrencyDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{}

QString CurrencyDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    // Format the amount to display two decimal places
    return locale.toString(value.toDouble(), 'f', 2);
}
