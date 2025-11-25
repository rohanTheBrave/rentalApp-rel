#ifndef _BIKE_H
#define _BIKE_H

#include <string>
#include <array>
#include <QPixmap>
#include <QString>
#include <QStringView>
#include <QListWidgetItem>
#include "Client.h"

enum class ItemType
{
    brake,
    drive,
    wheel,
    saddle,
    handlebar,
    front_susp,
    rear_susp,
    max_count,
};

class Bike
{
public:
    using SpecDesc = std::array<std::string, static_cast<size_t>(ItemType::max_count)>;
    enum Type
    {
        enduro,
        all_mountain,
        downhill,
        road,
        gravel,
        trekking,
        max_count,
    };
    
private:
    int m_id{};
    int m_clientId{};
    bool m_available{true};
    std::string m_brand;
    std::string m_model;
    Type m_type;
    SpecDesc m_spec;
    QPixmap *m_pixmap;
    QListWidgetItem *m_listWidgetItem;

public:
    explicit Bike(int id, int clientId, bool av, std::string brand, std::string model, Type type, SpecDesc spec, QListWidget* widgetList= nullptr)
        :   m_id{ id }, m_clientId{ clientId }, m_available{ av }, m_brand{ std::move(brand) }, m_model{ std::move(model) },
             m_type{ type }, m_spec{ std::move(spec) }
    {
        m_listWidgetItem = new QListWidgetItem(QString::number(id) + ": " + QString::fromStdString(m_brand) + ' ' + QString::fromStdString(m_model), widgetList);
        m_pixmap = new QPixmap();
    }
    
    ~Bike()
    {
        delete m_pixmap; 
        delete m_listWidgetItem;
    }

    Bike(const Bike& bike) = delete;
    Bike(Bike&& bike)
        :   m_id{bike.m_id}, m_clientId{ bike.m_clientId }, m_available{ bike.m_available }, m_brand{ std::move(bike.m_brand) }, m_model{ std::move(bike.m_model) },
            m_type{ bike.m_type }, m_spec{ std::move(bike.m_spec) }, m_pixmap{ bike.m_pixmap }, m_listWidgetItem{ bike.m_listWidgetItem }
    {
        bike.m_pixmap = nullptr;
        bike.m_listWidgetItem = nullptr;        
    }
    
    Bike& operator=(const Bike& bike) = delete;
    Bike& operator=(Bike&& bike)
    {
        if(this != &bike)
        {
            m_id = bike.m_id;
            m_clientId = bike.m_clientId;
            m_available = bike.m_available;
            m_brand = std::move(bike.m_brand);
            m_model = std::move(bike.m_model);
            m_type = bike.m_type;
            m_spec = std::move(bike.m_spec);
            
            delete m_pixmap;
            m_pixmap = bike.m_pixmap;
            bike.m_pixmap = nullptr;

            delete m_listWidgetItem;
            m_listWidgetItem = bike.m_listWidgetItem;            
            bike.m_listWidgetItem = nullptr;
        }
        
        return *this;
    }

    int getClientId() const
    {
        return m_clientId;
    }
    void setSpec(const SpecDesc& spec)
    {
        m_spec = spec;
    }
    void setType(Type type)
    {
        m_type = type;
    }
    void newPixmap(const QString& name)
    {
        if(m_pixmap)
        {
            m_pixmap->load(name);
        }
    }
    const std::string& infoOf(ItemType type) const
    {
        return m_spec[static_cast<size_t>(type)];
    }
    bool isAvailable() const
    {
        return m_available;
    }
    
    int getId() const                           { return m_id; }
    QPixmap* getPixMap() const                  { return m_pixmap; }
    const std::string& getBrand() const         { return m_brand; }
    const std::string& getModel() const         { return m_model; }
    Type getType() const                        { return m_type; }
    const SpecDesc& getSpec() const             { return m_spec; }
    const QListWidgetItem* getListItem() const  { return m_listWidgetItem; }


    void rent(Client& client);
    void deliver(Client& client);

};

namespace BikeConst
{
    //using namespace Qt::Literals::StringLiterals;
    constexpr std::array<const char*, static_cast<size_t>(Bike::Type::max_count)> itemName
    { 
        "Enduro", "All Mountain", "Downhill", "Road", "Gravel", "Trekking"
    };

    inline Bike::Type convertQStringToType(const QString& qstr)
    {
        if(qstr == "Enduro")            return Bike::Type::enduro;
        if(qstr == "All Mountain")      return Bike::Type::all_mountain;
        if(qstr == "Downhill")          return Bike::Type::downhill;
        if(qstr == "Road")              return Bike::Type::road;
        if(qstr == "Gravel")            return Bike::Type::gravel;
        if(qstr == "Trekking")          return Bike::Type::trekking;
        else                            return Bike::Type::max_count;
    }
}

#endif