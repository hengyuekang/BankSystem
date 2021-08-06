#ifndef CUSTOM_H
#define CUSTOM_H

#include <QWidget>

namespace Ui
{
    class Custom;
}

class Custom : public QWidget
{
    Q_OBJECT

public:
    explicit Custom(QWidget *parent = nullptr);
    void readCustominfo();
    //    save card only!
    void saveMoney();
    void getMoney();
    void showTurnover();
    void calInterest();
    ~Custom();

private:
    Ui::Custom *ui;
};

#endif // CUSTOM_H
