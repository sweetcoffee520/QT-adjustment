#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "triangleBase.h"
#include <QTableWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    triangleBase base;
    QWidget *centralWidget=new QWidget(this);
    QHBoxLayout *horizontalLayout=new QHBoxLayout(centralWidget);
    QTableWidget *tableWidget=new QTableWidget();
    QLabel *label=new QLabel();
    bool drawflag=0;
    QDialog dialog;
    void paint();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_actionopen_triggered();

    void on_actionstart_adjust_triggered();

    void on_actionorigin_data_triggered();

    void on_actionadjusted_data_triggered();

    void on_actioncoor_data_triggered();

    void on_actionclose_triggered();

    void on_actioncoor_data_2_triggered();

    void on_actionobserve_data_triggered();

    void on_actionshow_graphic_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
