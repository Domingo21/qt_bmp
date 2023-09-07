/* 主界面
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <setpixeldialog.h>
#include <interpdialog.h>
#include <gaussiandialog.h>
#include "rotatedialog.h"
#include "bmpclass.h"

bool variantLessThan(const QVariant &v1, const QVariant &v2);

// 生成高斯模糊的模板，其中gTemp用于获取模板，模板的宽度不超过11
void generate_gaussian_template(double gTemp[][11], int kSize, double sigma);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //设置整个画布的大小
    void set_canva_size(int width, int height);

    //打开默认的图片，为方便调试
    void open_default_file(QString filename);

    /* 由于QStream是按字节读取的，
     * 读入的字/双字中的字节可能会反，
     * 因此需要用函数进行转换 */
    qint32 LONGtoQint32(LONG n);
    quint32 DWORDtoQuint32(DWORD n);
    quint16 WORDtoQuint16(WORD n);

    BYTE format;
    RGBQUAD *pColorTable;

 public slots:

    //以下函数均为实现基本功能而设置
    void open_bmp_file();
    void save_to_new_bmp_file();
    void display_file_header_new();

    void get_pixel_value_mode();
    void quit_pixel_value_mode();
    void get_pixel_value(QPoint point, QColor color);

    void set_pixel_value();
    void pixel_dialog_accepted(int r,int g,int b,int x,int y);

    void image_interpolation();
    void interp_dialog_accepted(double x,double y,int mode);
    void interpolation(double u, double v,int mode);

    void bmp_rotate();
    void bmp_rotate_accepted(double angle);
    void rotation(double angle);

    void median_filtering();

    void gaussian_smoothing();
    void gaussian_dialog_accepted(double x);


private:

    BITMAPFILEHEADER BitMapFileHeader;
    BITMAPINFOHEADER BitMapInfoHeader;

    //用于保存当前的文件路径
    QString currentFileName;

    //用于判断图片是否已经载入
    bool isFileLoaded;
    bool isGetValue;

    //4个对话框指针
    SetPixelDialog *pixelDialog;
    InterpDialog *interpDialog;
    GaussianDialog * gaussianDialog;
    RotateDialog *rotateDialog;

    QLabel statusLabel;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
