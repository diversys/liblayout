
/*

	Simple demo for liblayout.
	
	This demo shows:
	- how to use groups, including nesting
	- different methods of using MButtons:
		- provide only ID: button will send M_BUTTON_SELECTED message,
		  with the ID as one of the members
		- provide message instead of ID: button will use the message
		- provide message and target: button will send message to target
		- specify size. In this case, the sizes are all set -1, which will
		  make the button assume its minimum size at all times.

	Run the "demo1" application from a Terminal-window to see the
	different messages that the buttons transmit.
	Note how you can set the target of a button in its constructor,
	so you can easily have buttons send messages to some other object
	than the window.
	Also note how the messages sent include information about
	doubleclicking.

	This source is public domain.
*/

#include "ld_appwin.h"
#include "VGroup.h"
#include "HGroup.h"
#include "MButton.h"

int main()
{
	MView *topview;
	MButton *button;
	mywin *window;
	myapp app("application/x-marcone-layoutdemo1");

	window=new mywin(BRect(10,20,310,120),"Demo 1",B_TITLED_WINDOW,0);

	topview=new VGroup
			(
				new HGroup
				(
					new MButton("upper left",'ID-1'),
					new MButton("upper right",new BMessage('ID-2')),
					0
				),
				new MButton("middle row",new BMessage('ID-3'),be_app),
				button=new MButton("bottom row",'ID-4',minimax(-1,-1,-1,-1)),
				0
			);
	// Do something special with the last button:
	button->flags|=(M_REPORT_REPEAT|M_REPORT_IMMEDIATE);

	// because topview is an MView, we need to cast it to a BView.
	window->AddChild(dynamic_cast<BView*>(topview));
	// the MWindow that our "mywin" really is will take care of everything.
	window->Show();

	app.Run();
	return(0);
}
