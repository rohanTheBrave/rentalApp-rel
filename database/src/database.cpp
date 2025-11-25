#include "database.h"
#include "Client.h"
#include <string>
#include <fstream>

static const sql::SQLString m_url = "tcp://127.0.0.1";
static const sql::SQLString m_dbName = "rentalApp";
static const sql::SQLString m_clientTableName = "client";
static const sql::SQLString m_bikeTableName = "bike";

Database::Database()
    :   m_driver{ sql::mysql::get_driver_instance() }, 
        m_con{  m_driver->connect(m_url, 
                []() -> sql::SQLString
                {
                    std::ifstream infile("usr/data");
                    std::string line;
                    getline(infile, line);
                    return line;
                }(), 
                []() -> sql::SQLString
                {
                    std::ifstream infile("usr/data");
                    std::string line;
                    getline(infile, line);
                    getline(infile, line);
                    return line;
                }() 
            ) },
        m_stmt{ m_con->createStatement() }
{
}

void Database::dbInit()
{
    bool dbExist = false;

    std::unique_ptr< sql::ResultSet > res{ m_stmt->executeQuery("SHOW databases") };
     
    while (res->next())
    {
        if(res->getString("Database") == m_dbName)
        {
            std::cout << "\t... MySQL replies: " << res->getString("Database") << std::endl;
            m_stmt->execute("USE " + m_dbName);
            dbExist = true;
        }
    }

    if(!dbExist)
    {
        m_stmt->execute("CREATE DATABASE " + m_dbName);
        m_stmt->execute("USE " + m_dbName);
        m_stmt->execute("CREATE TABLE "+ m_bikeTableName + " (id INT, brand VARCHAR(30), model VARCHAR(30), clientId INT, available BOOL, \
                                             type INT, specification JSON, picture VARCHAR(100))");
        m_stmt->execute("CREATE TABLE " + m_clientTableName + " (id INT, name VARCHAR(30), surname VARCHAR(30), bikeId INT)");                                            
        std::cout << "rentalApp database created\n";
    }
}

void Database::dbAddClient(const Client& client)
{
    const sql::SQLString cmd = "INSERT INTO " + m_clientTableName + " VALUES (" +
                                std::to_string(client.getId()) + ",'" + client.getName() +
                                "','" + client.getSurname() + "','" + std::to_string(client.getBikeId()) + "')";
    m_stmt->execute(cmd);
}

void Database::dbUpdateClientState(const Client& client)
{
    const sql::SQLString cmd = "UPDATE " + m_clientTableName + " SET bikeId=" + std::to_string(client.getBikeId()) + 
                                " WHERE id=" + std::to_string(client.getId());

    m_stmt->execute(cmd);
}

Database::ClientDbEntry Database::dbLoadClient(int limit)
{
    const sql::SQLString cmd = "SELECT * FROM " + m_clientTableName + " LIMIT " + std::to_string(limit) + "," + std::to_string(limit+1);
    std::unique_ptr< sql::ResultSet > res{ m_stmt->executeQuery(cmd) };
    res->next();
    
    return std::tuple(res->getInt("id"), res->getString("name"), res->getString("surname"), res->getInt("bikeId"));
}

void Database::dbAddBike(const Bike& bike)
{
    const sql::SQLString cmd = "INSERT INTO " + m_bikeTableName + " VALUES (" + 
                                std::to_string(bike.getId()) + ",'" + bike.getBrand() +
                                "','" + bike.getModel() + "'," + std::to_string(bike.getClientId()) +
                                "," + (bike.isAvailable() ? "true" : "false") + 
                                "," + std::to_string(bike.getType()) + "," + "'[" + 
                                bike.getSpec()[static_cast<size_t>(ItemType::brake)] + 
                                bike.getSpec()[static_cast<size_t>(ItemType::drive)] +
                                bike.getSpec()[static_cast<size_t>(ItemType::wheel)] +
                                bike.getSpec()[static_cast<size_t>(ItemType::saddle)] +
                                bike.getSpec()[static_cast<size_t>(ItemType::handlebar)] +
                                bike.getSpec()[static_cast<size_t>(ItemType::front_susp)] +
                                bike.getSpec()[static_cast<size_t>(ItemType::rear_susp)] + 
                                "]'," + "'empty')";
    m_stmt->execute(cmd);
}

void Database::dbUpdateBikeSpec(const Bike& bike)
{
    const sql::SQLString cmd = "UPDATE " + m_bikeTableName + " SET specification='[\"" + 
                                bike.getSpec()[static_cast<size_t>(ItemType::brake)] + "\",\"" +
                                bike.getSpec()[static_cast<size_t>(ItemType::drive)] + "\",\"" +
                                bike.getSpec()[static_cast<size_t>(ItemType::wheel)] + "\",\"" +
                                bike.getSpec()[static_cast<size_t>(ItemType::saddle)] + "\",\"" +
                                bike.getSpec()[static_cast<size_t>(ItemType::handlebar)] + "\",\"" +
                                bike.getSpec()[static_cast<size_t>(ItemType::front_susp)] + "\",\"" +
                                bike.getSpec()[static_cast<size_t>(ItemType::rear_susp)] +  
                                "\"]' WHERE id=" + std::to_string(bike.getId());
    const sql::SQLString cmd_type = "UPDATE " + m_bikeTableName + " SET type=" + std::to_string(bike.getType()) + 
                                " WHERE id=" + std::to_string(bike.getId());
    m_stmt->execute(cmd_type);
    m_stmt->execute(cmd);
}

Bike::SpecDesc Database::dbParseBikeSpec(const std::string& spec_str)
{
    Bike::SpecDesc spec{};
    bool odd = false;
    int pos = 0;
    int pos_end = 0;
    int countChar = 0;
    int countItems = 0;
    for(char c : spec_str)
    {
        if(c == '\"' && !odd)
        {
            pos = countChar;
            odd = true;
        }
        else if(c == '\"' && odd)
        {
            pos_end = countChar;
            odd = false;
            spec[countItems] = std::move(spec_str.substr(pos + 1, pos_end - pos - 1));
            ++countItems;
            if(countItems > static_cast<int>(ItemType::max_count))
                return spec;
        }
        ++countChar;
    }
    return spec;
}

void Database::dbUpdateBikePic(const Bike& bike, const std::string& path)
{
    const sql::SQLString cmd = "UPDATE " + m_bikeTableName + " SET picture='" + path + "' WHERE id=" + std::to_string(bike.getId());
    m_stmt->execute(cmd);
}

void Database::dbUpdateBikeState(const Bike& bike)
{
    sql::SQLString cmd = "UPDATE " + m_bikeTableName + " SET available=" + std::to_string(bike.isAvailable()) + 
                                " WHERE id=" + std::to_string(bike.getId());
    m_stmt->execute(cmd);

    cmd = "UPDATE " + m_bikeTableName + " SET clientId=" + std::to_string(bike.getClientId()) + 
                                " WHERE id=" + std::to_string(bike.getId());
    m_stmt->execute(cmd);
}

Database::BikeDbEntry Database::dbLoadBike(int limit)
{
    const sql::SQLString cmd = "SELECT * FROM " + m_bikeTableName + " LIMIT " + std::to_string(limit) + "," + std::to_string(limit+1);
    std::unique_ptr< sql::ResultSet > res{ m_stmt->executeQuery(cmd) };
    res->next();
    Bike::SpecDesc spec;
    spec = dbParseBikeSpec(res->getString("specification"));
    
    return std::tuple(res->getInt("id"), res->getString("brand"), res->getString("model"), res->getInt("clientId"), res->getBoolean("available"),
                        res->getInt("type"), spec, res->getString("picture"));
}

void Database::dbRemoveItem(int id, TableType type)
{
    const sql::SQLString cmd = type == Bikes ?  "DELETE FROM " + m_bikeTableName + " WHERE id=" + std::to_string(id) :
                                                "DELETE FROM " + m_clientTableName + " WHERE id=" + std::to_string(id);
    m_stmt->execute(cmd);
}

int Database::dbRowsTable(TableType type)
{ 
    const sql::SQLString cmd = type == Bikes ?  "SELECT COUNT(*) AS Count FROM " + m_bikeTableName : 
                                                "SELECT COUNT(*) AS Count FROM " + m_clientTableName;
    std::unique_ptr< sql::ResultSet > res{ m_stmt->executeQuery(cmd) };
    res->next();

    return res->getInt("Count");
}