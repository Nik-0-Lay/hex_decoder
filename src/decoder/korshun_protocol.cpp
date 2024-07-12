#include "korshun_protocol.h"
#include "ui_korshun_protocol.h"

#include <QPushButton>

KorshunProtocol::KorshunProtocol(QWidget *parent) :
    Decoder(parent),
    ui_(new Ui::KorshunProtocol),
    protocol_{
        R"(^)"
        R"((?:(?:0x|0X)?(?:3A|3a)[[:space:]]))"                  //  "3A " lower or uppercase with optional 0x or 0X prefix
        R"((?:(?:0x|0X)?(?:[[:xdigit:]]{2})[[:space:]]){1,10})"  //  1 to 10 "hex " with optional 0x or 0X prefix
        R"((?:(?:0x|0X)?(?:21)))"                                //  "21" lower or uppercase with optional 0x or 0X prefix
        R"($)"
    },
    message_{},  integer_value_{}, bytes_{}
{
   ui_->setupUi(this);
   ui_->tableWidgetKorshun ->setStyleSheet("QTableView{border : 1px solid black}");
   connect(ui_->pushButtonKorshun, &QPushButton::clicked, this, &KorshunProtocol::Decode);
}

KorshunProtocol::~KorshunProtocol()
{
    delete ui_;
}

bool KorshunProtocol::MatchCodeF()
{        
    return (integer_value_ >= 100) && (integer_value_ <= 5000) && (integer_value_ % 10);
}

bool KorshunProtocol::MatchCodeS()
{
    return (integer_value_ >= 1) && (integer_value_ <= 3);  //  S|s
}

bool KorshunProtocol::MatchCodeC()
{
    return (integer_value_ >= 1) && (integer_value_ <= 16);  //  C|c
}

void KorshunProtocol::SetF()
{
    if(integer_value_ < 1000)
    {
        ui_->tableWidgetKorshun->item(0,0)->setText("0" + QString::number(integer_value_));
    } 
    else
    {
        ui_->tableWidgetKorshun->item(0, 0)->setText(QString::number(integer_value_));
    }
}

void KorshunProtocol::SetS()
{
    if (integer_value_ == 1)
    {
        ui_->tableWidgetKorshun->item(0, 1)->setText("1");
    }
    if (integer_value_ == 2)
    {
        ui_->tableWidgetKorshun->item(0, 1)->setText("2");
    }
    if (integer_value_ == 3)
    {
        ui_->tableWidgetKorshun->item(0, 1)->setText("3");
    }
}

void KorshunProtocol::SetC()
{
    if (integer_value_ < 10)
    {
        ui_->tableWidgetKorshun->item(0, 2)->setText("0" + QString::number(integer_value_));
    }
    else
    {
        ui_->tableWidgetKorshun->item(0, 2)->setText(QString::number(integer_value_));
    }
}

void KorshunProtocol::Getline()
{
    message_ = {ui_->lineEditKorshun->text()};
}

void KorshunProtocol::SetDataForCodes()
{
    bool ok{};
    int pos{};
    //  Convert 4 bytes after code F to int
    if(((pos = bytes_.indexOf("F")) > 0) && (pos + 4 < bytes_.size()))
    {
        integer_value_ = bytes_.sliced(pos + 1, 4).toUInt(&ok, 16);
        if (MatchCodeF())
        {
            SetF();
        }
        else
        {
            InvalidInput(pos);
        }
    } 
    else
    {
        InvalidFormat();
    }
    //  Convert 1 bytes after code S to int
    if(((pos = bytes_.indexOf("S")) > 0) && (pos + 1 < bytes_.size()))
    {
        integer_value_ = bytes_.sliced(pos + 1, 1).toUInt(&ok, 16);
        if (MatchCodeS())
        {
            SetS();
        }
        else
        {
            InvalidInput(pos);
        }
    }  
    else
    {
        InvalidFormat();
    }
    //  Convert 2 bytes after code C to int
    if(((pos = bytes_.indexOf("C")) > 0) && (pos + 2 < bytes_.size()))
    {
        integer_value_ = bytes_.sliced(pos + 1, 2).toUInt(&ok, 16);
        if (MatchCodeC())
        {
            SetC();
        }
        else
        {
            InvalidInput(pos);
        }
    } 
    else 
    {
        InvalidFormat();
    }
}

void KorshunProtocol::OutputResult()
{
    for(int i = 0; i < 3; ++i)  //  clear table
    {
        ui_->tableWidgetKorshun->item(0,i)->setText("");
    }
    SetDataForCodes();
}

void KorshunProtocol::Decode()
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

