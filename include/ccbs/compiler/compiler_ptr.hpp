#ifndef CCBS_COMPILER_PTR_HPP
#define CCBS_COMPILER_PTR_HPP

#include <ccbs/util/polymorphic_value.hpp>

namespace ccbs
{

class compiler;
using compiler_ptr = jbcoe::polymorphic_value<compiler>;

} // namespace ccbs

#endif // CCBS_COMPILER_PTR_HPP
