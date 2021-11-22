#pragma once
#include "TextDisplayer.h"

class AnchoredTextDisplayer : public TextDisplayer {
    const Kuribrawl::Vector orig_pos;

    AnchoredTextDisplayer(int x, int y, TextureFont& font);

    void reset();
};