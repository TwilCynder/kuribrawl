#include "Display/DisplayText.h"


namespace Kuribrawl {
namespace Text {
    namespace {
        static FontSPtr default_debug_font;
    }

    void setDebugFont(const FontSPtr& font_sptr){
        default_debug_font = font_sptr;
    }

    void setDebugFont(FontSPtr&& font_sptr){
        default_debug_font = font_sptr;
    }

    FontSPtr& getDebugFont(){
        return default_debug_font;
    }

    void displayText(const std::string & s, int x, int y, SDL_Renderer *target){
        if (default_debug_font)
            default_debug_font->displayString(s, x, y, target);
    }
}
}


