#pragma once
#include <RakNet/BitStream.h>

#include "../ConfusShared/PacketType.h"
#include "Networking/Connection.h"

namespace ConfusServer
{
    class RemoteFlagUpdater
    {
    private:
        ConfusShared::Flag& m_Flag;
        Networking::Connection& m_Connection;
        ConfusShared::Networking::EPacketType m_PacketType;
    public:
        RemoteFlagUpdater(ConfusShared::Flag& a_Flag, Networking::Connection& a_Connection);
        ~RemoteFlagUpdater();

        void broadcast() const;
    };
}

