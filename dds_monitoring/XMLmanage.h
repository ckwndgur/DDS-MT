#pragma once
#include <string>
#include <sstream>
#include "tinyxml2.h"

using namespace std;

class XMLmanage {

	public:
		XMLmanage(void);
		~XMLmanage(void);

		void LoadXMLFile(char* root);
		void parseXML(tinyxml2::XMLElement* elem);

	};