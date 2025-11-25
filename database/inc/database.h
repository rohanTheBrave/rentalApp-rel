#ifndef _DATABASE_H
#define _DATABASE_H

#include <mysql/jdbc.h>
#include <string>
#include <memory>
#include <tuple>

#include "Bike.h"

class Client;
class Database
{
public:
    enum TableType
    {
        Bikes,
        Clients,
    };
    using ClientDbEntry = std::tuple<int, std::string, std::string, int>;
    using BikeDbEntry = std::tuple<int, std::string, std::string, int, bool, int, Bike::SpecDesc, std::string>;
    Database();
    void dbInit();
    
    void dbAddClient(const Client&);
    ClientDbEntry dbLoadClient(int limit);
    void dbUpdateClientState(const Client&);

    void dbAddBike(const Bike&);
    void dbUpdateBikeSpec(const Bike&);
    Bike::SpecDesc dbParseBikeSpec(const std::string& spec_str);
    void dbUpdateBikePic(const Bike&, const std::string&);
    BikeDbEntry dbLoadBike(int limit);
    void dbUpdateBikeState(const Bike&);

    void dbRemoveItem(int id, TableType type);
    int dbRowsTable(TableType type);
    
private:
    sql::Driver* m_driver;
    std::unique_ptr< sql::Connection > m_con;
    std::unique_ptr< sql::Statement > m_stmt;
};

#endif