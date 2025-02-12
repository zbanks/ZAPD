#include "HLAnimationIntermediette.h"

using namespace tinyxml2;

HLAnimationIntermediette::HLAnimationIntermediette()
{
	limit = 0;
	limbCount = 0;
	frameCount = 0;
	rotationValues = std::vector<uint16_t>();
	rotationIndices = std::vector<RotationIndex>();
}

HLAnimationIntermediette::~HLAnimationIntermediette()
{
}

HLAnimationIntermediette* HLAnimationIntermediette::FromXML(std::string xmlPath)
{
	HLAnimationIntermediette* anim = new HLAnimationIntermediette();
	XMLDocument doc;

	doc.LoadFile(xmlPath.c_str());

	XMLElement* root = doc.RootElement();

	anim->limit = root->IntAttribute("Limit");
	anim->limbCount = root->IntAttribute("LimbCount");
	anim->frameCount = root->IntAttribute("FrameCount");

	for (XMLElement* child = root->FirstChildElement(); child != NULL;
	     child = child->NextSiblingElement())
	{
		if (std::string(child->Name()) == "RotationValues")
		{
			for (XMLElement* child2 = child->FirstChildElement(); child2 != NULL;
			     child2 = child2->NextSiblingElement())
			{
				std::string value = child2->GetText();
				anim->rotationValues.push_back(atoi(value.c_str()));
			}
		}
		else if (std::string(child->Name()) == "RotationIndices")
		{
			for (XMLElement* child2 = child->FirstChildElement(); child2 != NULL;
			     child2 = child2->NextSiblingElement())
				anim->rotationIndices.push_back(RotationIndex(child2->IntAttribute("X"),
				                                              child2->IntAttribute("Y"),
				                                              child2->IntAttribute("Z")));
		}
	}

	return anim;
}

HLAnimationIntermediette* HLAnimationIntermediette::FromZAnimation(ZAnimation* zAnim)
{
	HLAnimationIntermediette* anim = new HLAnimationIntermediette();

	return anim;
}

ZAnimation* HLAnimationIntermediette::ToZAnimation()
{
	ZAnimation* zAnim = new ZAnimation(nullptr);

	return zAnim;
}

std::string HLAnimationIntermediette::OutputXML()
{
	std::string output = "";
	XMLDocument doc;

	XMLElement* root = doc.NewElement("HLAnimationIntermediette");
	root->SetAttribute("Limit", limit);
	root->SetAttribute("LimbCount", limbCount);
	root->SetAttribute("FrameCount", frameCount);

	doc.InsertFirstChild(root);

	XMLElement* rotValues = doc.NewElement("RotationValues");

	for (size_t i = 0; i < rotationValues.size(); i++)
	{
		XMLElement* rotValue = doc.NewElement("Value");
		rotValue->SetText(rotationValues[i]);
		rotValues->InsertEndChild(rotValue);
	}

	root->InsertEndChild(rotValues);

	XMLElement* rotIndices = doc.NewElement("RotationIndices");

	for (size_t i = 0; i < rotationIndices.size(); i++)
	{
		XMLElement* rotIndex = doc.NewElement("Value");
		rotIndex->SetAttribute("X", rotationIndices[i].x);
		rotIndex->SetAttribute("Y", rotationIndices[i].y);
		rotIndex->SetAttribute("Z", rotationIndices[i].z);
		rotIndices->InsertEndChild(rotIndex);
	}

	root->InsertEndChild(rotIndices);

	XMLPrinter printer;

	doc.Accept(&printer);
	return printer.CStr();
}