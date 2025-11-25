#include "MainWindow.h"

#include <QPushButton>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    :   QWidget(parent)
{
    // Set size of the window
    //setGeometry(10,10,600,300);
    resize(610,310);
    
    // Create and position the button
    m_rentBikeButton = new QPushButton("Rent a bike", this);
    m_rentBikeButton->setGeometry(10, 10, 130, 30);
    m_bikeButton = new QPushButton("Add/Remove bike", this);
    m_bikeButton->setGeometry(10, 50, 130, 30);
    m_clientButton = new QPushButton("Add/Remove client", this);
    m_clientButton->setGeometry(10, 90, 130, 30);
    m_quitButton = new QPushButton("Quit", this);
    m_quitButton->setGeometry(10, 130, 130, 30);
 
    connect(m_rentBikeButton, SIGNAL( clicked(bool) ), parent, SLOT( showRentalGui(bool)));
    connect(m_clientButton, SIGNAL( clicked(bool) ), parent, SLOT( showClientGui(bool) ));
    connect(m_bikeButton, SIGNAL( clicked(bool) ), parent, SLOT (showBikeGui(bool)));
    connect(m_quitButton, SIGNAL( clicked(bool) ), QApplication::instance(), SLOT (quit()));
}
