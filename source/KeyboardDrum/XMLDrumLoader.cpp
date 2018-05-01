#include "XMLDrumLoader.h"
#include "LibraryLoader.h"
#include <mmreg.h>


using namespace MapleDrum::Utilities;

XMLDrumLoader::XMLDrumLoader()
{
	keycodeTable["ESCAPE"] = 0x01;
	keycodeTable["1"] = 0x02;
	keycodeTable["2"] = 0x03;
	keycodeTable["3"] = 0x04;
	keycodeTable["4"] = 0x05;
	keycodeTable["5"] = 0x06;
	keycodeTable["6"] = 0x07;
	keycodeTable["7"] = 0x08;
	keycodeTable["8"] = 0x09;
	keycodeTable["9"] = 0x0A;
	keycodeTable["0"] = 0x0B;
	keycodeTable["MINUS"] = 0x0C;
	keycodeTable["EQUALS"] = 0x0D;
	keycodeTable["BACK"] = 0x0E;
	keycodeTable["TAB"] = 0x0F;
	keycodeTable["Q"] = 0x10;
	keycodeTable["W"] = 0x11;
	keycodeTable["E"] = 0x12;
	keycodeTable["R"] = 0x13;
	keycodeTable["T"] = 0x14;
	keycodeTable["Y"] = 0x15;
	keycodeTable["U"] = 0x16;
	keycodeTable["I"] = 0x17;
	keycodeTable["O"] = 0x18;
	keycodeTable["P"] = 0x19;
	keycodeTable["LBRACKET"] = 0x1A;
	keycodeTable["RBRACKET"] = 0x1B;
	keycodeTable["RETURN"] = 0x1C;
	keycodeTable["LCONTROL"] = 0x1D;
	keycodeTable["A"] = 0x1E;
	keycodeTable["S"] = 0x1F;
	keycodeTable["D"] = 0x20;
	keycodeTable["F"] = 0x21;
	keycodeTable["G"] = 0x22;
	keycodeTable["H"] = 0x23;
	keycodeTable["J"] = 0x24;
	keycodeTable["K"] = 0x25;
	keycodeTable["L"] = 0x26;
	keycodeTable["SEMICOLON"] = 0x27;
	keycodeTable["APOSTROPHE"] = 0x28;
	keycodeTable["GRAVE"] = 0x29;
	keycodeTable["LSHIFT"] = 0x2A;
	keycodeTable["BACKSLASH"] = 0x2B;
	keycodeTable["Z"] = 0x2C;
	keycodeTable["X"] = 0x2D;
	keycodeTable["C"] = 0x2E;
	keycodeTable["V"] = 0x2F;
	keycodeTable["B"] = 0x30;
	keycodeTable["N"] = 0x31;
	keycodeTable["M"] = 0x32;
	keycodeTable["COMMA"] = 0x33;
	keycodeTable["PERIOD"] = 0x34;
	keycodeTable["SLASH"] = 0x35;
	keycodeTable["RSHIFT"] = 0x36;
	keycodeTable["MULTIPLY"] = 0x37;
	keycodeTable["LMENU"] = 0x38;
	keycodeTable["SPACE"] = 0x39;
	keycodeTable["CAPITAL"] = 0x3A;
	keycodeTable["F1"] = 0x3B;
	keycodeTable["F2"] = 0x3C;
	keycodeTable["F3"] = 0x3D;
	keycodeTable["F4"] = 0x3E;
	keycodeTable["F5"] = 0x3F;
	keycodeTable["F6"] = 0x40;
	keycodeTable["F7"] = 0x41;
	keycodeTable["F8"] = 0x42;
	keycodeTable["F9"] = 0x43;
	keycodeTable["F10"] = 0x44;
	keycodeTable["NUMLOCK"] = 0x45;
	keycodeTable["SCROLL"] = 0x46;
	keycodeTable["NUMPAD7"] = 0x47;
	keycodeTable["NUMPAD8"] = 0x48;
	keycodeTable["NUMPAD9"] = 0x49;
	keycodeTable["SUBTRACT"] = 0x4A;
	keycodeTable["NUMPAD4"] = 0x4B;
	keycodeTable["NUMPAD5"] = 0x4C;
	keycodeTable["NUMPAD6"] = 0x4D;
	keycodeTable["ADD"] = 0x4E;
	keycodeTable["NUMPAD1"] = 0x4F;
	keycodeTable["NUMPAD2"] = 0x50;
	keycodeTable["NUMPAD3"] = 0x51;
	keycodeTable["NUMPAD0"] = 0x52;
	keycodeTable["DECIMAL"] = 0x53;
	keycodeTable["OEM_102"] = 0x56;
	keycodeTable["F11"] = 0x57;
	keycodeTable["F12"] = 0x58;
	keycodeTable["F13"] = 0x64;
	keycodeTable["F14"] = 0x65;
	keycodeTable["F15"] = 0x66;
	keycodeTable["KANA"] = 0x70;
	keycodeTable["ABNT_C1"] = 0x73;
	keycodeTable["CONVERT"] = 0x79;
	keycodeTable["NOCONVERT"] = 0x7B;
	keycodeTable["YEN"] = 0x7D;
	keycodeTable["ABNT_C2"] = 0x7E;
	keycodeTable["NUMPADEQUALS"] = 0x8D;
	keycodeTable["PREVTRACK"] = 0x90;
	keycodeTable["AT"] = 0x91;
	keycodeTable["COLON"] = 0x92;
	keycodeTable["UNDERLINE"] = 0x93;
	keycodeTable["KANJI"] = 0x94;
	keycodeTable["STOP"] = 0x95;
	keycodeTable["AX"] = 0x96;
	keycodeTable["UNLABELED"] = 0x97;
	keycodeTable["NEXTTRACK"] = 0x99;
	keycodeTable["NUMPADENTER"] = 0x9C;
	keycodeTable["RCONTROL"] = 0x9D;
	keycodeTable["MUTE"] = 0xA0;
	keycodeTable["CALCULATOR"] = 0xA1;
	keycodeTable["PLAYPAUSE"] = 0xA2;
	keycodeTable["MEDIASTOP"] = 0xA4;
	keycodeTable["VOLUMEDOWN"] = 0xAE;
	keycodeTable["VOLUMEUP"] = 0xB0;
	keycodeTable["WEBHOME"] = 0xB2;
	keycodeTable["NUMPADCOMMA"] = 0xB3;
	keycodeTable["DIVIDE"] = 0xB5;
	keycodeTable["SYSRQ"] = 0xB7;
	keycodeTable["RMENU"] = 0xB8;
	keycodeTable["PAUSE"] = 0xC5;
	keycodeTable["HOME"] = 0xC7;
	keycodeTable["UP"] = 0xC8;
	keycodeTable["PRIOR"] = 0xC9;
	keycodeTable["LEFT"] = 0xCB;
	keycodeTable["RIGHT"] = 0xCD;
	keycodeTable["END"] = 0xCF;
	keycodeTable["DOWN"] = 0xD0;
	keycodeTable["NEXT"] = 0xD1;
	keycodeTable["INSERT"] = 0xD2;
	keycodeTable["DELETE"] = 0xD3;
	keycodeTable["LWIN"] = 0xDB;
	keycodeTable["RWIN"] = 0xDC;
	keycodeTable["APPS"] = 0xDD;
	keycodeTable["POWER"] = 0xDE;
	keycodeTable["SLEEP"] = 0xDF;
	keycodeTable["WAKE"] = 0xE3;
	keycodeTable["WEBSEARCH"] = 0xE5;
	keycodeTable["WEBFAVORITES"] = 0xE6;
	keycodeTable["WEBREFRESH"] = 0xE7;
	keycodeTable["WEBSTOP"] = 0xE8;
	keycodeTable["WEBFORWARD"] = 0xE9;
	keycodeTable["WEBBACK"] = 0xEA;
	keycodeTable["MYCOMPUTER"] = 0xEB;
	keycodeTable["MAIL"] = 0xEC;
	keycodeTable["MEDIASELECT"] = 0xED;
}


XMLDrumLoader::~XMLDrumLoader()
{

}




bool XMLDrumLoader::LoadDrum(XMLElement* drumElem, int index)
{
	
	if (!drumElem)
		return false;
	
	auto name = drumElem->Attribute("name");
	auto audioName = drumElem->Attribute("audioName");
	if (!( name && audioName))
		return false;

	DrumState drum;
	drum.id = index;
	drum.audioId = audioTable[audioName];
	drum.mutexId = -1;
	
	
	

	if (auto playEvent = drumElem->Attribute("playEvent"))
		drum.keyPlayId = eventTable[playEvent];
	else
		drum.keyPlayId = -1;
	if(auto stopEvent = drumElem->Attribute("stopEvent"))
		drum.keyStopId = eventTable[stopEvent];
	else
		drum.keyStopId = -1;
	drum.volume = drumElem->FloatAttribute("volume", 1.0f);
	drum.switchLength = drumElem->IntAttribute("switchLength", 400);
	
	drum.disappearDefer = drumElem->IntAttribute("disappearDefer", 0);
	drum.disappearLength = drumElem->IntAttribute("disappearLength", 400);
	
	
	drumTable[name] = index;
	
	initInfo.audioDrum.processor.drums.push_back(drum);
	return true;

}

bool XMLDrumLoader::LoadDrums(XMLElement* drumsElem)
{
	
	
	auto& drums = initInfo.audioDrum.processor.drums;
	auto drumElem = drumsElem->FirstChildElement("Drum");
	int i = 0;
	while (drumElem)
	{
		
		
		LoadDrum(drumElem,i);
		drumElem = drumElem->NextSiblingElement("Drum");
		i++;
	}
	return true;
	
}


bool XMLDrumLoader::LoadFromFile(const char* filename)
{
	doc.LoadFile(filename);
	
	auto error = doc.ErrorName();
	if (doc.Error())
	{
		return false;
	}
	
	XMLElement* rootElem = doc.FirstChildElement();       //DrumConfig
	
	
	XMLElement* keyboardDrum = rootElem->FirstChildElement("KeyboardDrum");
	if (!keyboardDrum)
		return false;
	initInfo.windowWidth = keyboardDrum->IntAttribute("windowWidth", 1280);
	initInfo.windowHeight = keyboardDrum->IntAttribute("windowHeight", 720);
	
	auto audioElem = keyboardDrum->FirstChildElement("AudioDrum");
	initInfo.audioDrum.blockNum = audioElem->IntAttribute("blockNum", 128);
	
	auto inputElem = audioElem->FirstChildElement("Input");
	auto& input = initInfo.audioDrum.input;

	auto keyEventsElem = inputElem->FirstChildElement("KeyEvents");
	LoadKeyEvents(keyEventsElem);

	auto processorElem = audioElem->FirstChildElement("Processor");
	auto audioDatasElem = processorElem->FirstChildElement("AudioDatas");
	auto drumsElem = processorElem->FirstChildElement("Drums");
	auto mutexesElem = processorElem->FirstChildElement("PlayMutexes");
	LoadAudios(audioDatasElem);
	LoadDrums(drumsElem);
	LoadMutexes(mutexesElem);

	auto aEngineElem = audioElem->FirstChildElement("AudioEngine");

	initInfo.audioDrum.audioEngine.bufferNum = aEngineElem->IntAttribute("bufferNum", 8);

	
	auto visualElem = keyboardDrum->FirstChildElement("VisualDrum");

	auto particlesElem = visualElem->FirstChildElement("Particles");
	auto& particles = initInfo.visualDrum.particles;
	particles.hueVel = particlesElem->FloatAttribute("hueVel", 0.0f);
	particles.hueWidth = particlesElem->FloatAttribute("hueWidth", 1.0f);

	XMLElement* modelsElem = particlesElem->FirstChildElement("Models");
	XMLElement* emittersElem = particlesElem->FirstChildElement("Emitters");
	LoadModels(modelsElem);
	LoadParticles(emittersElem);

	
	auto cameraElem = visualElem->FirstChildElement("Camera");
	auto& camera = initInfo.visualDrum.camera;
	camera.fov = cameraElem->FloatAttribute("fov", 0.8f);
	camera.radius = cameraElem->FloatAttribute("radius", 100.0f);
	camera.kDrag = cameraElem->FloatAttribute("kDrag", 1.0f);
	camera.regress = cameraElem->FloatAttribute("regress", 10.0f);
	camera.slope = cameraElem->FloatAttribute("slope", 10.0f);
	
	auto movsElem = cameraElem->FirstChildElement("Movements");
	LoadMovements(movsElem);

	auto graphicElem = visualElem->FirstChildElement("GraphicsEngine");
	initInfo.visualDrum.graphicsEngine.shaderPath = L"BasicEffect.fx";;
	//initInfo.visualDrum.graphicsEngine.shaderPath = graphicElem->Attribute("shaderPath");
	
	return true;
	
}

const InitInfo& XMLDrumLoader::GetOutput()
{
	return initInfo;
}

bool XMLDrumLoader::LoadAudio(XMLElement* audioElem, string folderPath, int index)
{
	auto name = audioElem->Attribute("name");
	auto path = audioElem->Attribute("path");
	if (!(name && path))
		return false;

	string fullPath = folderPath + path;
	RawAudio rawAudio;
	AudioData tmpAudio;
	if (!LoadRawAudio(fullPath, rawAudio))
		return false;
	if (memcmp(&rawAudio.format, &AudioDrum::DefaultFormat, sizeof(WAVEFORMATEX)))
		return false;


	tmpAudio.dataPtr = (float*)rawAudio.data;
	tmpAudio.blockNum = rawAudio.size / rawAudio.format.nBlockAlign;
	initInfo.audioDrum.processor.audioDatas.push_back(tmpAudio);
	audioTable[name] = index;
	return true;
}

bool XMLDrumLoader::LoadAudios(XMLElement* audiosElem)
{
	

	auto& audioDatas = initInfo.audioDrum.processor.audioDatas;

	string folderPath = "";
	if (auto tmp = audiosElem->Attribute("folderPath"))
		folderPath = tmp;
	auto audioElem = audiosElem->FirstChildElement("AudioData");
	
	int i = 0;
	while (audioElem)
	{
		if(LoadAudio(audioElem, folderPath, i))
			i++;
		audioElem = audioElem->NextSiblingElement("AudioData");
		
	}
	return true;
}
bool XMLDrumLoader::LoadMutexes(XMLElement* mutexesElem)
{
	
	auto& drums = initInfo.audioDrum.processor.drums;
	auto mutexElem = mutexesElem->FirstChildElement("PlayMutex");
	int i = 0;
	while (mutexElem)
	{
		
		auto mutexDrum = mutexElem->FirstChildElement("MutexDrum");
		
		while (mutexDrum)
		{
			if (auto name = mutexDrum->Attribute("name"))
			{
				
				drums[drumTable[name]].mutexId = i;
			}
			
			mutexDrum = mutexDrum->NextSiblingElement("MutexDrum");
		}

		mutexElem = mutexElem->NextSiblingElement("PlayMutex");
		i++;
	}

	initInfo.audioDrum.processor.mutexNum = i;
	return true;
}

bool XMLDrumLoader::LoadKeyEvents(XMLElement* keyEventsElem)
{
	

	
	auto defualtValue = keyEventsElem->FloatAttribute("value", 1.0);
	auto& eventDefs = initInfo.audioDrum.input.eventDefs;
	
	auto keyEventElem = keyEventsElem->FirstChildElement("KeyEvent");
	
	int i = 0;
	while (keyEventElem)
	{
		

		auto eventName = keyEventElem->Attribute("name");
		eventTable[eventName] = i;

		auto keyElem = keyEventElem->FirstChildElement("Key");
		
		while (keyElem)
		{
			auto name = keyElem->Attribute("name");
			auto action = keyElem->Attribute("action");
			if (name && action)
			{
				string saction = action;
				EventDef evtDef;
				evtDef.keyAction = saction == "release" ? Release : Press;
				evtDef.keycode = keycodeTable[name];
				evtDef.eventId = i;
				evtDef.value = keyElem->FloatAttribute("value", defualtValue);
				eventDefs.push_back(evtDef);
			}
			keyElem = keyElem->NextSiblingElement("Key");
		}
		
		keyEventElem = keyEventElem->NextSiblingElement("KeyEvent");
		i++;
	}

	initInfo.eventNum = i;
	return true;
	
}


bool XMLDrumLoader::ParseModel(string text,vector<XMFLOAT3>& vertexes)
{
	int level = 0;
	int ptBegin, ptEd;
	
	for (int i = 0; i < text.size(); i++)
	{
		bool parseVertex = false;
		switch (text[i])
		{
		case '[':
			if (level == 0)
			{
				ptBegin = i+1;
			}
				
			level++;
			
			break;
		case ']':
			
			level--;
			if (level == 0)
				parseVertex = true;
			break;
		case ',':
			if (level == 1)
				parseVertex = true;
			break;
		}
		if (parseVertex)
		{
			ptEd = i;
			auto subText = text.substr(ptBegin, ptEd - ptBegin);
			XMFLOAT3 vertex;
			if (ParseVertex(subText, vertex))
				vertexes.push_back(vertex);
			else
				return false;

			ptBegin = i + 1;
		}
	}
	int verNum = vertexes.size();
	if (verNum == 0 || verNum % 3 != 0)
		return false;
	return true;
}
bool XMLDrumLoader::ParseVertex(string text, XMFLOAT3& vertex)
{
	if (sscanf(text.c_str(), " [ %f , %f , %f \0] ", &vertex.x, &vertex.y, &vertex.z) < 3)
		return false;
	else return true;
}
bool XMLDrumLoader::LoadModels(XMLElement* elem)
{
	auto& vertexes = initInfo.visualDrum.graphicsEngine.vertexes;
	auto& models = initInfo.visualDrum.particles.models;
	Model model = { 0 };
	int modelIndex = 0;
	auto modelElem = elem->FirstChildElement("Model");
	while (modelElem)
	{
		auto name = modelElem->Attribute("name");
		if (!name)
		{
			continue;
		}
		auto text = modelElem->GetText();
		if (!text)
			continue;
		vector<XMFLOAT3> subverts;
		if (!ParseModel(text, subverts))
			continue;
		
		

		vertexes.insert(vertexes.end(), subverts.begin(), subverts.end());
		model.vertexNum = subverts.size();
		models.push_back(model);
		modelTable[name] = modelIndex;
		model.vertexStart += model.vertexNum;
		
		modelElem = modelElem->NextSiblingElement("Model");
		modelIndex++;
	}
	return true;
}

bool XMLDrumLoader::LoadParticles(XMLElement* elem)
{
	auto emitterElem = elem->FirstChildElement("Emitter");
	while (emitterElem)
	{
		
		ParticleEventDef emitter;
		auto eventName = emitterElem->Attribute("event");
		emitter.eventId = eventTable[eventName];
		auto modelName = emitterElem->Attribute("model");
		emitter.modelId = modelTable[modelName];
		emitter.lifetime = emitterElem->FloatAttribute("lifetime", 1.0f);
		
		emitter.objectNum = emitterElem->IntAttribute("objNum", 1);
		emitter.hue = emitterElem->FloatAttribute("hue", 0.0f);
		emitter.gamma = emitterElem->FloatAttribute("gamma", 1.0f);
		emitter.brightness = emitterElem->FloatAttribute("brightness", 1.0f);
		emitter.kDrag = emitterElem->FloatAttribute("kDrag", 0.0f);
		emitter.scale = emitterElem->FloatAttribute("scale", 1.0f);
		emitter.appearSt = emitterElem->FloatAttribute("appearPlace", 10.0f);
		emitter.spreadSpeed = emitterElem->FloatAttribute("spreadSpeed", 100.0f);
		emitter.selfRotateSpeed = emitterElem->FloatAttribute("selfRotateSpeed", 1.0f);
		emitter.axisRotateSpeed = emitterElem->FloatAttribute("axisRotateSpeed", 0.0f);
		initInfo.visualDrum.particles.emitters.push_back(emitter);
		emitterElem = emitterElem->NextSiblingElement("Emitter");
	}
	return true;
}


bool XMLDrumLoader::LoadMovements(XMLElement* elem)

{
	auto movElem = elem->FirstChildElement("Movement");
	while (movElem)
	{

		MovEvent evtDef;
		evtDef.eventId = eventTable[movElem->Attribute("event")];
		evtDef.rad = movElem->FloatAttribute("rad");
		evtDef.power = movElem->FloatAttribute("power", 1.0f);
		evtDef.rollAcc = movElem->FloatAttribute("roll", 1.0f);
		initInfo.visualDrum.camera.movements.push_back(evtDef);
		movElem = movElem->NextSiblingElement("Movement");

	}
	return true;
}

bool XMLDrumLoader::LoadVibs(XMLElement* elem)

{
	auto vibElem = elem->FirstChildElement("Vib");
	while (vibElem)
	{

		VibDef evtDef;
		evtDef.eventId = eventTable[vibElem->Attribute("event")];
		evtDef.vib.amplitude = vibElem->FloatAttribute("amplitude",1.0f);
		evtDef.vib.decayRate = vibElem->FloatAttribute("decayRate", 1.0f);
		evtDef.vib.endAmp = vibElem->FloatAttribute("endAmp", 0.01f);
		evtDef.vib.frequency = vibElem->FloatAttribute("frequency", 20);
		evtDef.vib.phase = 0;

		auto& direction = evtDef.vib.direction;
		auto textDir = vibElem->Attribute("direction");
		sscanf(textDir, " %f , %f , %f ", &direction.x, &direction.y, &direction.z);

		//initInfo.vibEvtDefs.push_back(evtDef);
		vibElem = vibElem->NextSiblingElement("Vib");

	}
	return true;
}

KeyboardDrum* XMLDrumLoader::CreateInstance(InitInfo& initInfo)
{
	auto& adi = initInfo.audioDrum;
	auto& dii = adi.input;
	auto drumInput = new DrumInput(initInfo.eventNum, dii.eventDefs, dii.doubleDiscount, dii.doubleJudge);
	auto& dpi = adi.processor;
	auto drumProcessor = new DrumProcessor(adi.blockNum, AudioDrum::DefaultFormat.nChannels, 
		dpi.mutexNum, dpi.drums, dpi.audioDatas);
	auto& aei = adi.audioEngine;
	auto audioEngine = new AudioEngine(aei.bufferNum, adi.blockNum, AudioDrum::DefaultFormat);
	auto audioDrum = new AudioDrum(drumInput, drumProcessor, audioEngine);

	auto& vdi = initInfo.visualDrum;
	auto& pi = vdi.particles;
	auto particles = new Particles(pi.hueWidth, pi.hueVel, pi.models, pi.emitters);
	auto& ci = vdi.camera;
	auto camera = new Camera(ci.fov, float(initInfo.windowWidth) / initInfo.windowHeight, ci.radius, ci.kDrag,ci.regress,ci.slope, ci.movements);
	auto& gei = vdi.graphicsEngine;
	auto graphicsEngine = new GraphicsEngine(gei.shaderPath, gei.vertexes, initInfo.windowWidth, initInfo.windowHeight);
	auto visualDrum = new VisualDrum(initInfo.eventNum, camera, particles, graphicsEngine);
	auto keyboardDrum = new KeyboardDrum(initInfo.windowWidth, initInfo.windowHeight, audioDrum, visualDrum);
	return keyboardDrum;
}

KeyboardDrum* XMLDrumLoader::CreateInsntanceFromFile(const char* filename)
{
	auto success = LoadFromFile(filename);
	if (success)
		return CreateInstance(initInfo);
	else
		return nullptr;
}

void XMLDrumLoader::Reset()
{
	initInfo = InitInfo();
	audioTable.clear();
	drumTable.clear();
	eventTable.clear();
	modelTable.clear();
}