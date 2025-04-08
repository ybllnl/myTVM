#ifndef MYTVM_RELAY_TRANSFORM_H
#define MYTVM_RELAY_TRANSFORM_H

#include <vector>
#include <string>

namespace mytvm{
namespace transform{

class PassContext{
public:
    /*! \brief The optimization level for the pass. */
    int opt_level{2};

    /*! \brief A list of passes that must be executed, regardless of optimization level.*/
    std::vector<std::string> required_passes;

    /*! \brief A list of passes that should be skipped, even if normally included.*/
    std::vector<std::string> disabled_passes;

    PassContext() = default;
    
    static PassContext Create() {
        return PassContext();
    }
};

}
}

#endif
