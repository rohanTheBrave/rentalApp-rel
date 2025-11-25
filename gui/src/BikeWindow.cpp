#include "BikeWindow.h"

#include <QPushButton>
#include <QTextEdit>
#include <QFrame>
#include <QListWidget>
#include <QLabel>
#include <QFileDialog>
#include <QComboBox>
#include <algorithm>


BikeWindow::BikeWindow(QWidget* parent)
    :   QWidget(parent)
{
    // Set size of the window
    m_specForm = new SpecForm(this);
    m_specForm->setWindowFlags(Qt::Window);
    m_specForm->hide();

    // Create and position the button
    m_addButton = new QPushButton("Add bike", this);
    m_addButton->setGeometry(10, 10, 100, 30);
    
    m_brandText = new QTextEdit("Enter brand here", this);
    m_brandText->setGeometry(10, 50, 150, 30);
    
    m_modelText = new QTextEdit("Enter model here", this);
    m_modelText->setGeometry(10, 90, 150, 30);

    m_specButton = new QPushButton("Update spec", this);
    m_specButton->setGeometry(10, 130, 100, 30);

    m_backButton = new QPushButton("Back", this);
    m_backButton->setGeometry(300, 260, 100, 30);

    m_removeButton = new QPushButton("Remove Bike", this);
    m_removeButton->setGeometry(190, 260, 100, 30);

    m_line = new QFrame(this);
    m_line->setFrameShape(QFrame::VLine);
    m_line->setFrameShadow(QFrame::Sunken);
    m_line->setGeometry(150, 10, 50, 290);

    m_outputList = new QListWidget(this);
    m_outputList->setGeometry(190, 10, 400, 240);
    
    connect(m_backButton, SIGNAL (clicked(bool)), parent, SLOT (showMainGui(bool)));
    connect(m_addButton, SIGNAL (clicked(bool)), parent, SLOT (newBike(bool)));
    connect(m_removeButton, SIGNAL (clicked()), parent, SLOT (removeBike()));
    connect(m_specButton, SIGNAL (clicked(bool)), this, SLOT(switchSpec(bool)));
}

bool BikeWindow::checkIfAnySelected() const
{
    for(int i=0; i < m_outputList->count(); ++i)
    {
        if(m_outputList->item(i)->isSelected())
            return true;
    }
    return false;
}

void BikeWindow::switchSpec(bool)
{
    if(checkIfAnySelected())
    {
        m_specForm->setGeometry(parentWidget()->x() + parentWidget()->width() / 2,
                                parentWidget()->y() + parentWidget()->height() / 2,
                                800,
                                600);
        m_specForm->show();
    }
    else
    {
        emit riseWarning("Cannot update spec: Highlight a proper bike");
    }
}

QString BikeWindow::getBrandPlainText() const
{
    return m_brandText->toPlainText();
}

QString BikeWindow::getModelPlainText() const
{
    return m_modelText->toPlainText();
}

SpecForm::SpecForm(QWidget* parent)
    :   QWidget(parent)
{
    m_bikeTypeComboBox = new QComboBox(this);
    m_bikeTypeComboBox->setPlaceholderText("Chooose bike type");
    for(auto item : BikeConst::itemName)
    {
        m_bikeTypeComboBox->addItem(QString{item});
    }
    m_bikeTypeComboBox->setGeometry(10,10,400,30);
    
    m_brakeLabel = new QLabel("Brakes: ", this);
    m_brakeLabel->setGeometry(10, 40, 100, 30);
    m_brakeText = new QTextEdit(this);
    m_brakeText->setGeometry(10, 70, 400, 30); 

    m_driveLabel = new QLabel("Drivetrain setup: ", this);
    m_driveLabel->setGeometry(10,100,100,30);
    m_driveText = new QTextEdit(this);
    m_driveText->setGeometry(10,130,400,30);

    m_wheelLabel = new QLabel("Wheel setup: " , this);
    m_wheelLabel->setGeometry(10,160,100,30);
    m_wheelText = new QTextEdit(this);
    m_wheelText->setGeometry(10,190,400,30);

    m_saddleLabel = new QLabel("Saddle: ", this);
    m_saddleLabel->setGeometry(10,220,100,30);
    m_saddleText = new QTextEdit(this);
    m_saddleText->setGeometry(10,250,400,30);

    m_handlebarLabel = new QLabel("Handlebar setup: ", this);
    m_handlebarLabel->setGeometry(10,280,100,30);
    m_handlebarText = new QTextEdit(this);
    m_handlebarText->setGeometry(10,310,400,30);

    m_fsuspLabel = new QLabel("Front suspension: ", this);
    m_fsuspLabel->setGeometry(10,340,100,30);
    m_fsuspText = new QTextEdit(this);
    m_fsuspText->setGeometry(10,370,400,30);

    m_rsuspLabel = new QLabel("Rear suspension: ", this);
    m_rsuspLabel->setGeometry(10,400,100,30);
    m_rsuspText = new QTextEdit(this);
    m_rsuspText->setGeometry(10,430,400,30);

    m_uploadPictureButton = new QPushButton("Upload picture", this);
    m_uploadPictureButton->setGeometry(10,490,400,30);
    m_openFile = new QFileDialog(this);
    m_openFile->setFileMode(QFileDialog::ExistingFile);
    m_openFile->setNameFilter(tr("Images (*.png *.xpm *.jpeg)"));

    m_applyButton = new QPushButton("Apply", this);
    m_applyButton->setGeometry(10,560,100,30);

    connect(m_uploadPictureButton, SIGNAL (clicked()), this, SLOT (chooseFile()));
    connect(m_applyButton, SIGNAL (clicked()), this, SLOT (updateSpecDescription()));
    connect(m_openFile, SIGNAL (fileSelected(const QString&)), parent->parent(), SLOT (createPixMap(const QString&)));
}

void SpecForm::chooseFile()
{
    m_openFile->setVisible(true);
}

void SpecForm::updateSpecDescription()
{
    specDescription[static_cast<size_t>(ItemType::brake)]        = m_brakeText->toPlainText().toStdString();
    specDescription[static_cast<size_t>(ItemType::drive)]        = m_driveText->toPlainText().toStdString();
    specDescription[static_cast<size_t>(ItemType::wheel)]        = m_wheelText->toPlainText().toStdString();
    specDescription[static_cast<size_t>(ItemType::saddle)]       = m_saddleText->toPlainText().toStdString();
    specDescription[static_cast<size_t>(ItemType::handlebar)]    = m_handlebarText->toPlainText().toStdString();
    specDescription[static_cast<size_t>(ItemType::front_susp)]   = m_fsuspText->toPlainText().toStdString();
    specDescription[static_cast<size_t>(ItemType::rear_susp)]    = m_rsuspText->toPlainText().toStdString();
    bikeType = BikeConst::convertQStringToType(m_bikeTypeComboBox->currentText());
    emit sendSpecDescription();
    hide();
}