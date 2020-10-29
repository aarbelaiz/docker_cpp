#ifdef _WIN32
	#ifdef docker_EXPORTS
		#define DOCKER_CPP_API __declspec( dllexport )
	#else
		#define DOCKER_CPP_API __declspec( dllimport )
	#endif
#else
	#define DOCKER_CPP_API
#endif
