/*
	Common source for liblayout-demos
	This file defines the basic window and application classes
	that any application with a GUI requires.
	
	This source is public domain. Customize at will.
*/

#include <stdio.h>
#include "ld_appwin.h"

// The MApplication receives font-change notifications, and sends them on
// to its windows.
myapp::myapp(char *id)
	: MApplication(id)
{
}

// Always call the inherited MessageReceived(), unless you don't
// want your windows to react to font-changes.
void myapp::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		// ....

		default:
			printf("received by application:\n");
			message->PrintToStream();
			printf("----------\n");
			MApplication::MessageReceived(message);
			break;
	}
}


// The MWindow receives font-change messages from the application.
// It also takes care of color-drops. 
mywin::mywin(BRect r,char *name,window_type type,ulong mode)
	: MWindow(r,name,type,mode)
{
}

mywin::mywin(BRect r,char *name,window_look look, window_feel feel,ulong mode)
	: MWindow(r,name,look,feel,mode)
{
}

bool mywin::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return TRUE;
}

// Always call the inherited MessageReceived(), unless you don't
// want your windows to react to font-changes and color-drops.
void mywin::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		// ....

		default:
			printf("received by window:\n");
			message->PrintToStream();
			printf("----------\n");
			MWindow::MessageReceived(message);
			break;
	}
}
