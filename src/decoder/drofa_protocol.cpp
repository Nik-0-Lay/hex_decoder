#include "drofa_protocol.h"
#include "ui_drofa_protocol.h"

#include <QPushButton>

DrofaProtocol::DrofaProtocol(QWidget *parent) :
    Decoder(parent),
    ui_(new Ui::DrofaProtocol),
    protocol_ {
        R"(^)"
        R"((?:(?:0x|0X)?(?:FF|ff)[[:space:]]){2})"                  //  "FF FF " lower- or uppercase with optional 0x or 0X prefix
        R"((?:(?:0x|0X)?(?:[[:xdigit:]]{2})[[:space:]]){1,4})"      //  1 to 4 "hex " with optional 0x or 0X prefix
        R"((?:(?:0x|0X)?(?:EF|ef)[[:space:]](?:0x|0X)?(?:EF|ef)))"  //  "EF EF" lower- or uppercase with optional 0x or 0X prefix
        R"($)"
    },
    message_{},  integer_value_{}, bytes_{}

{
    ui_->setupUi(this);
    ui_->tableWidgetDrofa->setStyleSheet("QTableView{border : 1px solid black}");
    connect(ui_->pushButtonDrofa, &QPushButton::clicked, this, &DrofaProtocol::Decode);
}

DrofaProtocol::~DrofaProtocol()
{
    delete ui_;
}

bool DrofaProtocol::FirstF()
{
    if(isF(2))  //  F is the first, the sequence is: F|FM|FMP
    {
        if (bytes_.size() == 6)
        {
            SetF();
        }
        else if(bytes_.size() == 7)
        {
            SetF();
            if (isM(4))
            {
                SetM();
            }
            else
            {
                InvalidInput(4);
            }
        } 
        else if(bytes_.size() == 8)
        {
            SetF();
            if (isM(4))
            {
                SetM();
            }
            else
            {
                InvalidInput(4);
            }
            if (isP(5))
            {
                SetP();
            }
            else
            {
                InvalidInput(5);
            }
        }
        return true;
    } 
    else
    {
        return false;
    }
}

bool DrofaProtocol::FirstM()
{
    if(isM(2))  //  M is the first, the sequence is M|MP
    {
        if(bytes_.size() == 5)
        {
            SetM();
        } 
        else if(bytes_.size() == 6)
        {
            SetM();
            if(isP(3))
            {
                SetP();
            } 
            else
            {
                InvalidInput(3);
            }
        }
        return true;
    } 
    else
    {
        return false;
    }
}

bool DrofaProtocol::FirstP()
{
        if(isP(2))  //  P is the only byte for device state
        {
            SetP();
            return true;
        }
        else
        {
            return false;
        }
}

bool DrofaProtocol::isF(int pos)
{
    bool ok;
    integer_value_ = bytes_.sliced(pos,2).toHex().toUInt(&ok, 16);
    if(ok)
    {
        return (integer_value_ >= 5000) && (integer_value_ <= 50000) && (bytes_.size() != 5);
    } 
    else
    {
        ConversionError(pos);
        return false;
    }
}

bool DrofaProtocol::isM(int pos)
{
    bool ok;
    integer_value_ = bytes_.sliced(pos,1).toHex().toUInt(&ok, 16);
    if(ok)
    {
        return integer_value_ == 1 || integer_value_ == 2 || integer_value_ == 10;
    } 
    else
    {
        ConversionError(pos);
        return false;
    }
}

bool DrofaProtocol::isP(int pos)
{
    bool ok;
    integer_value_ = bytes_.sliced(pos,1).toHex().toUInt(&ok, 16);
    if(ok)
    {
        return (integer_value_ % 10 == 0) && (integer_value_ >= 0) && (integer_value_ <= 100);
    } 
    else
    {
        ConversionError((pos));
        return false;
    }
}

void DrofaProtocol::SetF()
{
    ui_->tableWidgetDrofa->item(0,0)->setText(QString::number(integer_value_));
}

void DrofaProtocol::SetM()
{
    if (integer_value_ == 1)
    {
        ui_->tableWidgetDrofa->item(0, 1)->setText("A1");
    }
    if (integer_value_ == 2)
    {
        ui_->tableWidgetDrofa->item(0, 1)->setText("A2");
    }
    if (integer_value_ == 10)
    {
        ui_->tableWidgetDrofa->item(0, 1)->setText("F5R");
    }
}

void DrofaProtocol::SetP()
{
    ui_->tableWidgetDrofa->item(0,2)->setText(QString::number(integer_value_));
}

void DrofaProtocol::Getline()
{
    message_ = {ui_->lineEditDrofa->text()};
}

void DrofaProtocol::OutputResult()
{
    for(int i = 0; i < 3; ++i)  //  clear table
    {
        ui_->tableWidgetDrofa->item(0,i)->setText("");
    }
    //  If the first byte in message is malformed - output error
    if (!(FirstF() || FirstM() || FirstP()))
    {
        InvalidInput(2);
    }
}

void DrofaProtocol::Decode()
{
    Getline();
    if(CheckFormat(message_, protocol_))
    {
        bytes_ = {FillHexByteArray(message_)};
        OutputResult();
    } 
    else
    {
        InvalidFormat();
    }
}
