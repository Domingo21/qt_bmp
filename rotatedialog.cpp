#include "rotatedialog.h"
#include "ui_rotatedialog.h"
#include <QDebug>
#include <QValidator>
#include <QMessageBox>

RotateDialog::RotateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RotateDialog)
{
    ui->setupUi(this);
    QDoubleValidator *dbVali = new QDoubleValidator(this);
    dbVali->setDecimals(2);
    dbVali->setBottom(0);
    ui->lineEditA->setValidator(dbVali);
}


RotateDialog::~RotateDialog()
{
    delete ui;
}

int RotateDialog::getA()
{
    return ui->lineEditA->text().toDouble();
}

bool RotateDialog::isXEmpty()
{
    return ui->lineEditA->text().isEmpty();
}

void RotateDialog::clearAll()
{
    ui->lineEditA->clear();
}

void RotateDialog::accept()
{
    if(isXEmpty())
    {
        QMessageBox::warning(this,"未填完整","填写存在空缺！");
        return;
    }
    else
    {
        double angle;
        angle = this->getA();
        emit acceptEvent(angle);
        this->close();
        return;
    }
}
