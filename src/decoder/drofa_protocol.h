#ifndef FORMDROFA_H
#define FORMDROFA_H

#include "decoder.h"

namespace Ui 
{
    class DrofaProtocol;
}  //  namespace Ui

class DrofaProtocol : public Decoder
{
    Q_OBJECT

public:
    explicit DrofaProtocol(QWidget *parent = nullptr);
    ~DrofaProtocol();

private:
    void SetF();
    void SetM();
    void SetP();
    void Getline() override;
    void OutputResult() override;
    void Decode() override;

    bool FirstF();
    bool FirstM();
    bool FirstP();
    bool isF(int pos);
    bool isM(int pos);
    bool isP(int pos);

private:
    Ui::DrofaProtocol* ui_;
    uint               integer_value_;
    QRegularExpression protocol_;
    QString            message_;
    QByteArray         bytes_;
};

#endif  //  FORMDROFA_H
