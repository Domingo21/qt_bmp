#include "qlabelex.h"
#include <QToolTip>

QLabelEX::QLabelEX(QWidget *parent):QLabel(parent)
{

}

QLabelEX::~QLabelEX()
{

}

void QLabelEX::mouseMoveEvent(QMouseEvent *e)
{
    this->setMouseTracking(true); //自动跟踪鼠标
    QPoint mouse_pos = e->pos();
    QPixmap pixmap = this->grab(QRect(mouse_pos,QSize(1,1)));// 截取1个限像素
    QRgb rgb = pixmap.toImage().pixel(0,0);
 //   QToolTip::showText(this->pos(),tr("(%1, %2)").arg(mouse_pos.x()).arg(mouse_pos.y()));
    emit mouse_moved(mouse_pos,QColor(rgb));
}

void QLabelEX::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit mouse_double_clicked();
    /*
    this->setMouseTracking(true); //自动跟踪鼠标
    QPoint mouse_pos = e->pos();
    QPixmap pixmap = this->grab(QRect(mouse_pos,QSize(1,1)));// 截取1个限像素
    QRgb rgb = pixmap.toImage().pixel(0,0);
  QToolTip::showText(this->pos(),tr("(%1, %2)").arg(mouse_pos.x()).arg(mouse_pos.y()));
    emit mouse_moved(mouse_pos,QColor(rgb));*/
}
