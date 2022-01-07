#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLabel>
#include <QGridLayout>
#include <vector>
#include <algorithm>
#include <random>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_BtnRandom_clicked();

    void on_BtnReset_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QLabel*> Blocks; //Vector对象用于存储单元格指针
    std::vector<int> Nums; //存储当前初始顺序
    int Width = 0; //存储华容道规模
    void ClearGrid(); //清除华容道
    int MergeSort(std::vector<int> &nums,int left, int right); //归并求逆序数
    void InitialRandom(); //初始化随机华容道


};
#endif // MAINWINDOW_H
