#include "ClientWindow.h"

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QList>

#include <string>

ClientWindow::ClientWindow(QWidget *parent)
    :   QWidget(parent)
{
    // Set size of the window
    //setGeometry(10,10,600,300);
    
    // Create and position the button
    m_addButton = new QPushButton("Add client", this);
    m_addButton->setGeometry(10, 10, 100, 30);
    
    m_nameText = new QTextEdit("Enter name here", this);
    m_nameText->setGeometry(10, 50, 150, 30);
    
    m_surnameText = new QTextEdit("Enter surname here", this);
    m_surnameText->setGeometry(10, 90, 150, 30);

    m_backButton = new QPushButton("Back", this);
    m_backButton->setGeometry(300, 260, 100, 30);

    m_removeButton = new QPushButton("Remove client", this);
    m_removeButton->setGeometry(190, 260, 100, 30);

    m_line = new QFrame(this);
    m_line->setFrameShape(QFrame::VLine);
    m_line->setFrameShadow(QFrame::Sunken);
    m_line->setGeometry(150, 10, 50, 290);

    m_outputList = new QListWidget(this);
    m_outputList->setGeometry(190, 10, 400, 240);
    m_outputList->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);

    connect(m_backButton, SIGNAL (clicked(bool)), parent, SLOT (showMainGui(bool)));
    connect(m_addButton, SIGNAL (clicked(bool)), parent, SLOT (newClient(bool)));
    connect(m_removeButton, SIGNAL (clicked(bool)), parent, SLOT (deleteClient(bool)));
}

QString ClientWindow::getNamePlainText() const
{
    return m_nameText->toPlainText();
}

QString ClientWindow::getSurnamePlainText() const
{
    return m_surnameText->toPlainText();
}
