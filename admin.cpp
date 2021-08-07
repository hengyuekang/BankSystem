#include "admin.h"
#include "ui_admin.h"
#include "QDir"
#include "QFile"
#include "QDebug"
#include "QMessageBox"
#include "QString"
#include "QDate"
#include "mainwindow.h"
QString year_select;
QString month_select;
QString day_select;
Admin::Admin(QWidget *parent) : QWidget(parent),
                                ui(new Ui::Admin)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->open_pushButton, &QPushButton::clicked, this, [=]()
            { ui->stackedWidget->setCurrentIndex(1); });

    connect(ui->query_account_pushButton, &QPushButton::clicked, this, [=]()
            { ui->stackedWidget->setCurrentIndex(2); });

    connect(ui->daily_turnover_pushButton, &QPushButton::clicked, this, [=]()
            { ui->stackedWidget->setCurrentIndex(3); });

    connect(ui->set_pushButton, &QPushButton::clicked, this, [=]()
            { openAccount(); });

    connect(ui->query_pushButton, &QPushButton::clicked, this, [=]()
            {
                ui->account_show_label->setText("NULL");
                ui->type_show_label->setText("NULL");
                ui->balance_show_label->setText("NULL");
                ui->turnover_tableWidget->clear();
                queryAccount();
                accountTurnover();
            });

    connect(ui->exit_pushButton, &QPushButton::clicked, this, [=]()
            {
                MainWindow *login = new MainWindow;
                this->close();
                login->show();
            });

    connect(ui->select_date_pushButton, &QPushButton::clicked, this, [=]()
            {
                ui->daily_turnover_showtableWidget->clear();
                year_select = ui->year_comboBox->currentText();
                month_select = ui->month_comboBox->currentText();
                day_select = ui->day_comboBox->currentText();
                dailyTurnover();
            });
}

Admin::~Admin()
{
    delete ui;
}
void Admin::openAccount()
{
    //    opening1:account/password/0:admin,1:saving,2:credit card/date
    //        QString Filepath = QDir::currentPath() + "/opening.txt";
    QString Filepath = QDir::currentPath() + "/opening1.txt";
    // qDebug()<<Filepath;
    QFile file(Filepath);
    if (!file.open(QIODevice::Append))
    {
        qDebug() << "Can't open the file!" << Qt::endl;
        return;
    }
    QTextStream out(&file);
    QDate currentdate = QDate::currentDate();
    QString str10 = currentdate.toString("yyyy-MM-dd");
    //        type:saving
    if (ui->type_comboBox->currentIndex() == 0)
    {
        out << ui->account_input_lineEdit->text() << "/" << ui->password_input_lineEdit->text() << "//"
            << "1"
            << "///" << str10 << "\n"
            << Qt::endl;
    }
    else if (ui->type_comboBox->currentIndex() == 1)
    {
        //            credit card
        out << ui->account_input_lineEdit->text() << "/" << ui->password_input_lineEdit->text() << "//"
            << "2"
            << "///" << str10 << "\n"
            << Qt::endl;
    }

    file.close();
    //update account state
    //    format:account/type/curr money/update date
    QString Filepath1 = QDir::currentPath() + "/account.txt";
    qDebug() << Filepath;
    QFile file1(Filepath1);
    if (!file1.open(QIODevice::Append))
    {
        qDebug() << "Can't open the file!" << Qt::endl;
        return;
    }
    QTextStream out1(&file1);

    if (ui->type_comboBox->currentIndex() == 0)
    {
        out1 << ui->account_input_lineEdit->text() << "/"
             << "1"
             << "//"
             << "0.00"
             << "///" << str10 << "\n"
             << Qt::endl;
    }
    else if (ui->type_comboBox->currentIndex() == 1)
    {
        out1 << ui->account_input_lineEdit->text() << "/"
             << "2"
             << "//"
             << "0.00"
             << "///" << str10 << "\n"
             << Qt::endl;
    }
    file1.close();
    QMessageBox::information(NULL, "Creation", "done", QMessageBox::Ok);
}
void Admin::queryAccount()
{
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
        qDebug() << account_name;
        if (account_name == ui->input_accout_lineEdit->text())
        {
            ui->account_show_label->setText(account_name);
            QString type = str.mid(x1 + 1, x2 - x1 - 1);
            QString balance = str.mid(x2 + 2, x3 - x2 - 2);
            qDebug() << x1;
            qDebug() << x2;
            qDebug() << x3;
            if (type == "1")
            {
                ui->type_show_label->setText("Saving");
            }
            else if (type == "2")
            {
                ui->type_show_label->setText("Credit");
            }

            ui->balance_show_label->setText(balance);
        }
    }

    file.close();
}
void Admin::accountTurnover()
{
    int i = 0;
    ui->turnover_tableWidget->setColumnCount(3);
    ui->turnover_tableWidget->horizontalHeader()->setDefaultSectionSize(150);
    ui->turnover_tableWidget->setRowCount(100);

    //header
    QStringList header;
    header << tr("Operation") << tr("Count") << tr("Date");
    ui->turnover_tableWidget->setHorizontalHeaderLabels(header);
    ui->turnover_tableWidget->horizontalHeader()->setStretchLastSection(true);

    //format:accout/op:1:save,2:get/count/date
    QString Filepath = QDir::currentPath() + "/turnover.txt";
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
        qDebug() << account_name;
        if (account_name == ui->input_accout_lineEdit->text())
        {
            QString operation = str.mid(x1 + 1, x2 - x1 - 1);
            QString count = str.mid(x2 + 2, x3 - x2 - 2);
            QString date = str.mid(x3 + 3, 10);

            if (operation == "1")
            {
                operation = "Save";
            }
            else if (operation == "2")
            {
                operation = "Get";
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
void Admin::dailyTurnover()
{
    float total = 0.00;
    float save = 0.00;
    float get = 0.00;
    int i = 0;
    ui->daily_turnover_showtableWidget->setColumnCount(4);
    ui->daily_turnover_showtableWidget->horizontalHeader()->setDefaultSectionSize(150);
    ui->daily_turnover_showtableWidget->setRowCount(100);

    //header
    QStringList header;
    header << tr("Account") << tr("Operation") << tr("Count") << tr("Date");
    ui->daily_turnover_showtableWidget->setHorizontalHeaderLabels(header);
    ui->daily_turnover_showtableWidget->horizontalHeader()->setStretchLastSection(true);

    QString Filepath = QDir::currentPath() + "/turnover.txt";
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

        //                if(x1!=-1)
        //                {
        QString account_name = str.mid(0, x1);
        QString operation = str.mid(x1 + 1, x2 - x1 - 1);
        QString count = str.mid(x2 + 2, x3 - x2 - 2);
        QString date = str.mid(x3 + 3, 10);
        QString year = str.mid(x3 + 3, 4);
        QString month = str.mid(x3 + 8, 2);
        QString day = str.mid(x3 + 11, 2);
        if (year == year_select && month == month_select && day == day_select)
        {
            float fcount = count.toFloat();

            if (operation == "1")
            {
                operation = "Save";
                total = total + fcount;
                save = save + fcount;

                qDebug() << "Save:" << save;
            }
            else if (operation == "2")
            {
                operation = "Get";
                total = total - fcount;
                get = get + fcount;
                qDebug() << "Get:" << get;
            }
            i++;
            ui->daily_turnover_showtableWidget->setRowCount(i);
            ui->daily_turnover_showtableWidget->setItem(i - 1, 0, new QTableWidgetItem(account_name));
            ui->daily_turnover_showtableWidget->setItem(i - 1, 1, new QTableWidgetItem(operation));
            ui->daily_turnover_showtableWidget->setItem(i - 1, 2, new QTableWidgetItem(count));
            ui->daily_turnover_showtableWidget->setItem(i - 1, 3, new QTableWidgetItem(date));
        }

        //            }
    }

    ui->total_money_show_label->setText(QString::number(total));
    ui->save_money_show_label->setText(QString::number(save));
    ui->get_money_show_label->setText(QString::number(get));
    file.close();
}
