#include "RentalWindow.h"
#include "Timer.h"
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QString>

RentalWindow::RentalWindow(QWidget* parent)
    :   QWidget(parent)
{
    //setGeometry(10,10,800,600);

    m_rentBikeWindow = new RentBikeWindow(this);
    m_rentBikeWindow->hide();
    m_showedBikeId = 0;

    m_pictureLabel = new QLabel("Picture <empty>", this);
    m_pictureLabel->setGeometry(10, 10, 800, 600);

    m_typeLabel = new QLabel("Type <empty>", this);
    m_typeLabel->setGeometry(820, 10, 300, 30);

    m_availableLabel = new QLabel("Available <empty>", this);
    m_availableLabel->setGeometry(820, 40, 300, 30);

    m_brandLabel = new QLabel("Brand <empty>", this);
    m_brandLabel->setGeometry(820, 70, 300, 30);

    m_modelLabel = new QLabel("Model <empty>", this);
    m_modelLabel->setGeometry(820, 100, 300, 30);

    m_brakeLabel = new QLabel("Brakes <empty>", this);
    m_brakeLabel->setGeometry(820, 130, 300, 30);

    m_driveLabel = new QLabel("Drivetrain <empty>", this);
    m_driveLabel->setGeometry(820, 160, 300, 30);

    m_wheelLabel = new QLabel("Wheels <empty>", this);
    m_wheelLabel->setGeometry(820, 190, 300, 30);

    m_saddleLabel = new QLabel("Saddle <empty>", this);
    m_saddleLabel->setGeometry(820, 220, 300, 30);

    m_handlerbarLabel = new QLabel("Handlebar setup <empty>", this);
    m_handlerbarLabel->setGeometry(820, 250, 300, 30);

    m_fsuspLabel = new QLabel("Front suspension <empty>", this);
    m_fsuspLabel->setGeometry(820, 280, 300, 30);

    m_rsuspLabel = new QLabel("Rear suspension <empty>", this);
    m_rsuspLabel->setGeometry(820, 310, 300, 30);

    m_backButton = new QPushButton("Back", this);
    m_backButton->setGeometry(1120, 580, 50, 30);

    m_nextBikeButton = new QPushButton(">", this);
    m_nextBikeButton->setGeometry(1070, 580, 40, 30);

    m_prevBikeButton = new QPushButton("<", this);
    m_prevBikeButton->setGeometry(1020, 580, 40, 30);

    m_rentBikeButton = new QPushButton("Rent a bike", this);
    m_rentBikeButton->setGeometry(800, 580, 100, 30);

    m_returnBikeButton = new QPushButton("Return bike", this);
    m_returnBikeButton->setGeometry(910, 580, 100, 30);

    connect(m_backButton, SIGNAL(clicked(bool)), parent, SLOT(showMainGui(bool)));
    connect(m_nextBikeButton, SIGNAL(clicked(bool)), parent, SLOT(showRentalNextGui(bool)));
    connect(m_prevBikeButton, SIGNAL(clicked(bool)), parent, SLOT(showRentalPrevGui(bool)));
    connect(m_rentBikeButton, SIGNAL(clicked(bool)), this, SLOT(showRentBikeGui(bool)) );
    connect(m_returnBikeButton, SIGNAL(clicked(bool)), this, SLOT(returnBike(bool)));
}

RentBikeWindow::RentBikeWindow(QWidget* parent)
    :   QWidget(parent)
{
    setWindowFlags(Qt::Window);
    
    m_clientIdText = new QTextEdit("Enter client ID", this);
    m_clientIdText->setGeometry(10, 10, 130, 30);

    m_applyButton = new QPushButton("Apply", this);
    m_applyButton->setGeometry(10, 60, 130, 30);

    connect(m_applyButton, SIGNAL(clicked(bool)), this, SLOT(receivedClientId(bool)));
}

void RentBikeWindow::receivedClientId(bool)
{
    int value = m_clientIdText->toPlainText().toInt();
    hide();
    Timer t;
    emit sentClientAndBikeId(value,static_cast<RentalWindow*>(parentWidget())->getShowedBikeId());
}

void RentalWindow::returnBike(bool)
{
    emit sendReturnBikeId(m_showedBikeId);
}

void RentalWindow::showRentBikeGui(bool)
{
    m_rentBikeWindow->setGeometry(parentWidget()->x() + parentWidget()->width() / 2,
                                  parentWidget()->y() + parentWidget()->height() / 2,
                                  300,
                                  100);
    m_rentBikeWindow->show();
}

void RentalWindow::showBike(const Bike& bike)
{
    if(bike.getPixMap() != nullptr)
    {
        m_pictureLabel->setPixmap(*(bike.getPixMap()));    
    }
    else
    {
        m_pictureLabel->setText("Picture: <empty>");
    }
    m_showedBikeId = bike.getId();
    m_typeLabel->setText("Type: <b>" + QString(BikeConst::itemName[bike.getType()]) + "</b>");
    m_availableLabel->setText("Available: " + QString(bike.isAvailable() ? "<font color='green'><b>free</b></font>" : "<font color='red'><b>rented</b></font>"));
    m_brandLabel->setText("Brand: <b>" + QString(bike.getBrand().c_str()) + "</b>");
    m_modelLabel->setText("Model: <b>" + QString(bike.getModel().c_str())+ "</b>");
    m_brakeLabel->setText("Brakes: " + QString(bike.getSpec()[static_cast<size_t>(ItemType::brake)].c_str()));
    m_driveLabel->setText("Drivetrain: " + QString(bike.getSpec()[static_cast<size_t>(ItemType::drive)].c_str()));
    m_wheelLabel->setText("Wheels: " + QString(bike.getSpec()[static_cast<size_t>(ItemType::wheel)].c_str()));
    m_saddleLabel->setText("Saddle: " + QString(bike.getSpec()[static_cast<size_t>(ItemType::saddle)].c_str()));
    m_handlerbarLabel->setText("Handlebar setup: " + QString(bike.getSpec()[static_cast<size_t>(ItemType::handlebar)].c_str()));
    m_fsuspLabel->setText("Front suspension: " + QString(bike.getSpec()[static_cast<size_t>(ItemType::front_susp)].c_str()));  
    m_rsuspLabel->setText("Rear suspension: " + QString(bike.getSpec()[static_cast<size_t>(ItemType::rear_susp)].c_str()));
}

void RentalWindow::reset()
{
    m_showedBikeId = 0;
    m_pictureLabel->setText("Picture: <empty>");
    m_typeLabel->setText("Type: <empty>");
    m_availableLabel->setText("Available: <empty>");
    m_brandLabel->setText("Brand: <empty>");
    m_modelLabel->setText("Model: <empty>");
    m_brakeLabel->setText("Brakes: <empty>");
    m_driveLabel->setText("Drivetrain: <empty>");
    m_wheelLabel->setText("Wheels: <empty>");
    m_saddleLabel->setText("Saddle: <empty>");
    m_handlerbarLabel->setText("Handlebar setup: <empty>");
    m_fsuspLabel->setText("Front suspension: <empty>");  
    m_rsuspLabel->setText("Rear suspension: <empty>");
}