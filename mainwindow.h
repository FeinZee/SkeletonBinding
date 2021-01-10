#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector3D>
#include <QLabel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setCoords(QVector3D coord);
    void CoordinateChanged();
signals:
    void CoordinateHasChanged(float, float, float);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
