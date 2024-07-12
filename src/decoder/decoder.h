#ifndef DECODER_H
#define DECODER_H

#include <QWidget>
#include <QRegularExpression>

class Decoder : public QWidget
{
    Q_OBJECT
public:
    explicit Decoder(QWidget *parent = nullptr);

    virtual void Getline() = 0;
    virtual void OutputResult() = 0;
    virtual void Decode() = 0;

    void ConversionError(int pos);
    void InvalidInput(int pos);
    void InvalidFormat();

    bool CheckFormat(const QString& msg, const QRegularExpression& reg);

    QByteArray FillHexByteArray(const QString& msg); 
};

#endif  //  DECODER_H
