
/*

	Demo showing how to align dividable classes.
	
	This source is public domain.
*/


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
main()
{
	MView *topview;
	mywin *window;

	myapp app("application/x-marcone-layoutdemo5");

	window=new mywin(BRect(10,20,310,120),"Demo 5",B_TITLED_WINDOW,B_NOT_RESIZABLE);

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
							tv2=new MStringView("465666",B_ALIGN_CENTER),
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
	
	// call the global DivideSame() function
	// this function will set up the named MView/MDividables hybrids
	// so they are nicely aligned.
	DivideSame(tc1,tc2,tc3,tc4,0);


	but->MakeDefault(true);
	// because topview is an MView, we need to cast it to a BView.
	window->AddChild(dynamic_cast<BView*>(topview));
	// the MWindow that our "mywin" really is will take care of everything.
	window->Show();

	app.Run();
	return(0);
}
