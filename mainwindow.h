#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QVector>
#include <QString>
#include <QBrush>
#include <QAbstractItemView>

#include "sudoku.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void boardInit();
    void printBoard();
    void setBoard();

    ~MainWindow();

private slots:
    void setBoardToggle();
    void generateToggle();
    void checking();
    void msgScroll();

private:
    Ui::MainWindow *ui;
    Sudoku *s;

    bool isSettingBoard;
    bool isGenerated;
};

#endif // MAINWINDOW_H
