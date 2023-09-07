/* 弹窗类
 * 用于设置高斯模糊的sigma值
 */

#ifndef GAUSSIANDIALOG_H
#define GAUSSIANDIALOG_H

#include <QDialog>

namespace Ui {
class GaussianDialog;
}

class GaussianDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GaussianDialog(QWidget *parent = 0);
    ~GaussianDialog();
    double getX();
    bool isXEmpty();
    void clearAll();
    void accept();

private:
    Ui::GaussianDialog *ui;
signals:
    void acceptEvent(double x);
};

#endif // GAUSSIANDIALOG_H
