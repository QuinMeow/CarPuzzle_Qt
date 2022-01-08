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

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    int row = 0,col = 0;
    if (event->type() == QEvent::MouseButtonPress) //鼠标点击
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

        if(mouseEvent->button() == Qt::LeftButton)
        {
            int direction = -1;
            for(row=0;row<Width;row++)
                for(col=0;col<Width;col++)
                    if(Blocks[row][col]==obj)
                    {
                        direction = isAroundSpare(row,col);
                        switch(direction)
                        {
                        case -1:
                            return false;
                        case 0:
                            SwepBlocks(row,col,row,col+1);
                            return true;
                        case 1:
                            SwepBlocks(row,col,row+1,col);
                            return true;
                        case 2:
                            SwepBlocks(row,col,row,col-1);
                            return true;
                        case 3:
                            SwepBlocks(row,col,row-1,col);
                            return true;
                        }
                    }
        }
        else
            return false;
    }
    else if(event->type() == QEvent::KeyPress) //键盘事件
    {
        QKeyEvent *KeyPress = static_cast<QKeyEvent*>(event);

        for(row = 0 ; row < Width ;row++)
            for(col = 0 ; col < Width ; col++)
                if(Blocks[row][col]->text()=="0")
                {
                    if(KeyPress->key() == Qt::Key_Up)
                    {
                        if(row!=Width-1)
                        {
                            SwepBlocks(row+1,col,row,col);
                            return true;
                        }
                    }
                    else if(KeyPress->key() == Qt::Key_Down)
                    {
                        if(row!=0)
                        {
                            SwepBlocks(row-1,col,row,col);
                            return true;
                        }
                    }
                    else if(KeyPress->key() == Qt::Key_Left)
                    {
                        if(col!=Width-1)
                        {
                            SwepBlocks(row,col+1,row,col);
                            return true;
                        }
                    }
                    else if(KeyPress->key() == Qt::Key_Right)
                    {
                        if(col!=0)
                        {
                            SwepBlocks(row,col-1,row,col);
                            return true;
                        }
                    }
                    else
                        return false;
                }
    }
    else
       return QWidget::eventFilter(obj, event);
}




void MainWindow::on_BtnRandom_clicked()
{
    ClearGrid();
    QLabel* singleBlock;
    QGridLayout* GameGrid =ui->GameGrid;
    Width = ui->CbWidth->currentIndex()+2;
    InitialRandom(); //随机初始化
    Blocks.resize(Width);
    for(int i=0;i< Width ;i++)
    {
        Blocks[i].resize(Width);
        for(int j=0;j<Width;j++)
        {
            singleBlock = new QLabel(QString::number(Nums[i*Width+j]));
            singleBlock->installEventFilter(this); //安装事件过滤器
            singleBlock->setAlignment(Qt::AlignCenter);
            singleBlock->setFrameShape(QFrame::StyledPanel);
            GameGrid->addWidget(singleBlock,i,j);
            Blocks[i][j]=singleBlock;
        }
    }
    //处理空格
    Blocks[Width-1][Width-1]->setVisible(false);
    Blocks[Width-1][Width-1]->grabKeyboard(); //空格捕获键盘事件

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
            singleBlock->installEventFilter(this);
            singleBlock->setAlignment(Qt::AlignCenter);
            singleBlock->setFrameShape(QFrame::StyledPanel);
            GameGrid->addWidget(singleBlock,i,j);
            Blocks[i][j]=singleBlock;
        }
    }
    //处理空格
   Blocks[Width-1][Width-1]->setVisible(false);
   Blocks[Width-1][Width-1]->grabKeyboard();
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

int MainWindow::isAroundSpare(int row, int col)
{
    if(Blocks[row][col]->text()=="0")
        return -1;
    if(row>0)
        if(Blocks[row-1][col]->text()=="0")
            return 3; //up
    if(row<Width-1)
        if(Blocks[row+1][col]->text()=="0")
            return 1; //down
    if(col>0)
        if(Blocks[row][col-1]->text()=="0")
            return 2; //left
    if(col<Width-1)
        if(Blocks[row][col+1]->text()=="0")
            return 0; //right

    return -1;
}

void MainWindow::SwepBlocks(int o_row, int o_col, int t_row, int t_col)
{
    QString num = Blocks[t_row][t_col]->text();
    bool vis =Blocks[t_row][t_col]->isVisible();

    Blocks[t_row][t_col]->setText(Blocks[o_row][o_col]->text());
    Blocks[t_row][t_col]->setVisible(Blocks[o_row][o_col]->isVisible());

    Blocks[o_row][o_col]->setText(num);
    Blocks[o_row][o_col]->setVisible(vis);

    isVictory();

}

bool MainWindow::isVictory()
{
    for(int i=0;i<Width;i++)
        for(int j=0;j<Width;j++)
            if(!(i==Width-1 && j==Width-1) && Blocks[i][j]->text()!=QString::number(i*Width+j+1))
                return false;
    QMessageBox::information(NULL,"通关","恭喜获得胜利！");
    return true;
}






