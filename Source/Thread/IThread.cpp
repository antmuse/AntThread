#include "IThread.h"

#if defined( APP_PLATFORM_WINDOWS )
#include <process.h>
#endif
#if defined( APP_PLATFORM_ANDROID )  || defined( APP_PLATFORM_LINUX )
#include <pthread.h>
#include<signal.h>
#endif //APP_PLATFORM_WINDOWS


#if defined(APP_PLATFORM_WINDOWS)
namespace irr {


} // namespace irr
#elif defined( APP_PLATFORM_ANDROID )  || defined( APP_PLATFORM_LINUX )
namespace irr {


} // namespace irr
#endif //( APP_PLATFORM_ANDROID )  || ( APP_PLATFORM_LINUX )