#pragma once

#include "Networking/ClientConnection.h"

namespace Confus
{
    class RemoteFlagController
    {
    private:
        ConfusShared::Flag& m_Flag;
        Networking::ClientConnection& m_Connection;
    public:
        RemoteFlagController(ConfusShared::Flag& a_Flag, Networking::ClientConnection& a_Connection);
        ~RemoteFlagController();
    };
}

