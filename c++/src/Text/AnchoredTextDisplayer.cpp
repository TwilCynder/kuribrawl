#include "Text/AnchoredTextDisplayer.h"

AnchoredTextDisplayer::AnchoredTextDisplayer(int x, int y, TextureFont& font) : 
    AdvancedTextDisplayer(x, y, font),
    orig_pos {x, y}
{
}

void AnchoredTextDisplayer::reset(){
    pos.x = orig_pos.x;
    pos.y = orig_pos.y;
}