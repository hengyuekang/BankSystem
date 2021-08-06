#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "qdebug.h"
#include "QFile"
#include <qdir.h>
#include "custom.h"
#include "admin.h"
#include "qmessagebox.h"
#include "QDate"
QString input_account;
//opening:opening info of accout;accout/password//type///date////type_info
//account:update current state of accout;account/type//balance////update_date
//turnover:account/operation//count///date
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->login_pushButton,&QPushButton::clicked,this,[=]()
    {
        input_account = ui->account_lineEdit->text();
        QString password = ui->password_input_lineEdit->text();
        bool trueorfalse = ui->checkBox->isChecked();
       int result = loginAccount(input_account,password,trueorfalse);

       if(result == 1)
       {
           this->close();
           Custom *user = new Custom;
           user->show();
       }
       else if(result == 2)
       {
           this->close();
           Admin *admin = new Admin;
           admin->show();
       }
       else
       {
           QMessageBox::warning(NULL,"Warning","wrong input",QMessageBox::Ok);
       }


    });

    connect(ui->exit_pushButton,&QPushButton::released,this,[=]()
    {
       this->close();
    }
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}
int MainWindow::loginAccount(QString str1,QString str2,bool trueorfalse)
{
    int res = 0;

        QString Filepath = QDir::currentPath() + "/opening.txt";
        qDebug()<<Filepath;
        QFile file(Filepath);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<"Can't open the file!"<<endl;
        }
        while(!file.atEnd()) {
            QString tof = 0;
            QByteArray line = file.readLine();
            QString str(line);
            QString s1 = "/";
            QString s2 = "//";
            QString s3 = "///";
            int x1 = str.indexOf(s1);
            int x2 = str.indexOf(s2);
            int x3 = str.indexOf(s3);

            if(trueorfalse ==true)
            {
                tof = "3";
            }
            else if(trueorfalse == false)
            {
                tof = "2";
            }

            QString account_name = str.mid(0,x1);
            QString password = str.mid(x1+1,x2-x1-1);
            QString is_admin = str.mid(x2+2,1);

            //custom
            if(str1 == account_name && str2 == password && tof== is_admin&& tof == "2")
            {
                file.close();
                res = 1;
                return res;
            }
            //admin
            else if(str1 == account_name&&str2 ==password&&tof== is_admin&& tof == "3")
            {
                file.close();
                res = 2;
                return res;
            }
            else
            {
                res = 0;
            }
        }

        file.close();
        return res;
}
