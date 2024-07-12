#ifndef FORMKORSHUN_H
#define FORMKORSHUN_H

#include "decoder.h"

namespace Ui 
{
    class KorshunProtocol;
}  //  namespace Ui

class KorshunProtocol : public Decoder
{
    Q_OBJECT

public:
    explicit KorshunProtocol(QWidget *parent = nullptr);
    ~KorshunProtocol();

private:
    void SetF();
    void SetS();
    void SetC();
    void Getline() override;
    void SetDataForCodes();
    void OutputResult() override;
    void Decode() override;

    bool MatchCodeF();
    bool MatchCodeS();
    bool MatchCodeC();

private:
    Ui::KorshunProtocol* ui_;
    uint                 integer_value_;
    QRegularExpression   protocol_;
    QString              message_;
    QByteArray           bytes_;
};

#endif //  FORMKORSHUN_H
