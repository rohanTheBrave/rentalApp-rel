#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QPushButton;
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    
signals:
private slots:
private:
    QPushButton *m_rentBikeButton;
    QPushButton *m_bikeButton;
    QPushButton *m_clientButton;
    QPushButton *m_quitButton;
};

#endif // MAINWINDOW_H