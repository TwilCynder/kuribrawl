#pragma once

#include "Util/Text/TextureFont.h"
#include <memory>


namespace Kuribrawl {
    namespace Text {
        using FontSPtr = std::shared_ptr<const TextureFont>;
        void setDebugFont(const FontSPtr&);
        void setDebugFont(FontSPtr&&);
        
        FontSPtr& getDebugFont();

        void displayText(const std::string&, int x, int y, SDL_Renderer* target);
    }
}