#pragma once

namespace Kuribrawl {
    /**
     * @brief The state of a Fighter, definind the actions they can take and the behavior of the Fighter as an entity.  
     * See [states.md](md_doc_files_states.html).  
     * A Fighter can have only one State at a time.
     * A state being "started" means that the current state of a Fighter was set to this state.
     * @note States don't care about whether a Fighter is on the ground or not, certain states can very well be used no matter that ; what defines if a Fighter is grounded or not is
     * its \ref Fighter#grounded "grounded" attribute, which is updated each frame (based of course on terrain collision)
     */
    enum class FighterState {
        #include "states.enum"
        STATES          ///< Never used, is (because of how enums work) the total number of states.
    };
}