#pragma once

#include <string>

#include "../ZRoom.h"
#include "../ZRoomCommand.h"
#include "ZFile.h"

class SetLightList : public ZRoomCommand
{
public:
	SetLightList(ZRoom* nZRoom, std::vector<uint8_t> rawData, uint32_t rawDataIndex);

	virtual std::string GenerateSourceCodePass1(std::string roomName,
	                                            uint32_t baseAddress) override;
	virtual std::string GetCommandCName() const override;
	virtual RoomCommand GetRoomCommand() const override;
	virtual std::string GenerateExterns() const override;

private:
	uint8_t numLights;
	uint32_t segment;

	ZRoom* ptrRoom;
};