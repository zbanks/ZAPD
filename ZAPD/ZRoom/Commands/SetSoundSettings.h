#pragma once

#include "../ZRoomCommand.h"

class SetSoundSettings : public ZRoomCommand
{
public:
	SetSoundSettings(ZRoom* nZRoom, std::vector<uint8_t> rawData, uint32_t rawDataIndex);

	virtual std::string GenerateSourceCodePass1(std::string roomName,
	                                            uint32_t baseAddress) override;
	virtual std::string GetCommandCName() const override;
	virtual RoomCommand GetRoomCommand() const override;

private:
	uint8_t reverb;
	uint8_t nightTimeSFX;
	uint8_t musicSequence;
};