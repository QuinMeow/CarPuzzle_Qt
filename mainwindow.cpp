#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_BtnRandom_clicked()
{
    ClearGrid();
    QLabel* singleBlock;
    QGridLayout* GameGrid =ui->GameGrid;
    Width = ui->CbWidth->currentIndex()+3;
    InitialRandom(); //随机初始化
    Blocks.resize(Width);
    for(int i=0;i< Width ;i++)
    {
        Blocks[i].resize(Width);
        for(int j=0;j<Width;j++)
        {
            singleBlock = new QLabel(QString::number(Nums[i*Width+j]));
            singleBlock->setAlignment(Qt::AlignCenter);
            singleBlock->setFrameShape(QFrame::StyledPanel);
            GameGrid->addWidget(singleBlock,i,j);
            Blocks[i][j]=singleBlock;
        }
    }
    //处理空格
    Blocks[Width-1][Width-1]->setText("");
    Blocks[Width-1][Width-1]->setVisible(false);

}

void MainWindow::on_BtnReset_clicked()
{
    ClearGrid();
    QLabel* singleBlock;
    QGridLayout* GameGrid =ui->GameGrid;
    Blocks.resize(Width);
    for(int i=0;i< Width ;i++)
    {
        Blocks[i].resize(Width);
        for(int j=0;j<Width;j++)
        {
            singleBlock = new QLabel(QString::number(Nums[i*Width+j]));
            singleBlock->setAlignment(Qt::AlignCenter);
            singleBlock->setFrameShape(QFrame::StyledPanel);
            GameGrid->addWidget(singleBlock,i,j);
            Blocks[i][j]=singleBlock;
        }
    }
    //处理空格
   Blocks[Width-1][Width-1]->setText("");
   Blocks[Width-1][Width-1]->setVisible(false);
}

void MainWindow::ClearGrid()
{
    if(Width == 0)
        return;
    for(int i=0;i<Width;i++)
        for(int j=0;j<Width;j++)
            delete Blocks[i][j];
    Blocks.clear();
}

int MainWindow::MergeSort(std::vector<int> &nums, int left, int right)
{
        if (left == right)
            return 0;
        else
        {
            int mid = (left + right) / 2;
            //对left到mid，mid到right进行归并排序
            int C1 = MergeSort(nums, left, mid);
            int C2 = MergeSort(nums, mid + 1, right);
            int counts = C1 + C2;

            int j = mid + 1; //右队列指针
            for(int i = left ; i <= mid ; i++) //左队列指针
            {
                while (j <= right && nums[i] > nums[j])
                    j++;
                counts += (j - mid - 1);
            }

           std::sort(nums.begin()+left, nums.begin()+1+right);

            return counts;
        }
}
void MainWindow::InitialRandom()
{

    int length = Width * Width - 1; //比总数少1
    int ReverseNumbers;
    bool Vaild = false;

    Nums.clear();
    std::vector<int> temp;
    for (int i = 0; i < length; ++i)
    {
        Nums.push_back(i + 1);
    }
    do
    {
        unsigned seed = std::chrono::system_clock::now ().time_since_epoch ().count (); //seed
        std::shuffle(Nums.begin(),Nums.end(),std::default_random_engine(seed));
        temp.clear();
        temp.assign(Nums.begin(),Nums.end());//复制一份
        ReverseNumbers = MergeSort(temp,0,length-1);
        if(ReverseNumbers % 2 == 0 ) //判断是否有解
            Vaild = true;
    }while(!Vaild);
    Nums.push_back(0); //空格位添加0
}






