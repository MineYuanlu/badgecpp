#include "badgecpp/version.hpp"

namespace badge {
    int version::major() {
#ifdef BADGECPP_VERSION_MAJOR
        return BADGECPP_VERSION_MAJOR;
#else
        return 0;
#endif
    }
    int version::minor() {
#ifdef BADGECPP_VERSION_MINOR
        return BADGECPP_VERSION_MINOR;
#else
        return 0;
#endif
    }
    int version::patch() {
#ifdef BADGECPP_VERSION_PATCH
        return BADGECPP_VERSION_PATCH;
#else
        return 0;
#endif
    }
    const char *version::special() {
#ifdef BADGECPP_VERSION_SPECIAL
        return BADGECPP_VERSION_SPECIAL;
#else
        return nullptr;
#endif
    }
    const char *version::string() {
#ifdef BADGECPP_VERSION_STRING
        return BADGECPP_VERSION_STRING;
#else
        return nullptr;
#endif
    }
    const char *version::build_type() {
#ifdef BADGECPP_BUILD_TYPE
        return BADGECPP_BUILD_TYPE;
#else
        return nullptr;
#endif
    }
    bool version::is_debug() {
#ifdef BADGECPP_BUILD_TYPE
        return BADGECPP_BUILD_TYPE[0] == 'D';
#else
        return false;
#endif
    }
}// namespace badge