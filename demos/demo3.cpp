
/*

	Somewhat more elaborate demo for liblayout.
	
	This demo shows:
	- how to make your own layoutable class
	- that MWindow is *really* just a BWindow, so you can use
	  all the BWindow types and flags you want.
	- examples of some more classes
	- how to make a button send its message directly to another looper

	This source is public domain.
*/


// try what happens when you #undef this:
#define MODE1

#include "ld_appwin.h"
#include "VGroup.h"
#include "HGroup.h"
#include "TabGroup.h"
#include "MRadioGroup.h"
#include "MTextView.h"
#include "MButton.h"
#include "Space.h"
#include "MBorder.h"
#include "MDragBar.h"

// First, let's define our own layoutable class.
// To participate in the system, you need to derive
// (directly or indirectly) from both MView and BView,
// taking care to derive from MView first:
class Leaf :  public MView, public BView
{
	public:		Leaf(minimax size=0);
				virtual	minimax	layoutprefs();
				virtual BRect	layout(BRect rect);
				virtual void	Draw(BRect rect);
};

Leaf::Leaf(minimax size)
		: BView(BRect(0,0,1,1),"", B_WILL_DRAW,0)
{
	// ct_mpm is the size at construction. Layout will always take
	// place based on this value, so make sure it is initialized.
	ct_mpm=size;
};

// A minimal layoutprefs()
// It simply returns the minimax you gave it.
minimax Leaf::layoutprefs()
{
	return mpm=ct_mpm;
}

// Basic resizing. The top-level classes guarantee that your
// minimum and maximum values are respected, so you need not
// concern yourself with that.
BRect Leaf::layout(BRect rect)
{
	ResizeTo(rect.Width(),rect.Height());
	MoveTo(rect.LeftTop());
	Draw(rect);
	return rect;
}

// just a simple draw-function, so there's something to look at.
void Leaf::Draw(BRect)
{
	BRect r;

	r=Bounds();
	FillRect(r,B_SOLID_LOW);
	StrokeLine(BPoint(r.left,r.bottom),BPoint(r.right,r.top),B_SOLID_HIGH);
	StrokeLine(BPoint(r.left,r.top),BPoint(r.right,r.bottom),B_SOLID_HIGH);
	StrokeRect(r);
}

//
// main program
//
int main()
{
	MView *topview;
	mywin *window;
	MTextView *tv;
	MRadioGroup *rad;
	myapp app("application/x-marcone-layoutdemo3");

	window=new mywin(BRect(10,20,310,120),NULL,B_MODAL_WINDOW_LOOK,B_NORMAL_WINDOW_FEEL,B_NOT_ZOOMABLE|B_NOT_CLOSABLE);

	topview=new VGroup
			(
				new MDragBar(minimax(0,20,10000,20)),
				new MBorder
				(
					M_RAISED_BORDER,3,NULL,
					new TabGroup
					(
						"tabs",
						new HGroup
						(
							new Leaf(),
#ifdef MODE1
							new VGroup
							(
								new Space(),
								new Leaf(minimax(50,50,100,100)),
								new Space(),
								0
							),
#else
							new Leaf(minimax(50,50,100,100)),
#endif
							new VGroup
							(
								new Leaf(),
								new Leaf(),
								0
							),
							0
						),
						"can be",
						new MBorder
						(
							M_LABELED_BORDER,4,"The OS that rules",
							rad=new MRadioGroup("BeOS","AmigaOS","Linux","Wintendo",0)
						),
						"useful",
						new VGroup
						(
							tv=new MTextView(),
							new MButton("Quit",new BMessage(B_QUIT_REQUESTED),be_app,minimax(-1,-1,-1,-1)),
							0
						),
						0
					)
				),
				0
			);

	tv->MakeEditable(false);
	tv->SetText("Don't you agree?\n\nYou can also drop colors on this window."
				"Try it, it's really neat!");

	// because topview is an MView, we need to cast it to a BView.
	window->AddChild(dynamic_cast<BView*>(topview));
	// the MWindow that our "mywin" really is will take care of everything.
	window->Show();

	app.Run();
	return(0);
}
