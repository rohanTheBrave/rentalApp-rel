#include "Bike.h"
#include <iostream>

void Bike::rent(Client& client)
{
    // TODO: Bike can be rented if available and one client can have one bike
    if(m_available && client.getBikeId() == 0)
    {
        m_clientId = client.getId();
        m_available = false;
        client.setBikeId(m_id);
    }
}

void Bike::deliver(Client& client)
{
    client.setBikeId(0);
    m_clientId = 0;
    m_available = true;
}