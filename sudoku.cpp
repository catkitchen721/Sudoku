#include "sudoku.h"


Sudoku::Sudoku()
{
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;j++)
        {
            map[i][j] = 0;
        }
    }
}

void Sudoku::setMap(const QTableWidget &board)
{
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;j++)
        {
            if(board.item(i, j)->text().toInt() <= 0 || board.item(i, j)->text().toInt() > 9)
            {
                map[i][j] = 0;
            }
            else
            {
                map[i][j] = board.item(i, j)->text().toInt();
            }
        }
    }
}

void Sudoku::clearMap()
{
    for(int i=0; i<9; ++i)
    {
        for(int j=0; j<9; ++j)
        {
            this->setElement(i, j, 0);
        }
    }
}

int Sudoku::getElement(int row, int column)
{
    return map[row][column];
}

bool Sudoku::isCorrect()
{
    for(int i=0; i<9; ++i)
    {
        for(int j=0; j<9; ++j)
        {
            if(!checkingUnity(i, j))
            {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::checkingUnity(int x, int y)
{
    int countNum[10] = {0,0,0,0,0,0,0,0,0,0};

    for(int i=0; i<9; ++i)
    {
        countNum[this->getElement(i, y)] += 1;
    }

    for(int j=0; j<9; ++j)
    {
        countNum[this->getElement(x, j)] += 1;
    }

    int sqrI = 0;
    int sqrJ = 0;

    if(x >= 0 && x <= 2)
    {
        sqrI = 0;
    }
    else if(x >= 3 && x <= 5)
    {
        sqrI = 3;
    }
    else
    {
        sqrI = 6;
    }

    if(y >= 0 && y <= 2)
    {
        sqrJ = 0;
    }
    else if(y >= 3 && y <= 5)
    {
        sqrJ = 3;
    }
    else
    {
        sqrJ = 6;
    }

    for(int i=sqrI; i<sqrI+3; ++i)
    {
        for(int j=sqrJ; j<sqrJ+3; ++j)
        {
            countNum[this->getElement(i, j)] += 1;
        }
    }

    for(int k=1; k<=9; ++k)
    {
        if(countNum[k] != 3)    return false;
    }
    return true;
}

void Sudoku::solveSudoku(QTextBrowser *msg, QTableWidget *board)
{
    int x = 0, y = 0;

    QVector<bool> posV(10);

    if(isFull())
    {
        msg->insertHtml("<p style='color:#008800'>Solving progress completed.<br/></p>");
        this->canBeSolved = true;
        //
        board->clearSelection();
        for(int i=0; i<9; ++i)
        {
            for(int j=0; j<9; j++)
            {
                if(this->getElement(i, j) <= 0 || this->getElement(i, j) > 9)
                {
                    board->setItem(i, j, new QTableWidgetItem(*(new QString("")),0));
                }
                else
                {
                    board->setItem(i, j, new QTableWidgetItem(QString::number(this->getElement(i, j)),0));
                }
                QTableWidgetItem *theCell = board->item(i, j);
                theCell->setTextAlignment(Qt::AlignCenter);
                if((i >= 0 && i<= 2) || (i >= 6 && i <= 8))
                {
                    if((j >= 0 && j <= 2)||(j >= 6 && j <= 8))
                    {
                        theCell->setBackgroundColor(*(new QColor(255,120,120)));
                    }
                }
                else if(i >= 3 && i <= 5)
                {
                    if(j >= 3 && j <= 5)
                    {
                        theCell->setBackgroundColor(*(new QColor(255,120,120)));
                    }
                }
            }
        }
        //
        return;
    }
    else
    {
        bool isBreak = false;
        for(int i=0; i<9; ++i)
        {
            for(int j=0; j<9; ++j)
            {
                if(this->getElement(i, j) == 0)
                {
                    x = i;
                    y = j;
                    isBreak = true;
                    break;
                }
            }
            if(isBreak == true) break;
        }

        posV = this->getPossibleVector(x, y);

        for(int i=1; i<=9; ++i)
        {
            if(posV.at(i) == true)
            {
                this->setElement(x, y, i);
                this->solveSudoku(msg, board);
            }
        }
        this->setElement(x, y, 0);
    }
}

bool Sudoku::isFull()
{
    for(int i=0; i<9; ++i)
    {
        for(int j=0; j<9; ++j)
        {
            if(this->getElement(i, j) == 0) return false;
        }
    }
    return true;
}

bool Sudoku::isEmpty()
{
    for(int i=0; i<9; ++i)
    {
        for(int j=0; j<9; ++j)
        {
            if(this->getElement(i, j) != 0) return false;
        }
    }
    return true;
}

bool Sudoku::isProperMap()
{
    int numCount = 0;
    for(int i=0; i<9; ++i)
    {
        for(int j=0; j<9; ++j)
        {
            if(this->getElement(i, j) != 0) numCount += 1;
        }
    }
    if(numCount < 17) return false;
    else return true;
}

QVector<bool> Sudoku::getPossibleVector(int x, int y)
{
    QVector<bool> posV(10);
    QVector<bool> emptyV(10);

    for(int i=1; i<=9; ++i)
    {
        posV.replace(i,true);
        emptyV.replace(i,false);
    }

    for(int j=0; j<9; ++j)
    {
        if(this->map[x][j] != 0)
        {
            posV.replace(this->map[x][j], false);
        }
    }

    for(int i=0; i<9; ++i)
    {
        if(this->map[i][y] != 0)
        {
            posV.replace(this->map[i][y], false);
        }
    }

    int sqrI = 0;
    int sqrJ = 0;

    if(x >= 0 && x <= 2)
    {
        sqrI = 0;
    }
    else if(x >= 3 && x <= 5)
    {
        sqrI = 3;
    }
    else
    {
        sqrI = 6;
    }

    if(y >= 0 && y <= 2)
    {
        sqrJ = 0;
    }
    else if(y >= 3 && y <= 5)
    {
        sqrJ = 3;
    }
    else
    {
        sqrJ = 6;
    }

    for(int i=sqrI; i<sqrI+3; ++i)
    {
        for(int j=sqrJ; j<sqrJ+3; ++j)
        {
            if(this->map[i][j] != 0)
            {
                posV.replace(this->map[i][j], false);
            }
        }
    }

    return posV;
}

void Sudoku::setElement(int row, int column, int value)
{
    this->map[row][column] = value;
}

void Sudoku::generateSudoku(QTextBrowser *msg, QTableWidget *board)
{
    srand(time(NULL));
    int seedMap[9][9] =
    {
        3,7,1,9,8,2,4,6,5,
        5,8,6,4,3,7,2,9,1,
        4,9,2,1,6,5,7,3,8,
        7,6,3,8,9,1,5,2,4,
        1,2,4,6,5,3,8,7,9,
        8,5,9,2,7,4,6,1,3,
        9,4,8,7,1,6,3,5,2,
        2,3,7,5,4,9,1,8,6,
        6,1,5,3,2,8,9,4,7
    };

    //change row
    int r1 = 0, r2 = 0;
    while(r1 == r2)
    {
        r1 = rand() % 3;
        r2 = rand() % 3;
    }

    if(r1 == 0)  r1 = 0;
    else if(r1 == 1) r1 = 3;
    else if(r1 == 2) r1 = 6;

    if(r2 == 0)  r2 = 0;
    else if(r2 == 1) r2 = 3;
    else if(r2 == 2) r2 = 6;

    int temp = 0;
    for(int i=0; i<3; ++i)
    {
        for(int j=0; j<9; ++j)
        {
            temp = seedMap[r1+i][j];
            seedMap[r1+i][j] = seedMap[r2+i][j];
            seedMap[r2+i][j] = temp;
        }
    }
    //change column
    int c1 = 0, c2 = 0;
    while(c1 == c2)
    {
        c1 = rand() % 3;
        c2 = rand() % 3;
    }

    if(c1 == 0)  c1 = 0;
    else if(c1 == 1) c1 = 3;
    else if(c1 == 2) c1 = 6;

    if(c2 == 0)  c2 = 0;
    else if(c2 == 1) c2 = 3;
    else if(c2 == 2) c2 = 6;

    temp = 0;
    for(int j=0; j<3; ++j)
    {
        for(int i=0; i<9; ++i)
        {
            temp = seedMap[i][c1+j];
            seedMap[i][c1+j] = seedMap[i][c2+j];
            seedMap[i][c2+j] = temp;
        }
    }
    //change num
    int x = 0;
    int y = 0;

    while(x == y)
    {
        x = rand() % 9 + 1;
        y = rand() % 9 + 1;
    }

    for(int i=0; i<9; ++i)
    {
        for(int j=0; j<9; ++j)
        {
            if(seedMap[i][j] == x)
            {
                seedMap[i][j] = y;
            }
            else if(seedMap[i][j] == y)
            {
                seedMap[i][j] = x;
            }
            else;
        }
    }

    //rotate

    //flip

    //createHole
    int n = rand() % 51 + 20;
    bool isHole[9][9];
    for(int i=0; i<9; ++i)
    {
        for(int j=0; j<9; ++j)
        {
            isHole[i][j] = false;
        }
    }
    for(int i=0; i<n; ++i)
    {
        int indexX = rand() % 9;
        int indexY = rand() % 9;
        if(isHole[indexX][indexY])
        {
            i -= 1;
        }
        else
        {
            seedMap[indexX][indexY] = 0;
        }
    }

    for(int i=0; i<9; ++i)
    {
        for(int j=0; j<9; ++j)
        {
            this->setElement(i, j, seedMap[i][j]);
        }
    }

    board->clearSelection();
    for(int i=0; i<9; ++i)
    {
        for(int j=0; j<9; j++)
        {
            if(this->getElement(i, j) <= 0 || this->getElement(i, j) > 9)
            {
                board->setItem(i, j, new QTableWidgetItem(*(new QString("")),0));
            }
            else
            {
                board->setItem(i, j, new QTableWidgetItem(QString::number(this->getElement(i, j)),0));
            }
            QTableWidgetItem *theCell = board->item(i, j);
            theCell->setTextAlignment(Qt::AlignCenter);
            if((i >= 0 && i<= 2) || (i >= 6 && i <= 8))
            {
                if((j >= 0 && j <= 2)||(j >= 6 && j <= 8))
                {
                    theCell->setBackgroundColor(*(new QColor(255,120,120)));
                }
            }
            else if(i >= 3 && i <= 5)
            {
                if(j >= 3 && j <= 5)
                {
                    theCell->setBackgroundColor(*(new QColor(255,120,120)));
                }
            }
        }
    }
}
