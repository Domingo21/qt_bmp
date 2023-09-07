/* 弹窗类
 * 用于设置缩放比例和缩放方法
 */

#ifndef INTERPDIALOG_H
#define INTERPDIALOG_H

#include <QDialog>

namespace Ui {
class InterpDialog;
}

class InterpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InterpDialog(QWidget *parent = nullptr);
    ~InterpDialog();
    double getX();
    double getY();
    bool isXEmpty();
    bool isYEmpty();
    bool isChoosed();
    void clearAll();
    void accept();
signals:

    /* 最邻近插值（Mode = 0）
     * 双线性插值（Mode = 1）*/
    void acceptEvent(double x,double y, int mode);
private:
    Ui::InterpDialog *ui;
};

#endif // INTERPDIALOG_H
