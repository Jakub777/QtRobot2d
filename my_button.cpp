#include <QPainter>
#include <QPushButton>

class MyButton : public QPushButton
{
public:
    using QPushButton::QPushButton;

protected:
    void paintEvent(QPaintEvent *event) override;
};