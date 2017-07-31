
/*

	Somewhat more elaborate demo for liblayout.
	
	This demo shows:
	- how to do more complex layouts, using spacers where
	  appropriate
	- examples of some classes other than buttons
	- how to add additional tabs to a TabGroup, after it has
	  been constructed

	This source is public domain.
*/

#include <MessageFilter.h>

#include "ld_appwin.h"
#include "VGroup.h"
#include "HGroup.h"
#include "MButton.h"
#include "MCheckBox.h"
#include "Space.h"
#include "MBorder.h"
#include "TabGroup.h"
#include <stdio.h>
#include <string.h>


TabGroup *tbgrp;
int num=0;
char buf[100];

filter_result Filter(BMessage *message, BHandler **target,BMessageFilter *messageFilter);
filter_result Filter(BMessage *message, BHandler **target,BMessageFilter *messageFilter)
{

	if(message->what=='tab!')
	{
		// create the content for the new tab
		Space *space=new Space();
		// give it a name
		space->SetName("more");
		// add it to the tabgroup
		sprintf(buf,"%d",num++);
		MButton *but=new MButton(strdup(buf));
		tbgrp->AddChild(but);
		// tell the window to update itself
	    BLooper *looper=messageFilter->Looper();
	    looper->PostMessage(M_RECALCULATE_SIZE);
		return B_SKIP_MESSAGE;
	}
	return B_DISPATCH_MESSAGE;
}


int main()
{
	MView *topview;
	mywin *window;
	myapp app("application/x-marcone-layoutdemo2");

	window=new mywin(BRect(10,20,310,120),"Demo 2",B_TITLED_WINDOW,0);

	topview=new HGroup
			(
				new VGroup
				(
					new MBorder
					(
						M_LABELED_BORDER,3,"Click here",
						new MButton("Quit",new BMessage(B_QUIT_REQUESTED),be_app)
					),
					new Space(),
					new HGroup
					(
						new MButton("button 1",0,minimax(-1,-1,-1,-1)),
						new MButton("button 2",0,minimax(-1,-1,-1,-1)),
						0
					),
					0
				),
				new Space(minimax(20,0,20,0)),
				new VGroup
				(
					tbgrp=new TabGroup
					(
						"tab 1",
						new MButton("1",0,minimax(-1,-1,-1,-1)),
						"tab 2",
						new Space(),
						"tab 3",
						new Space(),
						0
					),
					new MButton("add tab",new BMessage('tab!')),
					0
				),
				0
			);


	// because topview is an MView, we need to cast it to a BView.
	window->AddChild(dynamic_cast<BView*>(topview));
	// the MWindow that our "mywin" really is will take care of everything.
	window->AddCommonFilter(new BMessageFilter(B_ANY_DELIVERY,B_ANY_SOURCE,Filter));
	window->Show();

	app.Run();
	return(0);
}
