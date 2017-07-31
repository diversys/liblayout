
/*
	This demo shows how to
	- use MViews with standard BWindows
	- mix MViews with standard BViews

	This source is public domain.
*/

#include <ScrollBar.h>

#include "ld_appwin.h"
#include "VGroup.h"
#include "HGroup.h"
#include "MTextControl.h"
#include "MButton.h"
#include "MStringView.h"
#include "Space.h"
#include "MBorder.h"

MStringView *tv1;
MStringView *tv2;
MStringView *tv3;
MTextControl *tc1;
MTextControl *tc2;
MTextControl *tc3;
MTextControl *tc4;
MButton *but;

//
// main program
//
int main()
{
	MView *topview;
	BWindow *window;

	myapp app("application/x-marcone-layoutdemo6");

	window=new BWindow(BRect(10,20,410,220),"Demo 6",B_DOCUMENT_WINDOW,0);

	// use the controls from demo5
	topview=new MBorder
			(
				M_RAISED_BORDER,3,"",
				new HGroup
				(
					new MBorder
					(
						M_LABELED_BORDER,10,"System Information",
						new VGroup
						(
							tc1=new MTextControl("Platform","Mac"),
							tc2=new MTextControl("CPU type","604e"),
							tc3=new MTextControl("CPU speed","180MHz x2"),
							tc4=new MTextControl("Bus speed","45 MHz"),
							0
						)
					),
					new MBorder
					(
						M_LABELED_BORDER,10,"Dhrystones",
						new VGroup
						(
							tv1=new MStringView("Your computer benchmarks at:",B_ALIGN_LEFT),
							tv2=new MStringView("1950",B_ALIGN_CENTER),
							tv3=new MStringView("Dhrystones/second",B_ALIGN_CENTER),
							but=new MButton("Calculate",'calc',minimax(-1,-1,-1,-1)),
							0
						)
					),
					0
				)
			);

	BFont font(be_plain_font);
	font.SetSize(18);
	tv2->SetFont(&font);
	DivideSame(tc1,tc2,tc3,tc4,0);
	but->MakeDefault(true);
//===================================
// be sure to try both versions.
//===================================
#if 0
	// add the MViews
	BView *bview=dynamic_cast<BView*>(topview);
	window->AddChild(bview);

	// now add a couple of plain BViews as well
	window->AddChild(new BScrollBar(BRect(0,200-B_H_SCROLL_BAR_HEIGHT,400-B_V_SCROLL_BAR_WIDTH,200),
								"", NULL,0,100, B_HORIZONTAL));
	window->AddChild(new BScrollBar(BRect(400-B_V_SCROLL_BAR_WIDTH,0,400,200-B_H_SCROLL_BAR_HEIGHT),
								"", NULL,0,100, B_VERTICAL));

	// must do all these manually now!
	bview->SetResizingMode(B_FOLLOW_ALL);
	bview->SetFlags(bview->Flags()|B_FRAME_EVENTS);
	minimax limit=topview->layoutprefs();
	window->SetSizeLimits(	limit.mini.x+B_V_SCROLL_BAR_WIDTH,1E6,
							limit.mini.y+B_H_SCROLL_BAR_HEIGHT,1E6);
	BRect wb=window->Bounds();
	bview->ResizeTo(wb.Width()-B_V_SCROLL_BAR_WIDTH-1,wb.Height()-B_H_SCROLL_BAR_HEIGHT-1);
#else
	// add the MViews
	BView *bview=new BView(BRect(0,0,50,50),"",B_FOLLOW_ALL,B_WILL_DRAW);
	window->AddChild(bview);
	bview->AddChild(dynamic_cast<BView*>(topview));

	// now add a couple of plain BViews as well
	BScrollBar *hbar, *vbar;
	window->AddChild(hbar=new BScrollBar(BRect(0,200-B_H_SCROLL_BAR_HEIGHT,400-B_V_SCROLL_BAR_WIDTH,200),
								"", bview,0,100, B_HORIZONTAL));
	window->AddChild(vbar=new BScrollBar(BRect(400-B_V_SCROLL_BAR_WIDTH,0,400,200-B_H_SCROLL_BAR_HEIGHT),
								"", bview,0,100, B_VERTICAL));

	// must do all these manually now!
	minimax limit=topview->layoutprefs();
	topview->layout(BRect(0,0,2*limit.mini.x,2*limit.mini.y));
	window->SetSizeLimits(50,2*limit.mini.x+B_V_SCROLL_BAR_WIDTH,50,2*limit.mini.y+B_H_SCROLL_BAR_HEIGHT);
	window->ResizeTo(limit.mini.x+B_V_SCROLL_BAR_WIDTH,limit.mini.y+B_H_SCROLL_BAR_HEIGHT);
	BRect wb=window->Bounds();
	bview->ResizeTo(wb.Width()-B_V_SCROLL_BAR_WIDTH-1,wb.Height()-B_H_SCROLL_BAR_HEIGHT-1);

/*
	Excercise for the reader: override BWindow::FrameResized to properly adjust the range and proportion
	of the scrollbars when the window is resized.
*/
#endif

	window->Show();

	app.Run();
	return(0);
}
