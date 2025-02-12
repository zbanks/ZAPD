#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include "ZResource.h"
#include "ZScalar.h"
#include "tinyxml2.h"

class ZVtx : public ZResource
{
public:
	int16_t x, y, z;
	uint16_t flag;
	int16_t s, t;
	uint8_t r, g, b, a;

	ZVtx(ZFile* nParent);

	void ExtractFromXML(tinyxml2::XMLElement* reader, const std::vector<uint8_t>& nRawData,
	                    const uint32_t nRawDataIndex) override;

	void ParseRawData() override;
	std::string GetBodySourceCode() const;
	std::string GetSourceOutputCode(const std::string& prefix) override;

	bool IsExternalResource() const override;
	bool DoesSupportArray() const override;
	std::string GetSourceTypeName() const override;
	ZResourceType GetResourceType() const override;
	virtual std::string GetExternalExtension() const override;

	size_t GetRawDataSize() const override;

protected:
};