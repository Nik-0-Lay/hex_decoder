#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class DrofaProtocol;
class KorshunProtocol;

namespace Ui 
{
    class Window;
}  //  namespace Ui

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();

private:
    Ui::Window*      ui_;
    DrofaProtocol*   drofa_ui_;
    KorshunProtocol* korshun_ui_;
};

#endif  //  WINDOW_H
