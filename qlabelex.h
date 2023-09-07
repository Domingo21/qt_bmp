/* 继承QLabel类
 * 为了实现能在图片上实时获取
 * 鼠标所在处的坐标值和RGB值
 */

#ifndef QLABELEX_H
#define QLABELEX_H
#include <QLabel>
#include <QPoint>
#include <QColor>
#include <QMouseEvent>

class QLabelEX: public QLabel
{
    Q_OBJECT
public:
    explicit QLabelEX(QWidget *parent= 0);
    ~QLabelEX();
protected:
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent *ev);
signals:
    void mouse_moved(QPoint mouseLocalPos, QColor pointColor);
    void mouse_double_clicked();

};

#endif // QLABELEX_H
