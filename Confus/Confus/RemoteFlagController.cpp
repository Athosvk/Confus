#include "../ConfusShared/Flag.h"
#include "../ConfusShared/TeamIdentifier.h"
#include "../ConfusShared/PacketType.h"
#include "RemoteFlagController.h"

#include <iostream>
namespace Confus
{
    RemoteFlagController::RemoteFlagController(ConfusShared::Flag& a_Flag, Networking::ClientConnection& a_Connection)
        :m_Flag(a_Flag),
        m_Connection(a_Connection)
    {
        // Determine the packettype (red or blue flag).
        ConfusShared::Networking::EPacketType packetType = m_Flag.getTeamIdentifier() == ConfusShared::ETeamIdentifier::TeamRed ?
            ConfusShared::Networking::EPacketType::RedFlag :
            ConfusShared::Networking::EPacketType::BlueFlag;

        m_Connection.addFunctionToMap(static_cast<unsigned char>(packetType), [this](RakNet::Packet* a_Packet)
        {
            RakNet::BitStream inputStream(a_Packet->data, a_Packet->length, false);

            ConfusShared::EFlagEnum flagStatus;
            irr::core::vector3df flagPosition;
            inputStream.IgnoreBytes(sizeof(unsigned char));

            inputStream.Read(flagStatus);
            inputStream.Read(flagPosition);

            m_Flag.setFlagStatus(flagStatus);
            m_Flag.setPosition(flagPosition);
        });
    }

    RemoteFlagController::~RemoteFlagController()
    {
    }
}
