#include "decoder.h"

#include <QMessageBox>

Decoder::Decoder(QWidget *parent) : QWidget(parent)
{

}

void Decoder::ConversionError(int pos)
{
    QMessageBox::warning(this, "Conversion error", QString("Byte pos: ").append(QString::number(pos)));
}

void Decoder::InvalidInput(int pos)
{
    QMessageBox::warning(this, "Invalid input", QString("0-based index: ").append(QString::number(pos)));
}

void Decoder::InvalidFormat()
{
    QMessageBox::warning(this, "Invalid format", "Check protocol compliance");
}

bool Decoder::CheckFormat(const QString& msg, const QRegularExpression& reg)
{
    QRegularExpressionMatch match = reg.match(msg);
    bool hasMatch = match.hasMatch();
    return hasMatch;
}

QByteArray Decoder::FillHexByteArray(const QString& msg)
{
    QRegularExpression prefixPos("([[:xdigit:]]{2})");
    QRegularExpressionMatchIterator iter = prefixPos.globalMatch(msg);
    QByteArray byteArray;
    int i = 0;
    while (iter.hasNext()) 
    {
        QRegularExpressionMatch match = iter.next();
        QString word = match.captured(1);
        byteArray.push_back(QByteArray::fromHex(word.toUtf8()));
        ++i;
    }
    return byteArray;
}