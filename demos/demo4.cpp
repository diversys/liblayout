
/*

	Demo containing almost all layout-classes.
	
	This source is public domain.
*/


#include "ld_appwin.h"
#include "VGroup.h"
#include "HGroup.h"
#include "TabGroup.h"
#include "LayeredGroup.h"
#include "MRadioGroup.h"
#include "MTextView.h"
#include "MOutlineListView.h"
#include "MTextControl.h"
#include "MSplitter.h"
#include "MRew.h"
#include "MPlayBW.h"
#include "MStop.h"
#include "MPlayFW.h"
#include "MFFWD.h"
#include "MVolume.h"
#include "MEject.h"
#include "MButton.h"
#include "MCheckBox.h"
#include "Space.h"
#include "MPopup.h"
#include "MBorder.h"
#include "MDragBar.h"
#include "MProgressBar.h"
#include "PropGadget.h"
#include "SliderBits.h"
#include "MScrollView.h"
#include "SpinButton.h"

// The Leaf class from demo3
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
	ct_mpm=size;
};

minimax Leaf::layoutprefs()
{
	return mpm=ct_mpm;
}

BRect Leaf::layout(BRect rect)
{
	ResizeTo(rect.Width(),rect.Height());
	MoveTo(rect.LeftTop());
	Draw(rect);
	return rect;
}

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
	MProgressBar *stat1;
	MProgressBar *stat2;
	MOutlineListView *outlineview;
	BBitmap *myknob;

	myapp app("application/x-marcone-layoutdemo4");

	myknob = new BBitmap(BRect(0, 0, KNOB_WIDTH-1, KNOB_HEIGHT-1), B_COLOR_8_BIT);
	myknob->SetBits((char*)knob, myknob->BitsLength(), 0, B_COLOR_8_BIT);

	window=new mywin(BRect(10,20,310,120),"Demo 4",B_TITLED_WINDOW,0);


	LayeredGroup *layers=
					new LayeredGroup
					(
						new VGroup
						(
							new HGroup(
								new MButton("MViews"),
								new MButton("are quite"),
								new MButton("flexible"),
								0),
							new HGroup(
								new MButton("biggest and smallest"),
								new MSplitter(),
								new MButton("objects"),
								0),
							new HGroup(
								new MButton("are recognized and set"),
								new MButton("fully automatically"),
								0),
							new HGroup(
								new MButton("and it",0,minimax(-1,-1,1000,1000,2)),
								new MButton("does",0,minimax(-1,-1,1000,1000,3)),
								new MButton("weights",0,minimax(-1,-1,1000,1000,4)),
								new MButton("too",0,minimax(-1,-1,1000,1000,5)),
								0),
							new HGroup(
								new MRew((BHandler*)NULL),
								new MPlayBW((BHandler*)NULL),
								new MStop((BHandler*)NULL),
								new MPlayFW((BHandler*)NULL),
								new MFFWD((BHandler*)NULL),
								new Space(),
								new MVolume((BHandler*)NULL),
								new Space(),
								new MEject((BHandler*)NULL),
								0),
							0
						),
						new VGroup
						(
							new HGroup
							(
								tv=new MTextView(minimax(100,50)),
								new MSplitter(),
								new VGroup
								(
									new MPopup(NULL,"label-less","MPopups", "have been","fixed", "at last!",0),
//									new MPopup("lookahere","short","very, very long item",0),
									new Space(),
									new MTextControl("enter your name:","Willy Wonka"),
									new SpinButton("Enter your age",SPIN_INTEGER),
									0
								),
								0
							),
							stat1=new MProgressBar(window),
							new MSplitter(),
							stat2=new MProgressBar(window),
							0
						),
						new VGroup
						(
							new HGroup
							(
								new VGroup
								(
									new MCheckBox("check 1"),
									new MCheckBox("check 2",0,true),
									new MCheckBox("check 3",0,true),
									new MCheckBox("check 4"),
								0
								),
								new TabGroup
								(
									"group 1",
									new VGroup
									(
										new MBorder
										(
											M_LABELED_BORDER,11,"pick one",
											new MRadioGroup
											(
												"pick me!",
												"no me!",
												"no way, I was first!",
												0
											)
										),
										new MCheckBox("checkbox"),
										new Leaf(),
										0
									),
									"group 2",
									new VGroup
									(
										new Space(),
//										new MPopup("pick one","pick me!","no me!","no way, I was first!",0),
										new Space(),
										0
									),
									"group 3",
									new VGroup
									(
										new PropGadget(0.02,.8,0.02,.8,myknob),
										new Space(),
										0
									),
									0
								),
								0
							),
							new MDragBar(minimax(10,10,65536,10)),
							0
						),
						new HGroup
						(
							new MScrollView
							(
								outlineview=new MOutlineListView(),
								true,true
							),
							new MSplitter(),
							new Space(),
/*
							new VGroup
							(
								new HGroup
								(
									minimax(-1,-1,65535,65535,1.75),
									new Leaf(minimax(-1,-1,65535,65535,1.75)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.50)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.25)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.00)),
									0
								),
								new MSplitter(),
								new HGroup
								(
									minimax(-1,-1,65535,65535,1.50),
									new Leaf(minimax(-1,-1,65535,65535,1.75)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.50)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.25)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.00)),
									0
								),
								new MSplitter(),
								new HGroup
								(
									minimax(-1,-1,65535,65535,1.25),
									new Leaf(minimax(-1,-1,65535,65535,1.75)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.50)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.25)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.00)),
									0
								),
								new MSplitter(),
								new HGroup
								(
									minimax(-1,-1,65535,65535,1.00),
									new Leaf(minimax(-1,-1,65535,65535,1.75)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.50)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.25)),
									new MSplitter(),
									new Leaf(minimax(-1,-1,65535,65535,1.00)),
									0
								),
								0
							),
*/
							0
						),
						0
					);


	// An MLayeredGroup is like a TabGroup without tabs. You activate a layer by
	// calling the ActivateLayer() function, or by sending it a message
	// In this example, I use a popup to directly control the group
	BMessage message(M_ACTIVATE_LAYER);
	MPopup *popup=new MPopup("category",&message,layers,"buttons","other stuff","more stuff", "yet more",0);
	popup->flags|=M_NO_Y_LEFTOVERS;

	topview=	new MBorder
				(
					M_RAISED_BORDER,3,"outer",
					new VGroup
					(
						popup,
						layers,
						0
					)
				);

	tv->SetText("Try dropping colors on this window or on the statusbars below. Use roColour or Beatware Painter for this.");
	BListItem *litem;
	outlineview->AddItem(litem=new BStringItem("MOutlineListView"));
	outlineview->AddUnder(new BStringItem("ofcourse)"),litem);
	outlineview->AddUnder(new BStringItem("(with subitems"),litem);
	outlineview->AddItem(new BStringItem("================="));
	outlineview->AddItem(new BStringItem("MOutlineListView,"));
	outlineview->AddItem(new BStringItem("MListView and MTextView"));
	outlineview->AddItem(new BStringItem("can be placed in an"));
	outlineview->AddItem(new BStringItem("MScrollView to allow for"));
	outlineview->AddItem(new BStringItem("horizontal and/or vertical"));
	outlineview->AddItem(new BStringItem("scrolling."));
	
	stat1->value=0.4;
	stat1->ct_mpm.weight=1.4;
	stat2->value=0.3;
	stat2->ct_mpm.weight=0.6;

	// because topview is an MView, we need to cast it to a BView.
	window->AddChild(dynamic_cast<BView*>(topview));
	// the MWindow that our "mywin" really is will take care of everything.
	window->Show();

	app.Run();
	return(0);
}
