#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    s = new Sudoku();

    boardInit();
    connect(ui->newGame, SIGNAL(clicked(bool)), this, SLOT(setBoardToggle()));
    connect(ui->generate, SIGNAL(clicked(bool)), this, SLOT(generateToggle()));
    connect(ui->checking, SIGNAL(clicked(bool)), this, SLOT(checking()));
    connect(ui->messageBlock, SIGNAL(cursorPositionChanged()), this, SLOT(msgScroll()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::boardInit()
{
    printBoard();

    this->isSettingBoard = false;
    ui->board->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->checking->setEnabled(false);
    ui->checking->setFlat(true);
    ui->messageBlock->insertHtml(*(new QString("<p>Initializing completed.<br/></p>")));
}

void MainWindow::printBoard()
{
    ui->board->clearSelection();
    for(int i=0; i<9; ++i)
    {
        for(int j=0; j<9; j++)
        {
            if(s->getElement(i, j) <= 0 || s->getElement(i, j) > 9)
            {
                ui->board->setItem(i, j, new QTableWidgetItem(*(new QString("")),0));
            }
            else
            {
                ui->board->setItem(i, j, new QTableWidgetItem(QString::number(s->getElement(i, j)),0));
            }
            QTableWidgetItem *theCell = ui->board->item(i, j);
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

void MainWindow::setBoard()
{
    s->setMap(*(ui->board));
    s->canBeSolved = false;
    if(s->isFull())
    {
        ui->messageBlock->insertHtml("<p style='color:#ff0000'>The Sudoku problem map is full now.<br/></p>");
        return;
    }
    if(s->isEmpty())
    {
        ui->messageBlock->insertHtml("<p style='color:#ff0000'>The Sudoku problem map is Empty now.<br/></p>");
        return;
    }
    if(!(s->isProperMap()))
    {
        ui->messageBlock->insertHtml("<p style='color:#ff0000'>Number of known cells is too less.<br/></p>");
        return;
    }
    try
    {
        s->solveSudoku(ui->messageBlock, ui->board);
    }
    catch(std::bad_exception e)
    {
        ui->messageBlock->insertHtml("<p style='color:#ff0000'>Unknown Error.<br/></p>");
        return;
    }
    if(s->canBeSolved == false)
    {
        ui->messageBlock->insertHtml("<p style='color:#ff0000'>The Sudoku problem can not be solved.<br/></p>");
    }
}

void MainWindow::setBoardToggle()
{
    if(this->isSettingBoard == false)
    {
        ui->board->setEditTriggers(QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
        ui->newGame->setText("Solve !");
        ui->checking->setEnabled(false);
        ui->checking->setFlat(true);
        ui->generate->setEnabled(false);
        ui->generate->setFlat(true);
        s->clearMap();
        this->printBoard();
        this->isSettingBoard = true;
    }
    else
    {
        ui->board->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->newGame->setText("Set Problem");
        ui->checking->setEnabled(true);
        ui->checking->setFlat(false);
        ui->generate->setEnabled(true);
        ui->generate->setFlat(false);
        this->setBoard();
        this->isSettingBoard = false;
    }
}

void MainWindow::generateToggle()
{
    if(this->isGenerated == false)
    {
        ui->board->setEditTriggers(QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
        ui->generate->setText("Give me answer..");
        ui->newGame->setEnabled(false);
        ui->newGame->setFlat(true);
        ui->checking->setEnabled(true);
        ui->checking->setFlat(false);
        s->generateSudoku(ui->messageBlock, ui->board);
        this->isGenerated = true;
    }
    else
    {
        ui->board->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->generate->setText("Generate");
        ui->newGame->setEnabled(true);
        ui->newGame->setFlat(false);
        ui->checking->setEnabled(false);
        ui->checking->setFlat(true);
        this->setBoard();
        this->isGenerated = false;
    }
}

void MainWindow::checking()
{
    ui->messageBlock->insertHtml("<p>Checking...<br/></p>");
    s->setMap(*(ui->board));
    if(s->isCorrect())
    {
        ui->messageBlock->insertHtml("<p>Congratulations! <span style='color:#008800'>Correct.</span><br/></p>");
    }
    else
    {
        ui->messageBlock->insertHtml("<p>Oops! <span style='color:#008800'>Wrong...</span><br/></p>");
    }
}

void MainWindow::msgScroll()
{
    QTextCursor c = ui->messageBlock->textCursor();
    c.movePosition(QTextCursor::End);
    ui->messageBlock->setTextCursor(c);
}
