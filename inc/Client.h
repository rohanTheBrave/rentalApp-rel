#ifndef _CLIENT_H
#define _CLIENT_H

#include <string>
#include <QListWidgetItem>
#include <ClientWindow.h>

class Client
{
private:
    int m_id{};
    int m_bikeId{};
    std::string m_name;
    std::string m_surname;
    QListWidgetItem *m_listWidgetItem;

public:
    explicit Client(int id, int bikeId, std::string name, std::string surname, QListWidget* widgetList= nullptr)
        :   m_id{ id }, m_bikeId{ bikeId }, m_name{ std::move(name) }, m_surname{ std::move(surname) }
    {
        m_listWidgetItem = new QListWidgetItem(QString::number(m_id) + ": " + QString::fromStdString(m_name) + ' ' + QString::fromStdString(m_surname), widgetList);
    }
    ~Client()
    {
        delete m_listWidgetItem;
    }

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    Client(Client&& client)
        :   m_id{ client.m_id }, m_bikeId{ client.m_bikeId }, m_name{ std::move(client.m_name) }, m_surname{ std::move(client.m_surname) },
            m_listWidgetItem{ client.m_listWidgetItem }
    {
        client.m_listWidgetItem = nullptr;
    }
    Client& operator=(Client&& client)
    {
        if(this != &client)
        {
            m_id = client.m_id;
            m_bikeId = client.m_bikeId;
            m_name = std::move(client.m_name);
            m_surname = std::move(client.m_surname);
            delete m_listWidgetItem;
            m_listWidgetItem = client.m_listWidgetItem;
            client.m_listWidgetItem = nullptr;
        }
        return *this;
    }

    QListWidgetItem* getListItem() const { return m_listWidgetItem; }
    void setListItem(QListWidgetItem* item) { m_listWidgetItem = item; }
    int getId() const { return m_id; }
    void setBikeId(int id)  { m_bikeId = id; }
    int getBikeId() const { return m_bikeId; }
    const std::string& getName() const { return m_name; }
    const std::string& getSurname() const { return m_surname; }
};

#endif