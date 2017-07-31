
#include "layout.h"

#include <Entry.h>
#include <Path.h>

class IMPEXPLIBLAYOUT MUtils
{
	public:
		static void			GetAppPath(BPath *path);
		static entry_ref*	AppRef();
		static const char*	AppSig();
		static int32		NumCPUs();
		static void*		Function(const char *func, const char *lib);
};
