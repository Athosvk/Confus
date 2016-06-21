#include "../ConfusShared/Flag.h"

#include "RemoteFlagUpdater.h"

namespace ConfusServer
{
    RemoteFlagUpdater::RemoteFlagUpdater(ConfusShared::Flag& a_Flag, Networking::Connection& a_Connection)
        : m_Flag(a_Flag),
        m_Connection(a_Connection)
    {
        m_PacketType = m_Flag.getTeamIdentifier() == ConfusShared::ETeamIdentifier::TeamRed ?
            ConfusShared::Networking::EPacketType::RedFlag :
            ConfusShared::Networking::EPacketType::BlueFlag;
    }


    RemoteFlagUpdater::~RemoteFlagUpdater()
    {
    }

    void RemoteFlagUpdater::broadcast() const
    {
		RakNet::BitStream outputStream;

		outputStream.Write(m_PacketType);
		outputStream.Write(m_Flag.getFlagStatus());
		outputStream.Write(m_Flag.getPosition());

		m_Connection.broadcastPacket(&outputStream);
	}
}
