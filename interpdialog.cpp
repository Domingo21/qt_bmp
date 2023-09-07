#include "interpdialog.h"
#include "ui_interpdialog.h"
#include <QDebug>
#include <QValidator>
#include <QMessageBox>

InterpDialog::InterpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InterpDialog)
{
    ui->setupUi(this);
    QDoubleValidator *dbVali = new QDoubleValidator(this);
    dbVali->setDecimals(2);
    dbVali->setBottom(0);
    ui->lineEditX->setValidator(dbVali);
    ui->lineEditY->setValidator(dbVali);

}

InterpDialog::~InterpDialog()
{
    delete ui;
}

double InterpDialog::getX()
{
    return ui->lineEditX->text().toDouble();
}

double InterpDialog::getY()
{
    return ui->lineEditY->text().toDouble();
}

bool InterpDialog::isXEmpty()
{
    return ui->lineEditX->text().isEmpty();
}

bool InterpDialog::isYEmpty()
{
    return ui->lineEditY->text().isEmpty();
}

bool InterpDialog::isChoosed()
{
    bool near;
    bool du;
    near = ui->radioButtonNear->isChecked();
    du = ui->radioButtonDu->isChecked();
    if(!near && !du)
    {
        return false;
    }

    return true;
}

void InterpDialog::clearAll()
{
    ui->lineEditX->clear();
    ui->lineEditY->clear();
}

void InterpDialog::accept()
{
    if(isXEmpty() || isYEmpty())
    {
        QMessageBox::warning(this,"未填完整","填写存在空缺！");
        return;
    }else{
        if(!isChoosed())
        {
            QMessageBox::warning(this,"未有勾选","请选择一种缩放方式");
            return;
        }
        double x,y;
        x = this->getX();
        y = this->getY();
        int mode = 0;
        if(!ui->radioButtonNear->isChecked())
        {
            mode = 1;
        }
        emit acceptEvent(x,y,mode);
        this->close();
        return;
    }
}


