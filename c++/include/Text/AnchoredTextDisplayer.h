#pragma once
#include "TextDisplayer.h"

/**
 * @brief AdvancedTextDisplayer that remembers its starting point.
 */
class AnchoredTextDisplayer : public AdvancedTextDisplayer {
    const Kuribrawl::Vector orig_pos;

    AnchoredTextDisplayer(int x, int y, TextureFont& font);

    void reset();
};