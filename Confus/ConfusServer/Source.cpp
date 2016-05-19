//m_Connection->addFunctionToMap(static_cast<unsigned char>(Networking::EPacketType::Player), [this](RakNet::Packet* a_Data)
//{
//	RakNet::BitStream bitstreamIn(a_Data->data, a_Data->length, false);
//
//	RakNet::RakNetGUID playerID;
//	bitstreamIn.IgnoreBytes(sizeof(unsigned char));
//	bitstreamIn.Read(playerID);
//
//	// Only continue if the playerID in the packet matches ours.
//	if(playerID == static_cast<RakNet::RakNetGUID>(ID))
//	{
//		irr::core::vector3df rotation;
//		EPlayerState state;
//		bool forwardKeyPressed;
//		bool backwardKeyPressed;
//		bool leftKeyPressed;
//		bool rightKeyPressed;
//
//		bitstreamIn.Read(rotation);
//		bitstreamIn.Read(state);
//		bitstreamIn.Read(forwardKeyPressed);
//		bitstreamIn.Read(backwardKeyPressed);
//		bitstreamIn.Read(leftKeyPressed);
//		bitstreamIn.Read(rightKeyPressed);
//
//		setRotation(rotation);
//
//		if(state == EPlayerState::LightAttacking)
//		{
//			startLightAttack();
//		}
//		else if(state == EPlayerState::HeavyAttacking)
//		{
//			startHeavyAttack();
//		}
//
//		auto movementDirection = irr::core::vector3df();
//		if(forwardKeyPressed)
//		{
//			movementDirection.Z = 1.0f;
//		}
//		else if(backwardKeyPressed)
//		{
//			movementDirection.Z = -1.0f;
//		}
//		if(leftKeyPressed)
//		{
//			movementDirection.X = -1.0f;
//		}
//		else if(rightKeyPressed)
//		{
//			movementDirection.X = 1.0f;
//		}
//	}
//});