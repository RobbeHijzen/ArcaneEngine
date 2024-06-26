#include <stdexcept>
#include <SDL_ttf.h>
#include "Font.h"

using namespace AE;

TTF_Font* Font::GetFont() const {
	return m_Font;
}

Font::Font(const std::string& fullPath, unsigned int size) : m_Font(nullptr)
{
	m_Font = TTF_OpenFont(fullPath.c_str(), size);
	if (m_Font == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

Font::~Font()
{
	TTF_CloseFont(m_Font);
}
