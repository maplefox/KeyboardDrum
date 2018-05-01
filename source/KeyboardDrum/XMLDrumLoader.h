#pragma once

#include "KeyboardDrum.h"
#include "tinyxml2.h"
#include <string>
#include <map>
#include <vector>


using namespace std;
using namespace tinyxml2;

namespace MapleDrum
{
	namespace Utilities
	{
		struct InitInfo
		{
			InitInfo() {}


			int windowWidth;
			int windowHeight;
			int eventNum;

			struct
			{
				int blockNum;
				struct
				{
					float doubleDiscount;
					float doubleJudge;
					vector<EventDef> eventDefs;
				} input;

				struct
				{
					int mutexNum;
					vector<DrumState> drums;
					vector<AudioData> audioDatas;
				}processor;

				struct
				{
					int bufferNum;
				}audioEngine;
			} audioDrum;

			
			struct
			{
				struct
				{

					float hueWidth;
					float hueVel;
					vector<Model> models;
					vector<ParticleEventDef> emitters;
				}particles;
				struct 
				{
					float fov;
					float radius;
					float kDrag;
					float regress;
					float slope;
					vector<MovEvent> movements;
				}camera;
				struct
				{
					wstring shaderPath;
					vector<XMFLOAT3> vertexes;
				}graphicsEngine;
			} visualDrum;
						
		};


		class XMLDrumLoader
		{
		public:


			XMLDrumLoader();
			~XMLDrumLoader();
			bool LoadFromFile(const char* filename);
			const InitInfo& GetOutput();
			static KeyboardDrum* CreateInstance(InitInfo& initInfo);
			KeyboardDrum* CreateInsntanceFromFile(const char* filename);
		private:
			tinyxml2::XMLDocument doc;
			InitInfo initInfo;
			map<string, byte> keycodeTable;
			map<string, int> audioTable;
			map<string, int> drumTable;
			map<string, int> eventTable;
			map<string, int> modelTable;




			bool LoadAudios(XMLElement* elem);
			bool LoadDrums(XMLElement* elem);
			bool LoadDrum(XMLElement* elem, int index);
			bool LoadAudio(XMLElement* audioElem, string folderPath, int index);

			bool LoadMutexes(XMLElement* elem);
			bool LoadKeyEvents(XMLElement* elem);

			bool LoadModels(XMLElement* elem);
			bool ParseModel(string text, vector<XMFLOAT3>& arrayOut);
			bool ParseVertex(string text, XMFLOAT3& vertex);
			bool LoadParticles(XMLElement* elem);
			bool LoadMovements(XMLElement* elem);
			bool LoadVibs(XMLElement* elem);
			void Reset();

			

		};


	}
}

