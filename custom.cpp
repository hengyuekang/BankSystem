#include "custom.h"
#include "ui_custom.h"
#include "QDir"
#include "qdebug.h"
#include "mainwindow.h"
#include "QString"
#include <QtCore/QTextStream>
#include "QDate"
extern QString input_account;
QString opening_time = "0";

Custom::Custom(QWidget *parent) : QWidget(parent),
                                  ui(new Ui::Custom)
{
    ui->setupUi(this);
    calInterest();
    readCustominfo();
    showTurnover();
    ui->account_show_label->setText(input_account);
    //    exit,back to login
    connect(ui->exit_pushButton, &QPushButton::released, this, [=]()
            {
                MainWindow *login = new MainWindow();
                this->close();
                login->show();
            });

    //save
    connect(ui->save_pushButton, &QPushButton::clicked, this, [=]()
            {
                saveMoney();
                readCustominfo();
                showTurnover();
            });

    //get
    connect(ui->get_pushButton, &QPushButton::clicked, this, [=]()
            {
                getMoney();
                readCustominfo();
                showTurnover();
            });
}

Custom::~Custom()
{
    delete ui;
}
void Custom::readCustominfo()
{
    //format:account_name/account_type//balance///date
    QString Filepath = QDir::currentPath() + "/account.txt";
    qDebug() << Filepath;
    QFile file(Filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open the file!" << Qt::endl;
        return;
    }
    while (!file.atEnd())
    {
        //            QString tof = 0;
        QByteArray line = file.readLine();
        QString str(line);
        QString s1 = "/";
        QString s2 = "//";
        QString s3 = "///";
        int x1 = str.indexOf(s1);
        int x2 = str.indexOf(s2);
        int x3 = str.indexOf(s3);
        QString account_name = str.mid(0, x1);
        //qDebug()<<account_name;
        if (account_name == input_account)
        {
            QString account_type = str.mid(x1 + 1, x2 - x1 - 1);
            QString balance = str.mid(x2 + 2, x3 - x2 - 2);
            if (account_type == "1")
            {
                ui->type_show_label->setText("saving");
            }
            else if (account_type == "2")
            {
                ui->type_show_label->setText("credit");
            }

            ui->balance_show_label->setText(balance);
        }
    }

    file.close();
}
void Custom::saveMoney()
{
    QString Filepath = QDir::currentPath() + "/turnover.txt";
    // qDebug()<<Filepath;
    QFile file(Filepath);
    if (!file.open(QIODevice::Append))
    {
        qDebug() << "Can't open the file!" << Qt::endl;
        return;
    }
    QTextStream out(&file);
    QString number = ui->money_lineEdit->text();
    QDate currentdate = QDate::currentDate();
    QString str10 = currentdate.toString("yyyy-MM-dd");
    //        1:save
    out << input_account << "/"
        << "1"
        << "//" << number << "///" << str10 << "\n"
        << Qt::endl;
    file.close();

    //update account.txt
    QString Filepath1 = QDir::currentPath() + "/account.txt";
    qDebug() << Filepath;
    QFile file1(Filepath1);
    if (!file1.open(QIODevice::Append))
    {
        qDebug() << "Can't open the file!" << Qt::endl;
        return;
    }
    QTextStream out1(&file1);
    QString number1 = ui->money_lineEdit->text();
    QString number2 = ui->balance_show_label->text();
    float fnum1 = number1.toFloat();
    float fnum2 = number2.toFloat();
    float fnum3 = fnum1 + fnum2;
    if (ui->type_show_label->text() == "saving")
    {
        out1 << input_account << "/"
             << "1"
             << "//" << QString::number(fnum3) << "///" << str10 << "\n"
             << Qt::endl;
    }
    else if (ui->type_show_label->text() == "credit")
    {
        out1 << input_account << "/"
             << "2"
             << "//" << QString::number(fnum3) << "///" << str10 << "\n"
             << Qt::endl;
    }
    file1.close();
}
void Custom::getMoney()
{
    QString Filepath = QDir::currentPath() + "/turnover.txt";
    // qDebug()<<Filepath;
    QFile file(Filepath);
    if (!file.open(QIODevice::Append))
    {
        qDebug() << "Can't open the file!" << Qt::endl;
        return;
    }
    QTextStream out(&file);
    QString number = ui->money_lineEdit->text();
    QDate currentdate = QDate::currentDate();
    QString str10 = currentdate.toString("yyyy-MM-dd");
    out << input_account << "/"
        << "2"
        << "//" << number << "///" << str10 << "\n"
        << Qt::endl;
    file.close();

    QString Filepath1 = QDir::currentPath() + "/account.txt";
    qDebug() << Filepath;
    QFile file1(Filepath1);
    if (!file1.open(QIODevice::Append))
    {
        qDebug() << "Can't open the file!" << Qt::endl;
    }
    QTextStream out1(&file1);
    QString number1 = ui->money_lineEdit->text();
    QString number2 = ui->balance_show_label->text();
    float fnum1 = number1.toFloat();
    float fnum2 = number2.toFloat();
    float fnum3 = fnum2 - fnum1;
    if (ui->type_show_label->text() == "saving")
    {
        out1 << input_account << "/"
             << "1"
             << "//" << QString::number(fnum3) << "///" << str10 << "\n"
             << Qt::endl;
    }
    else if (ui->type_show_label->text() == "credit")
    {
        out1 << input_account << "/"
             << "2"
             << "//" << QString::number(fnum3) << "///" << str10 << "\n"
             << Qt::endl;
    }
    file1.close();
}
void Custom::showTurnover()
{
    //    max:100 row
    int i = 0;
    ui->turnover_tableWidget->setColumnCount(3);
    ui->turnover_tableWidget->horizontalHeader()->setDefaultSectionSize(150);
    ui->turnover_tableWidget->setRowCount(100);

    //table head
    QStringList header;
    header << tr("operation") << tr("count") << tr("date");
    ui->turnover_tableWidget->setHorizontalHeaderLabels(header);
    ui->turnover_tableWidget->horizontalHeader()->setStretchLastSection(true);

    QString Filepath = QDir::currentPath() + "/turnover.txt";
    // qDebug()<<Filepath;
    QFile file(Filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open the file!" << Qt::endl;
        return;
    }
    while (!file.atEnd())
    {
        //            QString tof = 0;
        QByteArray line = file.readLine();
        QString str(line);
        QString s1 = "/";
        QString s2 = "//";
        QString s3 = "///";
        int x1 = str.indexOf(s1);
        int x2 = str.indexOf(s2);
        int x3 = str.indexOf(s3);

        QString account_name = str.mid(0, x1);
        //qDebug()<<account_name;
        //            format:account_name/operation_type//count///date
        if (account_name == input_account)
        {
            QString operation = str.mid(x1 + 1, x2 - x1 - 1);
            QString count = str.mid(x2 + 2, x3 - x2 - 2);
            QString date = str.mid(x3 + 3, 10);

            if (operation == "1")
            {
                operation = "save";
            }
            else if (operation == "2")
            {
                operation = "withdraw";
            }
            i++;
            ui->turnover_tableWidget->setRowCount(i);
            ui->turnover_tableWidget->setItem(i - 1, 0, new QTableWidgetItem(operation));
            ui->turnover_tableWidget->setItem(i - 1, 1, new QTableWidgetItem(count));
            ui->turnover_tableWidget->setItem(i - 1, 2, new QTableWidgetItem(date));
        }
    }

    file.close();
}
void Custom::calInterest()
{
    QDate currentdate = QDate::currentDate();
    QString str1 = currentdate.toString("yyyy-MM-dd");
    //opening time
    QString Filepath = QDir::currentPath() + "/opening1.txt";
    //qDebug()<<Filepath;
    QFile file(Filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Can't open the file!" << Qt::endl;
        return;
    }
    while (!file.atEnd())
    {
        //        QString tof = 0;
        QByteArray line = file.readLine();
        QString str(line);
        QString s1 = "/";
        QString s2 = "//";
        QString s3 = "///";
        //        QString s4 = "////";
        int x1 = str.indexOf(s1);
        int x2 = str.indexOf(s2);
        int x3 = str.indexOf(s3);
        //        int x4 = str.indexOf(s4);
        QString account_name = str.mid(0, x1);
        //        QString type = str.mid(x4+4,1);
        QString type = str.mid(x2 + 2, 1);
        //        save card only
        if (type == "1")
        {
            if (account_name == input_account)
            {
                opening_time = str.mid(x3 + 3, 10);
            }
        }
    }
    file.close();
    if (opening_time != "0")
    {
        //calculate interest
        QString Filepath1 = QDir::currentPath() + "/account.txt";
        //qDebug()<<Filepath;
        QFile file1(Filepath1);
        if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Can't open the file!" << Qt::endl;
            return;
        }
        while (!file1.atEnd())
        {
            //            QString tof = 0;
            QByteArray line = file1.readLine();
            QString str(line);
            QString s1 = "/";
            QString s2 = "//";
            QString s3 = "///";
            int x1 = str.indexOf(s1);
            int x2 = str.indexOf(s2);
            int x3 = str.indexOf(s3);
            QString name = str.mid(0, x1);
            if (name == input_account)
            {
                QString balance = str.mid(x2 + 2, x3 - x2 - 2);
                qDebug() << balance;

                QString year = str1.mid(0, 4);
                QString month = str1.mid(5, 2);
                QString day = str1.mid(8, 2);

                int nyear = 0;
                nyear = year.toInt();
                int nmonth = month.toInt();
                int nday = day.toInt();
                int year1 = opening_time.mid(0, 4).toInt();
                int month1 = opening_time.mid(5, 2).toInt();
                int day1 = opening_time.mid(8, 2).toInt();
                int ben = balance.toInt();
                qDebug() << "balance：" << balance;
                qDebug() << "current date" << nyear << "-" << nmonth << "-" << nday;
                qDebug() << "opening date：" << year1 << "-" << month1 << "-" << day1;

                if ((nyear - year1) * 365 + (6 - month1) * 30 + (30 - day1) >= 30)
                {
                    if (month.toInt() > 6)
                    {
                        nyear = year.toInt();
                    }
                    else
                    {
                        nyear = year.toInt() - 1;
                    }
                    int nmonth = 6;
                    int nday = 30;
                    float money = (ben * 0.5 * ((nyear - year1) * 365 + (nmonth - month1) * 30 + (nday - day1))) / (100 * 360);

                    if (money <= 0)
                    {
                        money = 0;
                    }
                    ui->interest_label1->setText("Interest has been clearing：");
                    ui->interest_show_label->setText(QString::number(money));
                }
            }
        }
        file1.close();
    }
}
