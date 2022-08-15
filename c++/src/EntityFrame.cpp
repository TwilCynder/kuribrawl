#include "EntityFrame.h"
#include "KBDebug/Debug.h"

/**
 * @brief Construct a new Frame object
 */
EntityFrame::EntityFrame() : 
    movement{
        .x = {0, 0, 0, 0}, 
        .y = {0, 0, 0, 0}
    }
{
}