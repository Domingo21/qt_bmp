/* 弹窗类
 * 用于设置某个坐标的像素值
 */
#ifndef SETPIXELDIALOG_H
#define SETPIXELDIALOG_H

#include <QDialog>

namespace Ui {
class SetPixelDialog;
}

class SetPixelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetPixelDialog(QWidget *parent = nullptr);
    ~SetPixelDialog();
    int getX();
    int getY();
    bool isXEmpty();
    bool isYEmpty();
    bool isREmpty();
    bool isGEmpty();
    bool isBEmpty();
    void setMaxX(int x);
    void setMaxY(int y);

    //清除所有填写的内容
    void clearAll();

    //获取填写的RGB值
    void getRgb(int* r, int* g, int* b);

    //在用户点击确认后触发
    void accept();

signals:
    void acceptEvent(int r, int g, int b, int x, int y);

private:
    //判断输入的X和Y是否在图像范围内（x<maxX,y<maxY)
    bool inRange(int x,int y);
    Ui::SetPixelDialog *ui;
    int maxX;
    int maxY;

};

#endif // SETPIXELDIALOG_H
