#include "RentalSystem.h"
#include "Timer.h"
#include <algorithm>
#include <tuple>

#include <QMessageBox>
#include <QList>

RentalSystem::RentalSystem(Database& dbInstance, QWidget* parent)
    :   m_dbInstance{ dbInstance }, QWidget(parent)
{
    m_clientGui = new ClientWindow(this);
    m_bikeGui = new BikeWindow(this);
    m_mainGui = new MainWindow(this);
    m_rentalGui = new RentalWindow(this);
    m_warningMessage = new QMessageBox(QMessageBox::Warning, "Warning", "Warning", QMessageBox::Ok, this);
    //m_rentBikeGui = new RentBikeWindow(this);

    m_bikeGui->hide();
    m_clientGui->hide();
    m_rentalGui->hide();
    //m_rentBikeGui->hide();
    m_mainGui->show();
    
    //m_countClient = 0;
    //m_countBike = 0;
    m_currentBikeShow = 0;

    // Load Clients db entries
    int rowsDb = m_dbInstance.dbRowsTable(Database::TableType::Clients);
    for(int i=0; i<rowsDb; ++i)
    {
        Database::ClientDbEntry dbEntry;
        dbEntry = dbInstance.dbLoadClient(i);
        m_vclients.emplace_back(std::get<0>(dbEntry), std::get<3>(dbEntry), std::get<1>(dbEntry), std::get<2>(dbEntry), m_clientGui->getClientList());
    }
    m_countClient = rowsDb != 0 ? m_vclients.back().getId() : 0;

    // Load bikes db entries
    rowsDb = m_dbInstance.dbRowsTable(Database::TableType::Bikes);
    for(int i=0; i<rowsDb; ++i)
    {
        Database::BikeDbEntry dbEntry;
        dbEntry = dbInstance.dbLoadBike(i);
        m_vbikes.emplace_back(  std::get<0>(dbEntry), std::get<3>(dbEntry), std::get<4>(dbEntry), std::get<1>(dbEntry), std::get<2>(dbEntry), 
                                static_cast<Bike::Type>(std::get<5>(dbEntry)), std::get<6>(dbEntry), m_bikeGui->getBikeList());
        m_vbikes.back().newPixmap( QString::fromStdString(std::get<7>(dbEntry)) );
    }
    m_countBike = rowsDb != 0 ? m_vbikes.back().getId() : 0;

    connect(m_bikeGui->getSpecForm(), SIGNAL (sendSpecDescription()), this, SLOT (updateBikeSpec()), Qt::DirectConnection);
    connect(m_rentalGui->getRentBikeWindow(), SIGNAL (sentClientAndBikeId(int,int)), this, SLOT (takeClientId(int,int)), Qt::DirectConnection);
    connect(m_rentalGui, SIGNAL( sendReturnBikeId(int)), this, SLOT( receivedReturnBikeId(int)), Qt::DirectConnection);
    connect(m_bikeGui, SIGNAL( riseWarning(const QString&)), this, SLOT( handleWarning(const QString&)));
}            

void RentalSystem::handleWarning(const QString& msg)
{
    m_warningMessage->setText(msg);
    m_warningMessage->exec();
}

void RentalSystem::takeClientId(int clientId, int bikeId)
{
    auto itBike = std::find_if(m_vbikes.begin(), m_vbikes.end(), [bikeId](const Bike& cur){
        return cur.getId() == bikeId;
    });

    auto itClient = std::find_if(m_vclients.begin(), m_vclients.end(), [clientId](const Client& cur){
            return cur.getId() == clientId;
    });

    if(itBike != m_vbikes.end() && itClient != m_vclients.end())
    {
        itBike->rent(*itClient);
        m_rentalGui->showBike(*itBike);
        m_dbInstance.dbUpdateBikeState(*itBike);
        m_dbInstance.dbUpdateClientState(*itClient);
    }
}

void RentalSystem::receivedReturnBikeId(int bikeId)
{
    auto itBike = std::find_if(m_vbikes.begin(), m_vbikes.end(), [bikeId](const Bike& cur){
        return cur.getId() == bikeId;
    });

    auto itClient = std::find_if(m_vclients.begin(), m_vclients.end(), [bikeId](const Client& cur){
            return cur.getBikeId() == bikeId;
    });

    if(itBike != m_vbikes.end() && itClient != m_vclients.end())
    {
        itBike->deliver(*itClient);
        m_rentalGui->showBike(*itBike);
        m_dbInstance.dbUpdateBikeState(*itBike);
        m_dbInstance.dbUpdateClientState(*itClient);
    }
}

auto RentalSystem::findBike()
{
    QListWidgetItem* item = m_bikeGui->getBikeList()->currentItem();

    auto it = std::find_if(m_vbikes.begin(), m_vbikes.end(), [item](const Bike& cur){
        return cur.getListItem() == item;
    });

    return it;
}

void RentalSystem::createPixMap(const QString& file_name)
{
    auto it = findBike();

    if(it != m_vbikes.end())
    {
        it->newPixmap(file_name);
        m_dbInstance.dbUpdateBikePic(*it, file_name.toStdString());
    }
}

void RentalSystem::removeClient(const QListWidgetItem* item)
{
    auto it = std::find_if(m_vclients.begin(), m_vclients.end(), [item](const Client& client){
        //return client.getId()  == clientId && client.getBikeId() == 0;
        return client.getListItem() == item;
    });

    // TODO: Item with this ID not found or client has bike rented - no erase
    if(it == m_vclients.end())
    {
        handleWarning("Client ID not found");
    }
    else
    {
        if(it->getBikeId() != 0)
        {
            handleWarning("Cannot remove client: Client has a bike with ID: <b>" + QString::number(it->getBikeId()) + "</b> rented");
        }
        else
        {
            // TODO: check if succeed
            m_dbInstance.dbRemoveItem(it->getId(), Database::TableType::Clients);
            m_vclients.erase(it);
        }
    }
}

bool RentalSystem::removeBike()
{
    auto it = findBike();
    // TODO: Item with this ID not found or bike is rented - no erase
    if(it == m_vbikes.end())
    {
        handleWarning("Bike ID not found");
        return false;
    }
    else
    {
        if(!it->isAvailable())
        {
            handleWarning("Cannot remove bike: Bike is rented to Client with ID: <b>" + QString::number(it->getClientId()) + "</b>");
            return false;
        }
        // TODO: check if succeed
        m_dbInstance.dbRemoveItem(it->getId(), Database::TableType::Bikes);
        m_vbikes.erase(it);
        return true;
    }
}

void RentalSystem::newClient(bool)
{
    ++m_countClient;
    m_vclients.emplace_back(m_countClient, 0, m_clientGui->getNamePlainText().toStdString(), m_clientGui->getSurnamePlainText().toStdString(), m_clientGui->getClientList());
    // TODO: check if succeed
    m_dbInstance.dbAddClient(m_vclients.back());
}

void RentalSystem::deleteClient(bool)
{
    QList<QListWidgetItem*> list = m_clientGui->getClientList()->selectedItems();
    for(QListWidgetItem* item : list)
    {
        removeClient(item);
    }
}

void RentalSystem::newBike(bool)
{
    ++m_countBike;
    m_vbikes.emplace_back(m_countBike, 0, true, m_bikeGui->getBrandPlainText().toStdString(), m_bikeGui->getModelPlainText().toStdString(), Bike::Type::max_count, Bike::SpecDesc{}, m_bikeGui->getBikeList());
    // TODO: check if succeed
    m_dbInstance.dbAddBike(m_vbikes.back());
}

void RentalSystem::updateBikeSpec()
{
    auto it = findBike();

    if(it != m_vbikes.end())
    {
        it->setSpec(m_bikeGui->getSpecForm()->getSpecDescription());
        it->setType(m_bikeGui->getSpecForm()->getBikeType());
        m_dbInstance.dbUpdateBikeSpec(*it);
    }
}

void RentalSystem::showMainGui(bool)
{
    m_clientGui->hide(); 
    m_bikeGui->hide();
    m_rentalGui->hide();
    resize(610,310);
    m_mainGui->show();
}

void RentalSystem::showClientGui(bool) 
{ 
    m_mainGui->hide();
    m_bikeGui->hide();
    m_rentalGui->hide();
    resize(610,310);
    m_clientGui->show(); 
}

void RentalSystem::showBikeGui(bool)
{ 
    m_mainGui->hide();
    m_clientGui->hide();
    m_rentalGui->hide();
    resize(610,310);
    m_bikeGui->show();
}

void RentalSystem::showRentalGui(bool)
{
    m_mainGui->hide();
    m_clientGui->hide(); 
    m_bikeGui->hide();
    if(!m_vbikes.empty())
    {
        m_rentalGui->showBike(m_vbikes[0]);
    }
    else
    {
        m_rentalGui->reset();
    }
    resize(1180,620);
    m_rentalGui->show();
}

void RentalSystem::showRentalNextGui(bool)
{
    if(!m_vbikes.empty())
    {
        if(m_currentBikeShow < m_vbikes.size() - 1)
        {
            ++m_currentBikeShow;
        }
        else
        {
            m_currentBikeShow = 0;
        }
        m_rentalGui->showBike(m_vbikes[m_currentBikeShow]);
    }
}

void RentalSystem::showRentalPrevGui(bool)
{
    if(!m_vbikes.empty())
    {
        if(m_currentBikeShow > 0)
        {
            --m_currentBikeShow;
        }
        else
        {
            m_currentBikeShow = m_vbikes.size() - 1;
        }
        m_rentalGui->showBike(m_vbikes[m_currentBikeShow]);
    }
}