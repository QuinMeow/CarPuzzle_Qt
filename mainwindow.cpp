#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Width = 0;
    isLatest = false;
    MinCount = 65535;
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
            char direction = 'n';
            for(row=0;row<Width;row++)
                for(col=0;col<Width;col++)
                    if(Blocks[row][col]==obj)
                    {
                        direction = isAroundSpare(row,col);
                        switch(direction)
                        {
                        case 'n':
                            return false;
                        case 'r':
                            SwepBlocks(row,col,row,col+1);
                            isLatest = false;
                            return true;
                        case 'd':
                            SwepBlocks(row,col,row+1,col);
                            isLatest = false;
                            return true;
                        case 'l':
                            SwepBlocks(row,col,row,col-1);
                            isLatest = false;
                            return true;
                        case 'u':
                            SwepBlocks(row,col,row-1,col);
                            isLatest = false;
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
                    switch(KeyPress->key())
                    {
                    case Qt::Key_Up:
                        if(row!=Width-1)
                        {
                            SwepBlocks(row+1,col,row,col);
                            isLatest = false;
                            return true;
                        }
                    case Qt::Key_Down:
                        if(row!=0)
                        {
                            SwepBlocks(row-1,col,row,col);
                            isLatest = false;
                            return true;
                        }
                    case Qt::Key_Left:
                        if(col!=Width-1)
                        {
                            SwepBlocks(row,col+1,row,col);
                            isLatest = false;
                            return true;
                        }
                    case Qt::Key_Right:
                        if(col!=0)
                        {
                            SwepBlocks(row,col-1,row,col);
                            isLatest = false;
                            return true;
                        }
                    default:
                        return false;

                    }
                }
    }
    else
       return QWidget::eventFilter(obj, event);
}


void MainWindow::on_BtnRandom_clicked()
{
    MinCount = 65535;
    ui->LabMin->setText("0");
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


void MainWindow::on_BtnNextStep_clicked()
{
    if(!isLatest) //路径不是最新
    {
        for(int i=0;i<path.size();i++)
            path.pop();
        string st;
        for(int i=0;i<Width;i++)
            for(int j=0;j<Width;j++)
                st+=Blocks[i][j]->text().toStdString();
        bfs(st);
        isLatest = true;
    }
    char op;
    if(!path.empty()) //当前路径为最新
    {
        op=path.top();
        path.pop();

        for(int row = 0 ; row < Width ;row++)
            for(int col = 0 ; col < Width ; col++)
                if(Blocks[row][col]->text()=="0")
                {
                    switch(op) //进行操作
                    {
                    case 'd':
                        if(row!=Width-1)
                        {
                            SwepBlocks(row+1,col,row,col);
                            return ;
                        }
                    case 'u':
                        if(row!=0)
                        {
                            SwepBlocks(row-1,col,row,col);
                            return ;
                        }
                    case 'r':
                        if(col!=Width-1)
                        {
                            SwepBlocks(row,col+1,row,col);
                            return ;
                        }
                    case 'l':
                        if(col!=0)
                        {
                            SwepBlocks(row,col-1,row,col);
                            return ;
                        }
                    default:
                        return ;

                    }
                }
    }

}

void MainWindow::ClearGrid()
{
    if(Width == 0)
        return;
    ui->LabCount->setText("0");
    for(int i=0;i<Width;i++)
        for(int j=0;j<Width;j++)
            delete Blocks[i][j];
    Blocks.clear();
    for(int i=0;i<path.size();i++)
        path.pop();
    isLatest = false;
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

char MainWindow::isAroundSpare(int row, int col)
{
    int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; //方向参数
    char op[5] = "udlr";


    if(Blocks[row][col]->text()=="0")
        return 'n';

    for (int i = 0; i < 4; i++)
    {
        int dx = row + dir[i][0], dy = col + dir[i][1]; //四个方向探查
        if (dx < 0 || dy < 0 || dx >= Width || dy >= Width) //越界跳过
            continue;
        if(Blocks[dx][dy]->text()=="0")
            return op[i];
    }

    return 'n';
}

void MainWindow::SwepBlocks(int o_row, int o_col, int t_row, int t_col)
{
    QString num = Blocks[t_row][t_col]->text(); //临时变量用于交换
    bool vis =Blocks[t_row][t_col]->isVisible();

    Blocks[t_row][t_col]->setText(Blocks[o_row][o_col]->text());
    Blocks[t_row][t_col]->setVisible(Blocks[o_row][o_col]->isVisible());

    Blocks[o_row][o_col]->setText(num);
    Blocks[o_row][o_col]->setVisible(vis);
    ui->LabCount->setText(QString::number(ui->LabCount->text().toInt()+1)); //计数+1
    isVictory();

}

bool MainWindow::isVictory()
{
    for(int i=0;i<Width;i++)
        for(int j=0;j<Width;j++)
            if(!(i==Width-1 && j==Width-1) && Blocks[i][j]->text()!=QString::number(i*Width+j+1))
                return false;
    if(ui->LabCount->text().toInt()<MinCount)
    {
        MinCount = ui->LabCount->text().toInt();
        ui->LabMin->setText(QString::number(MinCount));
    }
    QMessageBox::information(NULL,"通关","恭喜获得胜利！\n点击\"重置当前\"挑战更短步骤！");
    return true;
}

int MainWindow::f(string s)
{
    int res = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '0')
            continue;
        int t = s[i] - 1;
        res += abs(i / Width - t / Width) + abs(i % Width - t % Width);
    }
    return res;
}

void MainWindow::bfs(string st)
{
    unordered_map<string, int> dist; //无向图，记录不同布局的距离
    unordered_map<string, pair<string, char>> pre;
    typedef pair<int, string> PIS; //键值对
    string ed = "";
    for (int i = 1; i < Width * Width; i++)
        ed += static_cast<char>(i+48); //转字符
    ed += '0';
    int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; //方向参数
    char op[5] = "udlr";                                //方向标识

    priority_queue<PIS, vector<PIS>, greater<PIS>> q; //升序优先队列
    q.push({f(st), st});                              //入队初始距离与初始布局
    dist[st] = 0;                                     //初始布局键值为0
    while (q.size())                                  //当优先队列不为空时
    {
        auto t = q.top(); //取队列顶
        q.pop();
        string state = t.second; //获取布局
        if (state == ed) //当与期望结束布局一致时退出
            break;
        int step = dist[state]; //获取当前布局的距离
        int x, y;
        for (int i = 0; i < state.size(); i++) //获取空格所在位置
            if (state[i] == '0')
            {
                x = i / Width, y = i % Width;
                break;
            }
        string src = state; //复制当前布局
        for (int i = 0; i < 4; i++)
        {
            int dx = x + dir[i][0], dy = y + dir[i][1]; //四个方向探查
            if (dx < 0 || dy < 0 || dx >= Width || dy >= Width) //越界跳过
                continue;
            swap(src[x * Width + y], src[dx * Width + dy]);       //交换空格与探查对象
            if (!dist.count(src) || dist[src] > step + 1) //交换后布局不存在或者存在步数更多的相同布局
            {
                dist[src] = step + 1;                 //添加、更新布局
                pair<string, int> o = {state, op[i]}; //布局与操作字符
                q.push({step + 1 + f(src), src});     //当前步数+曼哈顿距离，当前布局，入队
                pre[src] = o;                         //记录步骤
            }
            swap(src[x * Width + y], src[dx * Width + dy]); //换回原布局，准备探查下一个方向
        }
    }

    while (ed != st)
    {
        auto o = pre[ed]; //依次取出到达结果的路径
        path.push(o.second);
        ed = o.first; //寻找上一步
    }
}


