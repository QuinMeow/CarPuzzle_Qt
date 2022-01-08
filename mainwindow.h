#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLabel>
#include <QGridLayout>
#include <vector>
#include <algorithm>
#include <random>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject *obj, QEvent *event); //鼠标事件监视
private slots:

    void on_BtnRandom_clicked();

    void on_BtnReset_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QVector<QLabel*>> Blocks; //Vector对象用于存储单元格指针
    std::vector<int> Nums; //存储当前初始顺序
    int Width = 0; //存储华容道规模
    void ClearGrid(); //清除华容道
    int MergeSort(std::vector<int> &nums,int left, int right); //归并求逆序数
    void InitialRandom(); //初始化随机华容道
    int isAroundSpare(int row,int col); //判断周围是否有空格
    void SwepBlocks(int o_row,int o_col,int t_row,int t_col); //交换格子
    bool isVictory(); //判断胜利



};
#endif // MAINWINDOW_H
