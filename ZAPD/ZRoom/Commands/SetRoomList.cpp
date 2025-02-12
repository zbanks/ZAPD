#include "SetRoomList.h"
#include "../../BitConverter.h"
#include "../../Globals.h"
#include "../../StringHelper.h"
#include "../../ZFile.h"
#include "../ZRoom.h"

SetRoomList::SetRoomList(ZRoom* nZRoom, std::vector<uint8_t> rawData, uint32_t rawDataIndex)
	: ZRoomCommand(nZRoom, rawData, rawDataIndex)
{
	int32_t numRooms = rawData[rawDataIndex + 1];
	segmentOffset = BitConverter::ToInt32BE(rawData, rawDataIndex + 4) & 0x00FFFFFF;

	rooms = std::vector<RoomEntry*>();

	int32_t currentPtr = segmentOffset;

	for (int32_t i = 0; i < numRooms; i++)
	{
		RoomEntry* entry = new RoomEntry(rawData, currentPtr);
		rooms.push_back(entry);

		currentPtr += 8;
	}

	zRoom->roomCount = numRooms;
}

SetRoomList::~SetRoomList()
{
	for (RoomEntry* entry : rooms)
		delete entry;
}

std::string SetRoomList::GenerateSourceCodePass1(std::string roomName, uint32_t baseAddress)
{
	return StringHelper::Sprintf(
		"%s 0x%02X, (u32)&%sRoomList0x%06X",
		ZRoomCommand::GenerateSourceCodePass1(roomName, baseAddress).c_str(), rooms.size(),
		zRoom->GetName().c_str(), segmentOffset);
}

std::string SetRoomList::GenerateSourceCodePass2(std::string roomName, uint32_t baseAddress)
{
	return "";
}

std::string SetRoomList::GenerateExterns() const
{
	return StringHelper::Sprintf("extern RomFile %sRoomList0x%06X[];\n", zRoom->GetName().c_str(),
	                             segmentOffset);
}

std::string SetRoomList::GetCommandCName() const
{
	return "SCmdRoomList";
}

RoomCommand SetRoomList::GetRoomCommand() const
{
	return RoomCommand::SetRoomList;
}

std::string SetRoomList::PreGenSourceFiles()
{
	std::string declaration = "";

	for (ZFile* file : Globals::Instance->files)
	{
		for (ZResource* res : file->resources)
		{
			if (res->GetResourceType() == ZResourceType::Room && res != zRoom)
			{
				std::string roomName = res->GetName();
				declaration += StringHelper::Sprintf(
					"    { (u32)_%sSegmentRomStart, (u32)_%sSegmentRomEnd },\n", roomName.c_str(),
					roomName.c_str());
			}
		}
	}

	zRoom->parent->AddDeclarationArray(
		segmentOffset, DeclarationAlignment::None, rooms.size() * 8, "RomFile",
		StringHelper::Sprintf("%sRoomList0x%06X", zRoom->GetName().c_str(), segmentOffset), 0,
		declaration);

	return std::string();
}

std::string SetRoomList::Save()
{
	return std::string();
}

RoomEntry::RoomEntry(uint32_t nVAS, uint32_t nVAE)
{
	virtualAddressStart = nVAS;
	virtualAddressEnd = nVAE;
}

RoomEntry::RoomEntry(std::vector<uint8_t> rawData, uint32_t rawDataIndex)
	: RoomEntry(BitConverter::ToInt32BE(rawData, rawDataIndex + 0),
                BitConverter::ToInt32BE(rawData, rawDataIndex + 4))
{
}