#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QDebug>
#include <QToolTip>
#include <QPixmap>
#include <QList>
#include <vector>
//#define PAI 3.14159

void MainWindow::open_default_file(QString filename)
{
    QImage *img = new QImage;
    if(!img->load(filename))
    {
        QMessageBox::information(this,
                                 tr("打开图像失败"),
                                 tr("打开默认失败!"));
        delete img;
        return;
    }
    set_canva_size(img->width(),img->height());
    ui->imgLabel->setPixmap(QPixmap::fromImage(*img));

    //将当前的文件名保存起来，以备后续使用
    currentFileName = filename;

    //标记图片已经加载，将那些unable的选项启用
    isFileLoaded = true;
    ui->actionDisplay_file_header->setEnabled(true);

    //获取当前图片的长宽，并赋给pixelDialog中的maxX和maxY，
      int maxX = ui->imgLabel->pixmap()->width();
      int maxY = ui->imgLabel->pixmap()->height();
      pixelDialog->setMaxX(maxX);
      pixelDialog->setMaxY(maxY);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    set_canva_size(this->width(),this->height());

    isFileLoaded = false;
    isGetValue = false;

    ui->actionDisplay_file_header->setEnabled(false);
    pixelDialog = new SetPixelDialog();
    interpDialog = new InterpDialog();
    gaussianDialog= new GaussianDialog();
    rotateDialog= new RotateDialog();

    /* 以下信号槽有两种主要功能：
     * ① 点击菜单中的选项后，触发相应的功能函数
     * ② 从QLabelEX和对话框发出的接收信号中获取参数，并按照相应的参数进行处理
     */
    connect(ui->actionOpen_BMP_file,SIGNAL(triggered()),this,SLOT(open_bmp_file()));
    connect(ui->actionSave_to_new_BMP_file,SIGNAL(triggered()),this,SLOT(save_to_new_bmp_file()));
    connect(ui->actionDisplay_file_header,SIGNAL(triggered()),this,SLOT(display_file_header_new()));

    connect(ui->actionGet_pixel_value,SIGNAL(triggered()),this,SLOT(get_pixel_value_mode()));
    connect(ui->imgLabel,SIGNAL(mouse_moved(QPoint,QColor)),this,SLOT(get_pixel_value(QPoint,QColor)));
    connect(ui->imgLabel,SIGNAL(mouse_double_clicked()),this,SLOT(quit_pixel_value_mode()));

    //设置像素值
    connect(ui->actionSet_pixel_value,SIGNAL(triggered()),this,SLOT(set_pixel_value()));
    connect(pixelDialog,SIGNAL(acceptEvent(int,int,int,int,int)),this,SLOT(pixel_dialog_accepted(int,int,int,int,int)));

    //图像缩放
    connect(ui->actionImage_interpolation,SIGNAL(triggered()),this,SLOT(image_interpolation()));
    connect(interpDialog,SIGNAL(acceptEvent(double,double,int)),this,SLOT(interp_dialog_accepted(double,double,int)));

    //图像旋转
    connect(ui->actionImage_Rotate,SIGNAL(triggered()),this,SLOT(bmp_rotate()));
    connect(rotateDialog,SIGNAL(acceptEvent(double)),this,SLOT(bmp_rotate_accepted(double)));

    //中值滤波
    connect(ui->actionMedian_filtering,SIGNAL(triggered()),this,SLOT(median_filtering()));

    //高斯平滑
    connect(ui->actionGaussian_smoothing,SIGNAL(triggered()),this,SLOT(gaussian_smoothing()));
    connect(gaussianDialog,SIGNAL(acceptEvent(double)),this,SLOT(gaussian_dialog_accepted(double)));

    // 打开默认图像,方便调试处理
    //open_default_file("C:/Users/47613/Desktop/bmpfiles/tiger.bmp");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_canva_size(int width, int height)
{
    ui->centralWidget->setMinimumWidth(width+35);
    ui->centralWidget->setMinimumHeight(height+25);
    ui->centralWidget->setMaximumWidth(width+35);
    ui->centralWidget->setMaximumHeight(height+25);
}

void MainWindow::open_bmp_file()
{
    QString filename;
       filename=QFileDialog::getOpenFileName(this,
                                             tr("选择图像"),
                                             "",
                                             tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
       if(filename.isEmpty())
       {
            return;
       }
       else
       {
           QImage* img=new QImage;

           qDebug() << filename;
           if(! ( img->load(filename) ) ) //加载图像
           {
               QMessageBox::information(this,
                                        tr("打开图像失败"),
                                        tr("打开图像失败!"));
               delete img;
               return;
           }
           set_canva_size(img->width(),img->height());
           ui->imgLabel->setPixmap(QPixmap::fromImage(*img));

           //将当前的文件名保存到类中，方便后续使用
           currentFileName = filename;

           //标记图片已经加载，将那些unable的选项启用
           isFileLoaded = true;
           ui->actionDisplay_file_header->setEnabled(true);

          /* 获取当前图片的长宽，并赋给pixelDialog中的maxX和maxY
           * 作为后续输入的约束
           */
           int maxX = ui->imgLabel->pixmap()->width();
           int maxY = ui->imgLabel->pixmap()->height();
           pixelDialog->setMaxX(maxX);
           pixelDialog->setMaxY(maxY);
           qDebug() << "Current photo width:" << maxX;
           qDebug() << "Current photo height:"<< maxY;
       }
}

void MainWindow::display_file_header_new(){
    QFile file(currentFileName);
    if (!file.open(QIODevice::ReadOnly))
    {
       return;
    }
    else
    {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_6);

        //读取文件头信息
        in >> BitMapFileHeader.bfType;
        in >> BitMapFileHeader.bfSize;
        in >> BitMapFileHeader.bfReserved1;
        in >> BitMapFileHeader.bfReserved2;
        in >> BitMapFileHeader.bfOffBits;


        //读取位图信息头
        in >> BitMapInfoHeader.biSize;
        in >> BitMapInfoHeader.biWidth;
        in >> BitMapInfoHeader.biHeight;
        in >> BitMapInfoHeader.biPlanes;
        in >> BitMapInfoHeader.biBitCount;
        in >> BitMapInfoHeader.biCompression;
        in >> BitMapInfoHeader.biSizeImage;
        in >> BitMapInfoHeader.biXPelsPerMeter;
        in >> BitMapInfoHeader.biYPelsPerMeter;
        in >> BitMapInfoHeader.biClrUsed;
        in >> BitMapInfoHeader.biClrImportant;

        //读取颜色表
        format = WORDtoQuint16(BitMapInfoHeader.biBitCount);
        if (format == 1)
        {
            pColorTable = new RGBQUAD[2];
            //读取颜色表
            for (int i = 0; i < 2; i++)
            {
                in >> pColorTable[i].rgbBlue;
                in >> pColorTable[i].rgbGreen;
                in >> pColorTable[i].rgbRed;
                in >> pColorTable[i].rgbReserved;
            }
        }
        if (format == 8)
        {
            pColorTable = new RGBQUAD[256];
            //读取颜色表
            for (int i = 0; i < 256; i++)
            {
                in >> pColorTable[i].rgbBlue;
                in >> pColorTable[i].rgbGreen;
                in >> pColorTable[i].rgbRed;
                in >> pColorTable[i].rgbReserved;
            }
        }

    }
    file.close();

    QString displayInfo = tr("文件名：%1\r\n\r\n").arg(currentFileName);
    QString tempInfo;

    //整理位图相关信息，并打印
    if(BitMapFileHeader.bfType != 0x424d)
        {
           tempInfo = tr("该文件不是BMP图片\r\n");
           displayInfo += tempInfo;
        }else
        {
            QString hexadecimal;
            qint16 bfType = WORDtoQuint16(BitMapFileHeader.bfType);
            tempInfo = tr("bfType (file type) = %1\r\n").arg(hexadecimal.setNum(bfType,16));
            displayInfo += tempInfo;
            quint32 bfSize = DWORDtoQuint32(BitMapFileHeader.bfSize);
            tempInfo = tr("bfSize (file length) = %1\r\n").arg(bfSize);
            displayInfo += tempInfo;
            quint32 bfOffBits = DWORDtoQuint32(BitMapFileHeader.bfOffBits);
            tempInfo =  tr("bfOffBits (offset of bitmap data in bytes) = %1 \r\n").arg(bfOffBits);
            displayInfo += tempInfo;
            quint32 biSize = DWORDtoQuint32(BitMapInfoHeader.biSize);
            tempInfo =  tr("biSize (header structure length should be 40 or 0x28) = %1 \r\n").arg(biSize);
            displayInfo += tempInfo;
            qint32 biWidth = LONGtoQint32(BitMapInfoHeader.biWidth);
            tempInfo =  tr("biWidth (image width)  = %1 \r\n").arg(biWidth);
            displayInfo += tempInfo;
            qint32 biHeight = LONGtoQint32(BitMapInfoHeader.biHeight);
            tempInfo =  tr("biHeight (image height) = %1 \r\n:").arg(biHeight);
            displayInfo += tempInfo;
            quint16 biPlanes = WORDtoQuint16(BitMapInfoHeader.biPlanes);
            tempInfo =  tr("biPlanes (must be eaual to 1) = %1 \r\n").arg(biPlanes);
            displayInfo += tempInfo;
            quint16 biBitCount = WORDtoQuint16(BitMapInfoHeader.biBitCount);
            tempInfo =  tr("biBitCount (color/pixel bits) = %1 \r\n").arg(biBitCount);
            displayInfo += tempInfo;
            quint32 biCompression = DWORDtoQuint32(BitMapInfoHeader.biCompression);
            tempInfo =  tr("biCompression (compressed?) = %1 \r\n").arg(biCompression);
            displayInfo += tempInfo;
            quint32 biSizeImage = DWORDtoQuint32(BitMapInfoHeader.biSizeImage);
            tempInfo =  tr("biSizeImage (length of bitmap "
                           "data in bytes must be the times of 4) = %1 \r\n").arg(biSizeImage);
            displayInfo += tempInfo;
            qint32 biXPelsPerMeter = LONGtoQint32(BitMapInfoHeader.biXPelsPerMeter);
            tempInfo =  tr("biXPelsPerMeter (horizontal "
                           "resolution of target device in pixels/metre) = %1 \r\n").arg(biXPelsPerMeter);
            displayInfo += tempInfo;
            qint32 biYPelsPerMeter = LONGtoQint32(BitMapInfoHeader.biYPelsPerMeter);
            tempInfo =  tr("biYPelsPerMeter (vertical "
                           "resolution of target device in pixels/metre) = %1 \r\n").arg(biYPelsPerMeter);
            displayInfo += tempInfo;
            quint32 biClrUsed = DWORDtoQuint32(BitMapInfoHeader.biClrUsed);
            tempInfo =  tr("biColorUsed (number of colors used in bitmap,0=2**biBitCount) = %1 \r\n").arg(biClrUsed);
            displayInfo += tempInfo;
            quint32 biClrImportant = DWORDtoQuint32(BitMapInfoHeader.biClrImportant);
            tempInfo =  tr("biColorImportant (number of important colors,"
                           "0=all colors are important) = %1 \r\n").arg(biClrImportant);
            displayInfo += tempInfo;
            tempInfo =  tr("\nThe following is additional information: \r\n");
            displayInfo += tempInfo;
            long nBytesPerRow = 4 * ((biWidth * biBitCount + 31) / 32);
            tempInfo =  tr("Bytes per row in bitmap (nBytesPerRow) = %1 \r\n").arg(nBytesPerRow);
            displayInfo += tempInfo;
            long nImageSizeInByte = biHeight * nBytesPerRow;
            tempInfo =  tr("Total bytes of bitmap (nImageSizeInByte) = %1 \r\n").arg(nImageSizeInByte);
            displayInfo += tempInfo;
            tempInfo =  tr("Actual pixels per row in bitmap (nPixelsPerRow) = %1 \r\n").arg(biWidth);
            displayInfo += tempInfo;
            tempInfo =  tr("Total rows of bitmap (nTotalRows) = %1 \r\n").arg(biHeight);
            displayInfo += tempInfo;
            quint16 biBitCount2 = 1 << WORDtoQuint16(BitMapInfoHeader.biBitCount);
            tempInfo =  tr("Total colors (2**biBitCount)(nTotalColors) = %1 \r\n").arg(biBitCount2);
            displayInfo += tempInfo;
            tempInfo =  tr("Used colors (biColorUsed)(nUsedColors) = %1 ").arg(biClrUsed);
            displayInfo += tempInfo;

    }
    QMessageBox::information(this,tr("BMP信息"),
                         displayInfo);
}

void MainWindow::save_to_new_bmp_file()
{
    QString filename = QFileDialog::getSaveFileName
            (this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)"));
    const QPixmap* aMap = ui->imgLabel->pixmap();
    aMap->save(filename);
}

void MainWindow::get_pixel_value_mode()
{
    ui->statusBar->showMessage(tr("按住鼠标左键,在图片上拖动即可取色；双击退出取色模式。"));
    isGetValue = true;
}

void MainWindow::quit_pixel_value_mode()
{
    ui->statusBar->showMessage(tr("已退出取色模式。"),2000);
    isGetValue = false;
}

void MainWindow::get_pixel_value(QPoint point, QColor color){
    int r,g,b;
    color.getRgb(&r,&g,&b);
    if(isGetValue)
    {
        QString msg = tr("坐标XY: (%1, %2)   颜色RGB: (%3, %4, %5)\n").
                arg(point.x()).arg(point.y()).arg(r).arg(g).arg(b);
        ui->statusBar->showMessage(msg);
    }
}

void MainWindow::set_pixel_value(){
    pixelDialog->clearAll();
    pixelDialog->setWindowTitle(tr("设置像素RGB值"));
    pixelDialog->show();
}

void MainWindow::pixel_dialog_accepted(int r ,int g ,int b ,int x ,int y){
    QString msg = tr("Point(%1,%2) has updated its RGB to (%3,%4,%5)")
            .arg(x).arg(y).arg(r).arg(g).arg(b);
    qDebug() << msg;
    pixelDialog->clearAll();
    QPixmap  pix = QPixmap(*ui->imgLabel->pixmap());
    QImage img  = pix.toImage();
    img.setPixelColor(x,y,QColor(r,g,b));
    ui->imgLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::image_interpolation(){
    interpDialog->clearAll();
    interpDialog->setWindowTitle(tr("设置缩放比例"));
    interpDialog->show();
}

void MainWindow::interp_dialog_accepted(double x, double y,int mode)
{
    QString msg = tr("The Photo has interpolated "
                     "with scale (x,y) = (%1, %2)").arg(x).arg(y);
    qDebug() << msg;
    interpolation(x,y,mode);
}

void MainWindow::interpolation(double u, double v,int mode){
    QPixmap  pix = QPixmap(*ui->imgLabel->pixmap());
    QImage img  = pix.toImage();

    double srcWidth = ui->imgLabel->pixmap()->width();
    double srcHeight = ui->imgLabel->pixmap()->height();
    double desWidth = qRound(srcWidth * u);
    double desHeight = qRound(srcHeight * v);
    QPixmap newPix = QPixmap(desWidth,desHeight);
    QImage newImg = newPix.toImage();

    if(mode == 0)
    {
        //最临近插值
        for(int i =0; i<desWidth-1;i++){
            for(int j=0; j<desHeight-1;j++){
                //i、j点的像素值对应着原图的si和sj
                int si = qRound(i / u);
                int sj = qRound(j / v);
                QColor pixColor = img.pixel(si,sj);
                int red = pixColor.red();
                int green = pixColor.green();
                int blue = pixColor.blue();
                newImg.setPixelColor(i,j,QColor(red,green,blue));
        }
        }
    }else if(mode == 1)
    {   //双线性插值
        //插值需要按坑的数目来
        double newU = (desWidth - 1) / (srcWidth -1);
        double newV = (desHeight -1) / (srcHeight -1);
        for(double i =0; i<desWidth-1;i++){
            for(double j=0; j<desHeight-1;j++){
                //i、j点的像素值按比例除后得到的值
                double si = i / newU;
                double sj = j / newV;
                double siInt = floor(si);
                double sjInt = floor(sj);
                double siDig = si - siInt;
                double sjDig = sj - sjInt;

                int red;
                int green;
                int blue;
                QColor pixColor;

                //下面分别计算f1,f2,f3和f4的值
                // f1 = (1-siDig)*(1-sjDig)*s(siInt,sjInt)
                pixColor = img.pixel(siInt,sjInt);
                red = pixColor.red();
                green = pixColor.green();
                blue = pixColor.blue();

                int f1Red = (1-siDig)*(1-sjDig)*red;
                int f1Green = (1-siDig)*(1-sjDig)*green;
                int f1Blue = (1-siDig)*(1-sjDig)*blue;

                // f2 = siDig*(1-sjDig)*s(siInt+1,sjInt)
                pixColor = img.pixel(siInt+1,sjInt);
                red = pixColor.red();
                green = pixColor.green();
                blue = pixColor.blue();

                int f2Red = siDig*(1-sjDig)*red;
                int f2Green = siDig*(1-sjDig)*green;
                int f2Blue = siDig*(1-sjDig)*blue;

                // f3 = (1-siDig)*sjDig*s(siInt,sjInt+1)
                pixColor = img.pixel(siInt,sjInt+1);
                red = pixColor.red();
                green = pixColor.green();
                blue = pixColor.blue();

                int f3Red = (1-siDig)*sjDig*red;
                int f3Green = (1-siDig)*sjDig*green;
                int f3Blue = (1-siDig)*sjDig*blue;

                // f4 = siDig*sjDig*s(siInt+1,sjInt+1)
                pixColor = img.pixel(siInt+1,sjInt+1);
                red = pixColor.red();
                green = pixColor.green();
                blue = pixColor.blue();

                int f4Red = siDig*sjDig*red;
                int f4Green = siDig*sjDig*green;
                int f4Blue =  siDig*sjDig*blue;

                int fRed = f1Red + f2Red + f3Red + f4Red;
                int fGreen = f1Green + f2Green + f3Green + f4Green;
                int fBlue = f1Blue + f2Blue + f3Blue + f4Blue;
                newImg.setPixelColor(i,j,QColor(fRed,fGreen,fBlue));
        }
        }
    }
    set_canva_size(newImg.width(),newImg.height()); //重新布局界面大小
    ui->imgLabel->setPixmap(QPixmap::fromImage(newImg));
    QString msg = tr("图像已经按要求进行了缩放,x缩放了%1倍，y缩放了%2倍").arg(u).arg(v);
    QMessageBox::information(this,tr("调整大小"),msg);
}

void MainWindow::bmp_rotate(){
    //QString msg = tr("第一步");
   // QMessageBox::information(this,tr("调整大小"),msg);
    rotateDialog->clearAll();
   // QString msg_2 = tr("第二步");
   // QMessageBox::information(this,tr("调整大小"),msg_2);
    rotateDialog->setWindowTitle(tr("设置旋转角度"));
   // QString msg_3 = tr("第三步");
   // QMessageBox::information(this,tr("调整大小"),msg_3);
    rotateDialog->show();
    //QString msg_4 = tr("第四步");
    //QMessageBox::information(this,tr("调整大小"),msg_4);
}

void MainWindow::bmp_rotate_accepted(double angle)
{
    QString msg = tr("The Photo has rotated "
                     "with scale angle = %1)").arg(angle);
    qDebug() << msg;
   // QString msg_5 = tr("第五步");
   // QMessageBox::information(this,tr("调整大小"),msg_5);
    rotation(angle);
}

void MainWindow::rotation(double angle)
{
    QString msg_6 = tr("进入函数");
    QMessageBox::information(this,tr("调整大小"),msg_6);
    QPixmap  pix = QPixmap(*ui->imgLabel->pixmap());
    QImage img  = pix.toImage();

    double srcWidth = ui->imgLabel->pixmap()->width();
    double srcHeight = ui->imgLabel->pixmap()->height();
    double desWidth = srcWidth * 2.5;
    double desHeight = srcHeight * 2.5;
    QPixmap newPix = QPixmap(desWidth,desHeight);
    QImage newImg = newPix.toImage();


    double midX_pre, midY_pre, midX_aft, midY_aft;//旋转所围绕的中心点的坐标
    midX_pre = srcWidth / 2;
    midY_pre = srcHeight / 2;
    midX_aft = srcWidth;
    midY_aft = srcHeight;
    double pre_i, pre_j, after_i, after_j;//旋转前后对应的像素点坐标
    //cout << "输入要旋转的角度：" << endl;
    //cin >> RotateAngle;
    angle = 1.0 * angle * 3.14159 / 180;
    for (int i = 0; i < 2 * srcHeight; ++i)
    {
        for (int j = 0; j < 2 * srcWidth; ++j)
        {
            after_i = i - midY_aft;//坐标变换
            after_j = j - midX_aft;
            pre_i = (int)(cos((double)angle) * after_i - sin((double)angle) * after_j) + midY_pre;
            pre_j = (int)(sin((double)angle) * after_i + cos((double)angle) * after_j) + midX_pre;
            if (pre_i >= 0 && pre_i < srcHeight && pre_j >= 0 && pre_j < srcWidth)
            {   //在原图范围内
                QColor pixColor = img.pixel(pre_i,pre_j);
                int red = pixColor.red();
                int green = pixColor.green();
                int blue = pixColor.blue();
                newImg.setPixelColor(i,j,QColor(red,green,blue));
            }
        }
    }
    set_canva_size(newImg.width(),newImg.height()); //重新布局界面大小
    ui->imgLabel->setPixmap(QPixmap::fromImage(newImg));
    QString msg = tr("The Photo has rotated %1").arg(angle);
    QMessageBox::information(this,tr("旋转角度"),msg);
}

void MainWindow::median_filtering(){
    //原图的图片信息
    QPixmap  pix = QPixmap(*ui->imgLabel->pixmap());
    QImage img  = pix.toImage();
    double imgWidth = ui->imgLabel->pixmap()->width();
    double imgHeight = ui->imgLabel->pixmap()->height();
    QPixmap newPix = QPixmap(imgWidth,imgHeight);
    QImage newImg = newPix.toImage();

    std::vector<int> tempRed;
    std::vector<int> tempGreen;
    std::vector<int> tempBlue;

    for(int i=0; i<imgWidth;i++)
        for(int j=0;j<imgHeight;j++)
        {
            if(i!=0 && j!= 0 && (i != imgWidth-1) && (j!=imgHeight-1))
            {
                //处理红色通道
                tempRed.push_back(img.pixelColor(i-1,j-1).red());
                tempRed.push_back(img.pixelColor(i-1,j).red());
                tempRed.push_back(img.pixelColor(i-1,j+1).red());
                tempRed.push_back(img.pixelColor(i,j-1).red());
                tempRed.push_back(img.pixelColor(i,j).red());
                tempRed.push_back(img.pixelColor(i,j+1).red());
                tempRed.push_back(img.pixelColor(i+1,j-1).red());
                tempRed.push_back(img.pixelColor(i+1,j).red());
                tempRed.push_back(img.pixelColor(i+1,j+1).red());
                std::sort(tempRed.begin(),tempRed.end());
                int newRed = tempRed[4];

                //处理绿色通道
                tempGreen.push_back(img.pixelColor(i-1,j-1).green());
                tempGreen.push_back(img.pixelColor(i-1,j).green());
                tempGreen.push_back(img.pixelColor(i-1,j+1).green());
                tempGreen.push_back(img.pixelColor(i,j-1).green());
                tempGreen.push_back(img.pixelColor(i,j).green());
                tempGreen.push_back(img.pixelColor(i,j+1).green());
                tempGreen.push_back(img.pixelColor(i+1,j-1).green());
                tempGreen.push_back(img.pixelColor(i+1,j).green());
                tempGreen.push_back(img.pixelColor(i+1,j+1).green());
                std::sort(tempGreen.begin(),tempGreen.end());
                int newGreen = tempGreen[4];

                //处理蓝色通道
                tempBlue.push_back(img.pixelColor(i-1,j-1).blue());
                tempBlue.push_back(img.pixelColor(i-1,j).blue());
                tempBlue.push_back(img.pixelColor(i-1,j+1).blue());
                tempBlue.push_back(img.pixelColor(i,j-1).blue());
                tempBlue.push_back(img.pixelColor(i,j).blue());
                tempBlue.push_back(img.pixelColor(i,j+1).blue());
                tempBlue.push_back(img.pixelColor(i+1,j-1).blue());
                tempBlue.push_back(img.pixelColor(i+1,j).blue());
                tempBlue.push_back(img.pixelColor(i+1,j+1).blue());
                std::sort(tempBlue.begin(),tempBlue.end());
               int newBlue = tempBlue[4];

                newImg.setPixelColor(i,j,QColor(newRed,newGreen,newBlue));
                tempRed.clear();
                tempGreen.clear();
                tempBlue.clear();
            }else
            {
                //处理边缘
                newImg.setPixelColor(i,j,img.pixelColor(i,j));
            }
        }

    //重新布局界面大小，并显示图像
    set_canva_size(newImg.width(),newImg.height());
    ui->imgLabel->setPixmap(QPixmap::fromImage(newImg));

    QString msg = tr("已完成中值滤波的去噪处理");
    QMessageBox::information(this,tr("调整大小"),msg);
}

void MainWindow::gaussian_smoothing()
{
    gaussianDialog->clearAll();
    gaussianDialog->setWindowTitle(tr("设置sigma值"));
    gaussianDialog->show();
}


void MainWindow::gaussian_dialog_accepted(double sigma)
{
    qDebug() << "gaussian_smoothing is triggered";

    //原图的图片信息
    QPixmap  pix = QPixmap(*ui->imgLabel->pixmap());
    QImage img  = pix.toImage();
    double imgWidth = ui->imgLabel->pixmap()->width();
    double imgHeight = ui->imgLabel->pixmap()->height();
    QPixmap newPix = QPixmap(imgWidth,imgHeight);
    QImage newImg = newPix.toImage();

    std::vector<int> tempRed;
    std::vector<int> tempGreen;
    std::vector<int> tempBlue;

    int size = 3;
    double dGaussianTemplate[3][11];
    generate_gaussian_template(dGaussianTemplate,size,sigma);
    int *gaussianTemplate = new int[size*size];
    int templateSum = 0;
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            gaussianTemplate[i+j*size] = floor(dGaussianTemplate[i][j]);
            templateSum += floor(dGaussianTemplate[i][j]);
        }
    }

    int newRed = 0;
    int newGreen = 0;
    int newBlue = 0;
    int i,j,m,n,k;
    for(i=0; i<imgWidth;i++)
        for(j=0;j<imgHeight;j++)
        {
            if(i!=0 && j!= 0 && (i != imgWidth-1) && (j!=imgHeight-1))
            {
                // 处理红色通道
                for(m=i-1;m<i+2;m++)
                     for(n=j-1;n<j+2;n++)
                        tempRed.push_back(img.pixelColor(m,n).red());

                newRed = 0;
                for(k =0;k<9;k++)
                {
                    newRed += tempRed[k] * gaussianTemplate[k];
                }
                newRed =  newRed / templateSum;

                // 处理绿色通道
                for(m=i-1;m<i+2;m++)
                     for(n=j-1;n<j+2;n++)
                        tempGreen.push_back(img.pixelColor(m,n).green());

                newGreen = 0;
                for(int k=0;k<9;k++)
                {
                    newGreen += tempGreen[k] * gaussianTemplate[k];
                }
                newGreen =  newGreen / templateSum;

                // 处理蓝色通道
                for(m=i-1;m<i+2;m++)
                     for(n=j-1;n<j+2;n++)
                        tempBlue.push_back(img.pixelColor(m,n).blue());

                newBlue = 0;
                for(k =0;k<9;k++)
                {
                    newBlue += tempBlue[k] * gaussianTemplate[k];
                }
                newBlue =  newBlue/ templateSum;

                newImg.setPixelColor(i,j,QColor(newRed,newGreen,newBlue));
                tempRed.clear();
                tempGreen.clear();
                tempBlue.clear();
            }else
            {
               //处理边缘
                newImg.setPixelColor(i,j,img.pixelColor(i,j));
            }
        }

    //重新布局界面大小，并显示处理后的图像
    set_canva_size(newImg.width(),newImg.height());
    ui->imgLabel->setPixmap(QPixmap::fromImage(newImg));

    QString msg = tr("已完成高斯平滑处理");
    QMessageBox::information(this,tr("完成"),msg);
}


//输入一个数组作为模板的容器；确定模板的大小和sigma值，将结果返回给模板。
void generate_gaussian_template(double gTemp[][11], int kSize, double sigma)
{
    //static const double PI = 3.1415926535;
    //模板的中心位置
    int center = kSize / 2;
    double x2, y2;
    for(int i = 0; i<kSize; i++)
    {
        x2 = pow(i-center,2);
        for(int j =0; j<kSize;j++)
        {
            y2 = pow(j-center,2);
            double g = exp(-(x2+y2) / (2*sigma*sigma));
            g /= 2* 3.1415926535 * sigma;
            gTemp[i][j] = g;
        }
    }

    //将右上角系数进行归一化处理
    double t = 1 /gTemp[0][0];
    for(int i =0;i<kSize;i++)
        for(int j=0; j<kSize;j++)
            gTemp[i][j] *=t;
}


bool variantLessThan(const QVariant &v1, const QVariant &v2)
{
     return v1.toString() < v2.toString();
}


quint32 MainWindow::DWORDtoQuint32(DWORD n)
{
    quint32 r = 0x00000000, temp = 0x00000000;
    temp = n >> 24;                  //取第一个字节
    r = r + temp;
    temp = (n & (0x00ff0000)) >> 8;  //取第二个字节
    r = r + temp;
    temp = (n & (0x0000ff00)) << 8;  //取第三个字节
    r = r + temp;
    temp = (n & (0x000000ff)) << 24; //取第四个字节
    r = r + temp;
    return r;
}

quint16 MainWindow::WORDtoQuint16(WORD n)
{
    quint16 r = 0x0000, temp = 0x0000;
    temp = n >> 8;
    r = r + temp;
    temp = (n & 0x00ff) << 8;        //取第二个字节
    r = r +temp;
    return r;

}

qint32 MainWindow::LONGtoQint32(LONG n)
{
    qint32 r = 0x00000000, temp = 0x00000000;
    temp = n >> 24;                  //取第一个字节
    temp = temp & 0x000000ff;        // 逻辑右移
    r = r + temp;
    temp = (n & (0x00ff0000)) >> 8;  //取第二个字节
    temp = temp & 0x00ffffff;
    r = r + temp;
    temp = (n & (0x0000ff00)) << 8;  //取第三个字节
    r = r + temp;
    temp = (n & (0x000000ff)) << 24; //取第四个字节
    r = r + temp;
    return r;
}
