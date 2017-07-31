
#include "MApplication.h"
#include "MWindow.h"

class myapp : public MApplication
{
	public:	myapp(char *);
			virtual void MessageReceived(BMessage*);
};

class mywin : public MWindow
{
	public:	mywin(BRect, char*,window_type,ulong);
			mywin(BRect, char*,window_look, window_feel,ulong);
			virtual bool	QuitRequested();
			virtual void MessageReceived(BMessage*);
};