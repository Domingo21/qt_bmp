#include "setpixeldialog.h"
#include "ui_setpixeldialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QValidator>

bool between0to255(int num){
    if(num > 255 || num <0)
    {
        return false;
    }else
    {
        return true;
    }
}

bool SetPixelDialog::inRange(int x, int y){
    if(x<maxX && y <maxY)
        return true;
    else
        return false;
}

SetPixelDialog::SetPixelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetPixelDialog)
{
    ui->setupUi(this);
    ui->lineEditR->setValidator(new QIntValidator(0,255,this));
    ui->lineEditG->setValidator(new QIntValidator(0,255,this));
    ui->lineEditB->setValidator(new QIntValidator(0,255,this));
    QIntValidator *intVali = new QIntValidator(this);
    intVali->setBottom(0);
    ui->lineEditX->setValidator(intVali);
    ui->lineEditY->setValidator(intVali);
}

SetPixelDialog::~SetPixelDialog()
{
    delete ui;
}

void SetPixelDialog::accept()
{
    int r,g,b,x,y;
    if(!this->isXEmpty() && !this->isYEmpty() &&
            !this->isREmpty() && !this->isGEmpty() && !this->isBEmpty())
    {
        this->getRgb(&r,&g,&b);
        x = this->getX();
        y = this->getY();


        if(inRange(x,y) )
        {
            emit acceptEvent(r,g,b,x,y);
            QString msg = tr("点(%1,%2)的像素值RGB已经改为(%3,%4,%5)")
                    .arg(x).arg(y).arg(r).arg(g).arg(b);
            QMessageBox::information(this,tr("修改成功"),msg);
            this->close();
        }else
        {
            if(x>=maxX)
                QMessageBox::information(this,tr("输入越界"),tr("x的值不能超过当前图片宽度%1").arg(maxX));

            if(y>=maxY)
                QMessageBox::information(this,tr("输入越界"),tr("y的值不能超过当前图片高度%1").arg(maxY));
        }
    }else{
        QMessageBox::warning(this,tr("未填完整"),tr("填写有空缺！"));
    }
}

void SetPixelDialog::clearAll()
{
    ui->lineEditX->clear();
    ui->lineEditY->clear();
    ui->lineEditR->clear();
    ui->lineEditG->clear();
    ui->lineEditB->clear();
}

bool SetPixelDialog::isXEmpty(){

    bool b;
    b = ui->lineEditX->text().isEmpty();
    return b;
}

bool SetPixelDialog::isYEmpty(){

    bool b;
    b = ui->lineEditY->text().isEmpty();
    return b;
}

bool SetPixelDialog::isREmpty(){

    bool b;
    b = ui->lineEditR->text().isEmpty();
    return b;
}

bool SetPixelDialog::isGEmpty(){

    bool b;
    b = ui->lineEditG->text().isEmpty();
    return b;
}

bool SetPixelDialog::isBEmpty(){

    bool b;
    b = ui->lineEditB->text().isEmpty();
    return b;
}

void SetPixelDialog::getRgb(int* r, int* g, int* b)
{
    if(this->isREmpty() || this->isGEmpty() || this->isBEmpty())
    {
        qDebug()<< "RGB BOXES ARE NOT ALL FILLED!";
    }else
    {
        *r = ui->lineEditR->text().toInt();
        *g = ui->lineEditG->text().toInt();
        *b = ui->lineEditB->text().toInt();
    }
}

int SetPixelDialog::getX()
{
    return ui->lineEditX->text().toInt();
}

int SetPixelDialog::getY()
{
    return ui->lineEditY->text().toInt();
}

void SetPixelDialog::setMaxX(int x)
{
    maxX = x;
    return;
}

void SetPixelDialog::setMaxY(int y)
{
    maxY = y;
    return;
}
