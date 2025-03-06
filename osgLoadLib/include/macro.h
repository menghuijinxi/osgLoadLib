#ifdef _WIN32
#ifdef OSGBLoadLib_EXPORTS
#define OSGBLoadLib_API __declspec(dllexport)
#else
#define OSGBLoadLib_API __declspec(dllimport)
#endif
#else
#define OSGBLoadLib_API
#endif