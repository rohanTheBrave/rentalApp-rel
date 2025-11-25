#ifndef _RENTALSYSTEM_H
#define _RENTALSYSTEM_H

#include "Bike.h"
#include "BikeWindow.h"
#include "Client.h"
#include "ClientWindow.h"
#include "MainWindow.h"
#include "RentalWindow.h"
#include "database.h"
#include <QWidget>
#include <QListWidgetItem>
#include <string>
#include <vector>
#include <iostream>

class QMessageBox;

class RentalSystem : public QWidget
{
    Q_OBJECT
private:
    std::vector<Client> m_vclients;
    std::vector<Bike> m_vbikes;
    ClientWindow* m_clientGui;
    BikeWindow* m_bikeGui;
    MainWindow* m_mainGui;
    RentalWindow* m_rentalGui;
    QMessageBox*    m_warningMessage;
    Database&   m_dbInstance;

    int m_countClient;
    int m_countBike;
    int m_currentBikeShow;

    auto findBike();

public:
    explicit RentalSystem(Database& dbInstance, QWidget* parent = 0);

    void removeClient(const QListWidgetItem* item);

private slots:
    void createPixMap(const QString&);
    void newClient(bool);
    void deleteClient(bool);
    void newBike(bool);
    bool removeBike();
    void updateBikeSpec();
    void showMainGui(bool);
    void showClientGui(bool); 
    void showBikeGui(bool);
    void showRentalGui(bool);
    void showRentalNextGui(bool);
    void showRentalPrevGui(bool);
    void takeClientId(int,int);
    void receivedReturnBikeId(int);
    void handleWarning(const QString& msg);
};

#endif