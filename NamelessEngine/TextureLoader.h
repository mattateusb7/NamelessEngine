#pragma once
#include"NamelessCore.h"
#include <vector>
#include <SFML\Graphics\Image.hpp>

namespace _NL {
	namespace Tools {
		class TextureLoader
		{
		public:
			TextureLoader();
			~TextureLoader();
			int LoadImage(const char* filePath);
			GLuint GenerateTexure(const char* filePath, bool bSaveTexID = true);
			void RESET();
			sf::Image LoadedTexture;
			std::vector<GLuint> GLTexIDs;
		};
	}
}

