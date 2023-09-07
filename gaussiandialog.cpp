#include "gaussiandialog.h"
#include "ui_gaussiandialog.h"
#include <QValidator>
#include <QMessageBox>

GaussianDialog::GaussianDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GaussianDialog)
{
    ui->setupUi(this);
    QDoubleValidator *dbVali = new QDoubleValidator(this);
    dbVali->setRange(0,1,2);
    ui->lineEditX->setValidator(dbVali);
}

GaussianDialog::~GaussianDialog()
{
    delete ui;
}

double GaussianDialog::getX()
{
    return ui->lineEditX->text().toDouble();
}


bool GaussianDialog::isXEmpty()
{
    return ui->lineEditX->text().isEmpty();
}

void GaussianDialog::clearAll()
{
    ui->lineEditX->clear();
}

void GaussianDialog::accept()
{
    if(isXEmpty())
    {
        QMessageBox::warning(this,"未填完整","填写存在空缺！");
        return;
    }else{

        double x;
        x = this->getX();
        emit acceptEvent(x);
        this->close();
        return;
    }
}

