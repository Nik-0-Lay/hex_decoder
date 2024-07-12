#include "window.h"
#include "ui_window.h"

#include "drofa_protocol.h"
#include "korshun_protocol.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::Window)
{
    ui_->setupUi(this);
    drofa_ui_ = new DrofaProtocol(this);
    korshun_ui_ = new KorshunProtocol(this);

    ui_->gridLayout->addWidget(drofa_ui_);
    ui_->gridLayout->addWidget(korshun_ui_);
}

Window::~Window()
{
    delete ui_;
}
