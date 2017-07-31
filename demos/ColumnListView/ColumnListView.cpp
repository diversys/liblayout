
//Column list view source file

#include <ListView.h>
#include <ScrollView.h>


// adapted for liblayout

#include "layout.h"


//******************************************************************************************************
//**** PROJECT HEADER FILES
//******************************************************************************************************
#define ColumnListView_CPP
#include "ColumnListView.h"
#include "CLVColumnLabelView.h"
#include "CLVColumn.h"
#include "CLVListItem.h"
#include "Benaphore.h"


//******************************************************************************************************
//**** BITMAPS
//******************************************************************************************************
uint8 CLVRightArrowData[132] =
{
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x12, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x12, 0x12, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x12, 0x12, 0x12, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x12, 0x12, 0x12, 0x12, 0x00, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x12, 0x12, 0x12, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x12, 0x12, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x12, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
uint8 CLVDownArrowData[132] =
{
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
	0xFF, 0x00, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x00, 0xFF, 0xFF,
	0xFF, 0xFF, 0x00, 0x12, 0x12, 0x12, 0x12, 0x12, 0x00, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0x12, 0x12, 0x12, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x12, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


//******************************************************************************************************
//**** ColumnListView CLASS DEFINITION
//******************************************************************************************************
class CLVContainerView : public MView, public BScrollView
{
	public:
		CLVContainerView(char* name, BView* target, uint32 resizingMode, uint32 flags, bool horizontal,
			bool vertical, border_style border);
		~CLVContainerView();
		bool IsBeingDestroyed;
		
		// adapted for liblayout
		virtual minimax layoutprefs();
		virtual BRect layout(BRect);
		// (end of adaptation)
};


CLVContainerView::CLVContainerView(char* name, BView* target, uint32 resizingMode, uint32 flags,
	bool horizontal, bool vertical, border_style border) :
BScrollView(name,target,resizingMode,flags,horizontal,vertical,border)
{
	IsBeingDestroyed = false;
};


CLVContainerView::~CLVContainerView()
{
	IsBeingDestroyed = true;
}

// adapted for liblayout
minimax CLVContainerView::layoutprefs()
{
	return mpm=ct_mpm;
}

BRect CLVContainerView::layout(BRect rect)
{
	MoveTo(rect.LeftTop());
	ResizeTo(rect.Width(),rect.Height());
	return rect;
}
// (end of adaptation)


ColumnListView::ColumnListView(BRect Frame, BScrollView **ContainerView, const char *Name,
	uint32 ResizingMode, uint32 flags, list_view_type Type, bool hierarchical, bool horizontal,
	bool vertical, border_style border, const BFont *LabelFont)
: BListView(Frame,Name,Type,B_FOLLOW_ALL_SIDES,flags),
fColumnList(6),
fColumnDisplayList(6),
fFullItemList(32),
fRightArrow(BRect(0.0,0.0,10.0,10.0),B_COLOR_8_BIT,CLVRightArrowData,false,false),
fDownArrow(BRect(0.0,0.0,10.0,10.0),B_COLOR_8_BIT,CLVDownArrowData,false,false)
{
	fHierarchical = hierarchical;

	//Create the column titles bar view
	font_height FontAttributes;
	LabelFont->GetHeight(&FontAttributes);
	float fLabelFontHeight = ceil(FontAttributes.ascent) + ceil(FontAttributes.descent);
	float ColumnLabelViewBottom = Frame.top+1.0+fLabelFontHeight+3.0;
	fColumnLabelView = new CLVColumnLabelView(BRect(Frame.left,Frame.top,Frame.right,
		ColumnLabelViewBottom),this,LabelFont);

	//Create the container view
	CreateContainer(horizontal,vertical,border,ResizingMode,flags);
	*ContainerView = fScrollView;

	//Complete the setup
	UpdateColumnSizesDataRectSizeScrollBars();
	fColumnLabelView->UpdateDragGroups();
	fSortMode = Ascending;
	fSortKey = NULL;
	fBoolGroupMode = TrueFirst;
	fBoolGroupKey = NULL;
	fExpanderColumn = -1;
	fCompare = NULL;
	fIsTrue = NULL;
}


ColumnListView::~ColumnListView()
{
	//Delete all list columns
	int16 ColumnCount = fColumnList.CountItems();
	for(int16 Counter = ColumnCount-1; Counter >= 0; Counter--)
	{
		CLVColumn* Item = (CLVColumn*)fColumnList.RemoveItem(Counter);
		if(Item)
			delete Item;
	}
	//Remove and delete the container view if necessary
	if(!fScrollView->IsBeingDestroyed)
	{
		fScrollView->RemoveChild(this);
		delete fScrollView;
	}
}


void ColumnListView::CreateContainer(bool horizontal, bool vertical, border_style border,
	uint32 ResizingMode, uint32 flags)
{
	BRect ViewFrame = Frame();
	BRect LabelsFrame = fColumnLabelView->Frame();

	fScrollView = new CLVContainerView(NULL,this,ResizingMode,flags,horizontal,vertical,border);
	BRect NewFrame = Frame();
	//Resize the main view to make room for the CLVColumnLabelView
	ResizeTo(ViewFrame.right-ViewFrame.left,ViewFrame.bottom-LabelsFrame.bottom-1.0);
	MoveTo(NewFrame.left,NewFrame.top+(LabelsFrame.bottom-LabelsFrame.top+1.0));
	fColumnLabelView->MoveTo(NewFrame.left,NewFrame.top);

	//Add the ColumnLabelView
	fScrollView->AddChild(fColumnLabelView);

	//Remove and re-add the BListView so that it will draw after the CLVColumnLabelView
	fScrollView->RemoveChild(this);
	fScrollView->AddChild(this);

	//If both scrollbars, needs a little filler view in the corner where the scrollbars meet
	if(horizontal && vertical)
	{
		fFillerView = new CLVFillerView(NewFrame.right+1.0,NewFrame.bottom+1.0);
		fScrollView->AddChild(fFillerView);
	}
	else
		fFillerView = NULL;
}


CLVFillerView::CLVFillerView(float Left,float Top)
: BView(BRect(Left,Top,Left+B_V_SCROLL_BAR_WIDTH,Top+B_H_SCROLL_BAR_HEIGHT),NULL,B_FOLLOW_RIGHT |
	B_FOLLOW_BOTTOM,B_WILL_DRAW)
{
	SetHighColor(BeShadow);
}


CLVFillerView::~CLVFillerView()
{ }


void CLVFillerView::Draw(BRect Update)
{
	if(Update.bottom >= B_H_SCROLL_BAR_HEIGHT)
		StrokeLine(BPoint(0.0,B_H_SCROLL_BAR_HEIGHT),BPoint(B_V_SCROLL_BAR_WIDTH,B_H_SCROLL_BAR_HEIGHT));
	if(Update.right >= B_V_SCROLL_BAR_WIDTH)
		StrokeLine(BPoint(B_V_SCROLL_BAR_WIDTH,0.0),
			BPoint(B_V_SCROLL_BAR_WIDTH,B_H_SCROLL_BAR_HEIGHT-1.0));
}


void ColumnListView::UpdateColumnSizesDataRectSizeScrollBars()
{
	//Figure out the width
	float ColumnBegin;
	float ColumnEnd = -1.0;
	fDataWidth = 0.0;
	bool NextPushedByExpander = false;
	int16 NumberOfColumns = fColumnDisplayList.CountItems();
	for(int16 Counter = 0; Counter < NumberOfColumns; Counter++)
	{
		CLVColumn* Column = (CLVColumn*)fColumnDisplayList.ItemAt(Counter);
		if(NextPushedByExpander)
			Column->fPushedByExpander = true;
		else
			Column->fPushedByExpander = false;
		if(Column->IsShown())
		{
			float ColumnWidth = Column->Width();
			ColumnBegin = ColumnEnd + 1.0;
			ColumnEnd = ColumnBegin + ColumnWidth;
			Column->fColumnBegin = ColumnBegin;
			Column->fColumnEnd = ColumnEnd;
			fDataWidth = Column->fColumnEnd;
			if(NextPushedByExpander)
				if(!(Column->fFlags & CLV_PUSH_PASS))
					NextPushedByExpander = false;
			if(Column->fFlags & CLV_EXPANDER)
				//Set the next column to be pushed
				NextPushedByExpander = true;
		}
	}

	//Figure out the height
	fDataHeight = 0.0;
	int32 NumberOfItems = CountItems();
	for(int32 Counter2 = 0; Counter2 < NumberOfItems; Counter2++)
		fDataHeight += ItemAt(Counter2)->Height()+1.0;
	if(NumberOfItems > 0)
		fDataHeight -= 1.0;

	//Update the scroll bars
	UpdateScrollBars();
}


void ColumnListView::UpdateScrollBars()
{
	if(fScrollView)
	{
		//Figure out the bounds and scroll if necessary
		BRect ViewBounds;
		float DeltaX,DeltaY;
		do
		{
			ViewBounds = Bounds();
			//Figure out the width of the page rectangle
			fPageWidth = fDataWidth;
			fPageHeight = fDataHeight;
			//If view runs past the end, make more visible at the beginning
			DeltaX = 0.0;
			if(ViewBounds.right > fDataWidth && ViewBounds.left > 0)
			{
				DeltaX = ViewBounds.right-fDataWidth;
				if(DeltaX > ViewBounds.left)
					DeltaX = ViewBounds.left;
			}
			DeltaY = 0.0;
			if(ViewBounds.bottom > fDataHeight && ViewBounds.top > 0)
			{
				DeltaY = ViewBounds.bottom-fDataHeight;
				if(DeltaY > ViewBounds.top)
					DeltaY = ViewBounds.top;
			}
			if(DeltaX != 0.0 || DeltaY != 0.0)
			{
				ScrollTo(BPoint(ViewBounds.left-DeltaX,ViewBounds.top-DeltaY));
				ViewBounds = Bounds();
			}
			if(ViewBounds.right-ViewBounds.left > fDataWidth)
				fPageWidth = ViewBounds.right;
			if(ViewBounds.bottom-ViewBounds.top > fDataHeight)
				fPageHeight = ViewBounds.bottom;
		}while(DeltaX != 0.0 || DeltaY != 0.0);
	
		//Figure out the ratio of the bounds rectangle width or height to the page rectangle width or height
		float WidthProp = (ViewBounds.right-ViewBounds.left)/fPageWidth;
		float HeightProp = (ViewBounds.bottom-ViewBounds.top)/fPageHeight;

		BScrollBar* HScrollBar = fScrollView->ScrollBar(B_HORIZONTAL);
		BScrollBar* VScrollBar = fScrollView->ScrollBar(B_VERTICAL);
		//Set the scroll bar ranges and proportions.  If the whole document is visible, inactivate the
		//slider
		if(HScrollBar)
		{
			if(WidthProp >= 1.0 && ViewBounds.left == 0.0)
				HScrollBar->SetRange(0.0,0.0);
			else
				HScrollBar->SetRange(0.0,fPageWidth-(ViewBounds.right-ViewBounds.left));
			HScrollBar->SetProportion(WidthProp);
			//Set the step values
			HScrollBar->SetSteps(20.0,ViewBounds.right-ViewBounds.left);
		}
		if(VScrollBar)
		{
			if(HeightProp >= 1.0 && ViewBounds.top == 0.0)
			{
				VScrollBar->SetRange(0.0,0.0);
				if(fFillerView)
					fFillerView->SetViewColor(BeInactiveControlGrey);
			}
			else
			{
				VScrollBar->SetRange(0.0,fPageHeight-(ViewBounds.bottom-ViewBounds.top));
				if(fFillerView)
					fFillerView->SetViewColor(BeBackgroundGrey);
			}
			VScrollBar->SetProportion(HeightProp);
		}
	}
}


void ColumnListView::ColumnsChanged()
{
	//Any previous column dragging/resizing will get corrupted, so deselect
	if(fColumnLabelView->fColumnClicked)
		fColumnLabelView->fColumnClicked = NULL;

	//Update the internal sizes and grouping of the columns and sizes of drag groups
	UpdateColumnSizesDataRectSizeScrollBars();
	fColumnLabelView->UpdateDragGroups();
	fColumnLabelView->Invalidate();
	Invalidate();
}


bool ColumnListView::AddColumn(CLVColumn* Column)
//Adds a column to the ColumnListView at the end of the list.  Returns true if successful.
{
	int16 NumberOfColumns = fColumnList.CountItems();
	int16 DisplayIndex = NumberOfColumns;

	//Make sure a second Expander is not being added
	if(Column->fFlags & CLV_EXPANDER)
	{
		if(!fHierarchical)
			return false;
		for(int16 Counter = 0; Counter < NumberOfColumns; Counter++)
			if(((CLVColumn*)fColumnList.ItemAt(Counter))->fFlags & CLV_EXPANDER)
				return false;
		if(Column->IsShown())
			fExpanderColumn = NumberOfColumns;
	}

	//Make sure this column hasn't already been added to another ColumnListView
	if(Column->fParent != NULL)
		return false;

	BWindow* ParentWindow = Window();
	if(ParentWindow)
		ParentWindow->Lock();
	//Check if this should be locked at the beginning or end, and adjust its position if necessary
	if(!Column->Flags() & CLV_LOCK_AT_END)
	{
		bool Repeat;
		if(Column->Flags() & CLV_LOCK_AT_BEGINNING)
		{
			//Move it to the beginning, after the last CLV_LOCK_AT_BEGINNING item
			DisplayIndex = 0;
			Repeat = true;
			while(Repeat && DisplayIndex < NumberOfColumns)
			{
				Repeat = false;
				CLVColumn* LastColumn = (CLVColumn*)fColumnDisplayList.ItemAt(DisplayIndex);
				if(LastColumn->Flags() & CLV_LOCK_AT_BEGINNING)
				{
					DisplayIndex++;
					Repeat = true;
				}
			}
		}
		else
		{
			//Make sure it isn't after a CLV_LOCK_AT_END item
			Repeat = true;
			while(Repeat && DisplayIndex > 0)
			{
				Repeat = false;
				CLVColumn* LastColumn = (CLVColumn*)fColumnDisplayList.ItemAt(DisplayIndex-1);
				if(LastColumn->Flags() & CLV_LOCK_AT_END)
				{
					DisplayIndex--;
					Repeat = true;
				}
			}
		}
	}

	//Add the column to the display list in the appropriate position
	fColumnDisplayList.AddItem(Column, DisplayIndex);

	//Add the column to the end of the column list
	fColumnList.AddItem(Column);

	//Tell the column it belongs to me now
	Column->fParent = this;

	//Set the scroll bars and tell views to update
	ColumnsChanged();
	if(ParentWindow)
		ParentWindow->Unlock();
	return true;
}


bool ColumnListView::AddColumnList(BList* NewColumns)
//Adds a BList of CLVColumn's to the ColumnListView at the position specified, or at the end of the list
//if AtIndex == -1.  Returns true if successful.
{
	int16 NumberOfColumns = int16(fColumnList.CountItems());
	int16 NumberOfColumnsToAdd = int16(NewColumns->CountItems());

	//Make sure a second CLVExpander is not being added
	int16 Counter;
	int16 NumberOfExpanders = 0;
	for(Counter = 0; Counter < NumberOfColumns; Counter++)
		if(((CLVColumn*)fColumnList.ItemAt(Counter))->fFlags & CLV_EXPANDER)
			NumberOfExpanders++;
	int16 SetfExpanderColumnTo = -1;
	for(Counter = 0; Counter < NumberOfColumnsToAdd; Counter++)
	{
		CLVColumn* ThisColumn = (CLVColumn*)NewColumns->ItemAt(Counter);
		if(ThisColumn->fFlags & CLV_EXPANDER)
		{
			NumberOfExpanders++;
			if(ThisColumn->IsShown())
				SetfExpanderColumnTo = NumberOfColumns + Counter;
		}
	}
	if(NumberOfExpanders != 0 && !fHierarchical)
		return false;
	if(NumberOfExpanders > 1)
		return false;
	if(SetfExpanderColumnTo != -1)
		fExpanderColumn = SetfExpanderColumnTo;

	//Make sure none of these columns have already been added to a ColumnListView
	for(Counter = 0; Counter < NumberOfColumnsToAdd; Counter++)
		if(((CLVColumn*)NewColumns->ItemAt(Counter))->fParent != NULL)
			return false;
	//Make sure none of these columns are being added twice
	for(Counter = 0; Counter < NumberOfColumnsToAdd-1; Counter++)
		for(int16 Counter2 = Counter+1; Counter2 < NumberOfColumnsToAdd; Counter2++)
			if(NewColumns->ItemAt(Counter) == NewColumns->ItemAt(Counter2))
				return false;

	BWindow* ParentWindow = Window();
	if(ParentWindow)
		ParentWindow->Lock();
	for(Counter = 0; Counter < NumberOfColumnsToAdd; Counter++)
	{
		CLVColumn* Column = (CLVColumn*)NewColumns->ItemAt(Counter);
		//Check if this should be locked at the beginning or end, and adjust its position if necessary
		int16 DisplayIndex = NumberOfColumns;
		if(!Column->Flags() & CLV_LOCK_AT_END)
		{
			bool Repeat;
			if(Column->Flags() & CLV_LOCK_AT_BEGINNING)
			{
				//Move it to the beginning, after the last CLV_LOCK_AT_BEGINNING item
				DisplayIndex = 0;
				Repeat = true;
				while(Repeat && DisplayIndex < NumberOfColumns)
				{
					Repeat = false;
					CLVColumn* LastColumn = (CLVColumn*)fColumnDisplayList.ItemAt(DisplayIndex);
					if(LastColumn->Flags() & CLV_LOCK_AT_BEGINNING)
					{
						DisplayIndex++;
						Repeat = true;
					}
				}
			}
			else
			{
				//Make sure it isn't after a CLV_LOCK_AT_END item
				Repeat = true;
				while(Repeat && DisplayIndex > 0)
				{
					Repeat = false;
					CLVColumn* LastColumn = (CLVColumn*)fColumnDisplayList.ItemAt(DisplayIndex-1);
					if(LastColumn->Flags() & CLV_LOCK_AT_END)
					{
						DisplayIndex--;
						Repeat = true;
					}
				}
			}
		}

		//Add the column to the display list in the appropriate position
		fColumnDisplayList.AddItem(Column, DisplayIndex);

		//Tell the column it belongs to me now
		Column->fParent = this;

		NumberOfColumns++;
	}

	//Add the columns to the end of the column list
	fColumnList.AddList(NewColumns);

	//Set the scroll bars and tell views to update
	ColumnsChanged();
	if(ParentWindow)
		ParentWindow->Unlock();
	return true;
}


bool ColumnListView::RemoveColumn(CLVColumn* Column)
//Removes a CLVColumn from the ColumnListView.  Returns true if successful.
{
	int16 ColumnIndex = fColumnList.IndexOf(Column);
	if(ColumnIndex < 0)
		return false;
	if(fSortKey == Column)
		fSortKey = NULL;
	if(fBoolGroupKey == Column)
		fBoolGroupKey = NULL;
	if(Column->fFlags & CLV_EXPANDER)
		fExpanderColumn = -1;

	BWindow* ParentWindow = Window();
	if(ParentWindow)
		ParentWindow->Lock();
	//Remove Column from the column and display lists
	fColumnDisplayList.RemoveItem(Column);
	fColumnList.RemoveItem(Column);

	//Tell the column it has been removed
	Column->fParent = NULL;

	//Set the scroll bars and tell views to update
	ColumnsChanged();
	if(ParentWindow)
		ParentWindow->Unlock();
	return true;
}


bool ColumnListView::RemoveColumns(CLVColumn* Column, int16 Count)
//Finds Column in ColumnList and removes Count columns and their data from the view and its items
{
	BWindow* ParentWindow = Window();
	if(ParentWindow)
		ParentWindow->Lock();
	int16 ColumnIndex = fColumnList.IndexOf(Column);
	if(ColumnIndex < 0)
	{
		if(ParentWindow)
			ParentWindow->Unlock();
		return false;
	}
	if(ColumnIndex + Count >= fColumnList.CountItems())
	{
		if(ParentWindow)
			ParentWindow->Unlock();
		return false;
	}

	//Remove columns from the column and display lists
	for(int16 Counter = ColumnIndex; Counter < ColumnIndex+Count; Counter++)
	{
		CLVColumn* ThisColumn = (CLVColumn*)fColumnList.ItemAt(Counter);
		fColumnDisplayList.RemoveItem(ThisColumn);
		if(fSortKey == ThisColumn)
			fSortKey = NULL;
		if(fBoolGroupKey == ThisColumn)
			fBoolGroupKey = NULL;
		if(ThisColumn->fFlags & CLV_EXPANDER)
			fExpanderColumn = -1;

		//Tell the column it has been removed
		ThisColumn->fParent = NULL;
	}
	fColumnList.RemoveItems(ColumnIndex,Count);

	//Set the scroll bars and tell views to update
	ColumnsChanged();
	if(ParentWindow)
		ParentWindow->Unlock();
	return true;
}


int16 ColumnListView::CountColumns() const
{
	return fColumnList.CountItems();
}


int16 ColumnListView::IndexOfColumn(CLVColumn* column) const
{
	return fColumnList.IndexOf(column);
}


CLVColumn* ColumnListView::ColumnAt(int16 column_index) const
{
	return (CLVColumn*)fColumnList.ItemAt(column_index);
}


bool ColumnListView::SetDisplayOrder(const int16* ColumnOrder)
//Sets the display order using a BList of CLVColumn's
{
	BWindow* ParentWindow = Window();
	if(ParentWindow)
		ParentWindow->Lock();
	//Add the items to the display list in order
	fColumnDisplayList.MakeEmpty();
	int16 ColumnsToSet = fColumnList.CountItems();
	for(int16 Counter = 0; Counter < ColumnsToSet; Counter++)
	{
		if(ColumnOrder[Counter] >= ColumnsToSet)
		{
			if(ParentWindow)
				ParentWindow->Unlock();
			return false;
		}
		for(int16 Counter2 = 0; Counter2 < Counter; Counter2++)
			if(ColumnOrder[Counter] == ColumnOrder[Counter2])
			{
				if(ParentWindow)
					ParentWindow->Unlock();
				return false;
			}
		fColumnDisplayList.AddItem(fColumnList.ItemAt(ColumnOrder[Counter]));
	}

	//Update everything about the columns
	ColumnsChanged();

	//Let the program know that the display order changed.
	if(ParentWindow)
		ParentWindow->Unlock();
	DisplayOrderChanged(ColumnOrder);
	return true;
}


void ColumnListView::ColumnWidthChanged(int16 ColumnIndex, float NewWidth)
{ }


void ColumnListView::DisplayOrderChanged(const int16* order)
{ }


int16* ColumnListView::DisplayOrder() const
{
	int16 ColumnsInList = fColumnList.CountItems();
	int16* ReturnList = new int16[ColumnsInList];
	BWindow* ParentWindow = Window();
	if(ParentWindow)
		ParentWindow->Lock();
	for(int16 Counter = 0; Counter < ColumnsInList; Counter++)
		ReturnList[Counter] = int16(fColumnList.IndexOf(fColumnDisplayList.ItemAt(Counter)));
	if(ParentWindow)
		ParentWindow->Unlock();
	return ReturnList;
}


void ColumnListView::SetSortKey(int16 ColumnIndex)
{
	CLVColumn* Column;
	if(ColumnIndex >= 0)
	{
		Column = (CLVColumn*)fColumnList.ItemAt(ColumnIndex);
		if(!(Column->Flags()&CLV_SORT_KEYABLE))
			return;
	}
	else
		Column = NULL;
	if(fSortKey!=Column)
	{
		BWindow* ParentWindow = Window();
		if(ParentWindow)
			ParentWindow->Lock();
		BRect LabelBounds = fColumnLabelView->Bounds();
		if(fSortMode != NoSort && fSortKey)
			//Need to erase old underline
			fColumnLabelView->Invalidate(BRect(fSortKey->fColumnBegin,LabelBounds.top,fSortKey->fColumnEnd,
				LabelBounds.bottom));
		fSortKey = Column;
		if(fSortKey)
			SortItems();
		//Need to draw new underline
		if(fSortKey)
			fColumnLabelView->Invalidate(BRect(fSortKey->fColumnBegin,LabelBounds.top,fSortKey->fColumnEnd,
				LabelBounds.bottom));
		if(ParentWindow)
			ParentWindow->Unlock();
	}
}


void ColumnListView::SetSortMode(CLVSortMode Mode)
{
	if(fSortMode != Mode)
	{
		BWindow* ParentWindow = Window();
		if(ParentWindow)
			ParentWindow->Lock();
		BRect LabelBounds = fColumnLabelView->Bounds();
		fSortMode = Mode;
		if(fSortMode != NoSort)
			SortItems();
		//Need to draw or erase underline
		if(fSortKey)
			fColumnLabelView->Invalidate(BRect(fSortKey->fColumnBegin,LabelBounds.top,fSortKey->fColumnEnd,
				LabelBounds.bottom));
		if(ParentWindow)
			ParentWindow->Unlock();
	}
}


void ColumnListView::ReverseSortMode()
{
	if(fSortMode == Ascending)
		SetSortMode(Descending);
	else if(fSortMode == Descending)
		SetSortMode(NoSort);
	else if(fSortMode == NoSort)
		SetSortMode(Ascending);
}


CLVSortMode ColumnListView::Sorting(int16* SortKey) const
//Returns the sort mode and key.  Note: sorting will only occur if the key column is shown so that it
//will be visually obvious to the user why items can't be reordered.
{
	if(SortKey)
		*SortKey = fColumnList.IndexOf(fSortKey);
	return fSortMode;
}


void ColumnListView::SetBoolGroupKey(int16 ColumnIndex)
{
	CLVColumn* Column;
	if(ColumnIndex >= 0)
	{
		Column = (CLVColumn*)fColumnList.ItemAt(ColumnIndex);
		 if(!(Column->Flags()&CLV_BOOL_GROUPABLE))
		 	return;
	}
	else
		Column = NULL;
	if(fBoolGroupKey!=Column)
	{
		BWindow* ParentWindow = Window();
		if(ParentWindow)
			ParentWindow->Lock();
		BRect LabelBounds = fColumnLabelView->Bounds();
		if(fBoolGroupMode != NoGroup && fBoolGroupKey)
			//Need to erase old underline
			fColumnLabelView->Invalidate(BRect(fBoolGroupKey->fColumnBegin,LabelBounds.top,
				fBoolGroupKey->fColumnEnd,LabelBounds.bottom));
		fBoolGroupKey = Column;
		if(fBoolGroupKey)
			SortItems();
		//Need to erase new underline
		if(fBoolGroupKey)
			fColumnLabelView->Invalidate(BRect(fBoolGroupKey->fColumnBegin,LabelBounds.top,
				fBoolGroupKey->fColumnEnd,LabelBounds.bottom));
		if(ParentWindow)
			ParentWindow->Unlock();
	}
}


void ColumnListView::SetBoolGroupMode(CLVGroupMode Mode)
{
	if(fBoolGroupMode != Mode)
	{
		BWindow* ParentWindow = Window();
		if(ParentWindow)
			ParentWindow->Lock();
		BRect LabelBounds = fColumnLabelView->Bounds();
		fBoolGroupMode = Mode;
		if(fBoolGroupMode != NoGroup)
			SortItems();
		//Need to draw or erase underline
		if(fBoolGroupKey)
			fColumnLabelView->Invalidate(BRect(fBoolGroupKey->fColumnBegin,LabelBounds.top,
				fBoolGroupKey->fColumnEnd,LabelBounds.bottom));
		if(ParentWindow)
			ParentWindow->Unlock();
	}
}


void ColumnListView::ReverseBoolGroupMode()
{
	if(fBoolGroupMode == TrueFirst)
		SetBoolGroupMode(TrueLast);
	else if(fBoolGroupMode == TrueLast)
		SetBoolGroupMode(NoGroup);
	else if(fBoolGroupMode == NoGroup)
		SetBoolGroupMode(TrueFirst);
}


CLVGroupMode ColumnListView::BoolGrouping(int16* GroupKey) const
//Returns the boolean grouping mode and key.  Note: grouping will only occur if the key column is shown
//so that it will be visually obvious to the user why items can't be dragged to the other group.
{
	if(GroupKey)
		*GroupKey = fColumnList.IndexOf(fBoolGroupKey);
	return fBoolGroupMode;
}


void ColumnListView::FrameResized(float width, float height)
{
	UpdateColumnSizesDataRectSizeScrollBars();
	int32 NumberOfItems = CountItems();
	BFont Font;
	GetFont(&Font);
	for(uint32 Counter = 0; Counter < NumberOfItems; Counter++)
		ItemAt(Counter)->Update(this,&Font);
	BListView::FrameResized(width,height);
}


void ColumnListView::AttachedToWindow()
//Hack to work around app_server bug
{
	BListView::AttachedToWindow();
	UpdateColumnSizesDataRectSizeScrollBars();
}


void ColumnListView::ScrollTo(BPoint point)
{
	BListView::ScrollTo(point);
	fColumnLabelView->ScrollTo(BPoint(point.x,0.0));
}


void ColumnListView::MouseDown(BPoint point)
{
	int32 ItemIndex = IndexOf(point);
	if(ItemIndex >= 0)
	{
		CLVListItem* ClickedItem = (CLVListItem*)BListView::ItemAt(ItemIndex);
		if(ClickedItem->fSuperItem)
			if(ClickedItem->fExpanderButtonRect.Contains(point))
			{
				if(ClickedItem->IsExpanded())
					Collapse(ClickedItem);
				else
					Expand(ClickedItem);
				return;
			}
	}
	BListView::MouseDown(point);
}


bool ColumnListView::AddUnder(CLVListItem* item, CLVListItem* superitem)
{
	if(!fHierarchical)
		return false;

	//Find the superitem in the full list and display list (if shown)
	int32 SuperItemPos = fFullItemList.IndexOf(superitem);
	if(SuperItemPos < 0)
		return false;
	uint32 SuperItemLevel = superitem->fOutlineLevel;

	//Add the item under the superitem in the full list
	int32 ItemPos = SuperItemPos + 1;
	item->fOutlineLevel = SuperItemLevel + 1;
	while(true)
	{
		CLVListItem* Temp = (CLVListItem*)fFullItemList.ItemAt(ItemPos);
		if(Temp)
		{
			if(Temp->fOutlineLevel > SuperItemLevel)
				ItemPos++;
			else
				break;
		}
		else
			break;
	}
	return AddItemPrivate(item,ItemPos);
}


bool ColumnListView::AddItem(CLVListItem* item, int32 fullListIndex)
{
	return AddItemPrivate(item,fullListIndex);
}


bool ColumnListView::AddItem(CLVListItem* item)
{
	if(fHierarchical)
		return AddItemPrivate(item,fFullItemList.CountItems());
	else
		return AddItemPrivate(item,CountItems());
}


bool ColumnListView::AddItemPrivate(CLVListItem* item, int32 fullListIndex)
{
	if(fHierarchical)
	{
		uint32 ItemLevel = item->OutlineLevel();

		//Figure out whether it is visible (should it be added to visible list)
		bool Visible = true;

		//Find the item that contains it in the full list
		int32 SuperItemPos;
		if(ItemLevel == 0)
			SuperItemPos = -1;
		else
			SuperItemPos = fullListIndex - 1;
		CLVListItem* SuperItem;
		while(SuperItemPos >= 0)
		{
			SuperItem = (CLVListItem*)fFullItemList.ItemAt(SuperItemPos);
			if(SuperItem)
			{
				if(SuperItem->fOutlineLevel >= ItemLevel)
					SuperItemPos--;
				else
					break;
			}
			else
				return false;
		}
		if(SuperItemPos >= 0 && SuperItem)
		{
			if(!SuperItem->IsExpanded())
				//SuperItem's contents aren't visible
				Visible = false;
			if(!HasItem(SuperItem))
				//SuperItem itself isn't showing
				Visible = false;
		}

		//Add the item to the full list
		if(!fFullItemList.AddItem(item,fullListIndex))
			return false;
		else
		{
			//Add the item to the display list
			if(Visible)
			{
				//Find the previous item, or -1 if the item I'm adding will be the first one
				int32 PreviousItemPos = fullListIndex - 1;
				CLVListItem* PreviousItem;
				while(PreviousItemPos >= 0)
				{
					PreviousItem = (CLVListItem*)fFullItemList.ItemAt(PreviousItemPos);
					if(PreviousItem && HasItem(PreviousItem))
						break;
					else
						PreviousItemPos--;
				}

				//Add the item after the previous item, or first on the list
				bool Result;
				if(PreviousItemPos >= 0)
					Result = BListView::AddItem(item,IndexOf(PreviousItem)+1);
				else
					Result = BListView::AddItem(item,0);
				if(Result == false)
					fFullItemList.RemoveItem(item);
				return Result;
			}
			return true;
		}
	}
	else
		return BListView::AddItem(item,fullListIndex);
}


bool ColumnListView::AddList(BList* newItems)
{
	if(fHierarchical)
		return AddListPrivate(newItems,fFullItemList.CountItems());
	else
		return AddListPrivate(newItems,CountItems());
}


bool ColumnListView::AddList(BList* newItems, int32 fullListIndex)
{
	return AddListPrivate(newItems,fullListIndex);
}


bool ColumnListView::AddListPrivate(BList* newItems, int32 fullListIndex)
{
	int32 NumberOfItems = newItems->CountItems();
	for(int32 count = 0; count < NumberOfItems; count++)
		if(!AddItemPrivate((CLVListItem*)newItems->ItemAt(count),fullListIndex+count))
			return false;
	return true;
}


bool ColumnListView::RemoveItem(CLVListItem* item)
{
	if(item == NULL)
		return false;
	if(fHierarchical)
	{
		int32 ItemsToRemove = 1 + FullListNumberOfSubitems(item);
		return RemoveItems(fFullItemList.IndexOf(item),ItemsToRemove);
	}
	else
		return BListView::RemoveItem(item);
}


BListItem* ColumnListView::RemoveItem(int32 fullListIndex)
{
	if(fHierarchical)
	{
		CLVListItem* TheItem = (CLVListItem*)fFullItemList.ItemAt(fullListIndex);
		if(TheItem)
		{
			int32 ItemsToRemove = 1 + FullListNumberOfSubitems(TheItem);
			if(RemoveItems(fullListIndex,ItemsToRemove))
				return TheItem;
			else
				return NULL;
		}
		else
			return NULL;
	}
	else
		return BListView::RemoveItem(fullListIndex);
}


bool ColumnListView::RemoveItems(int32 fullListIndex, int32 count)
{
	CLVListItem* TheItem;
	if(fHierarchical)
	{
		uint32 LastSuperItemLevel = ULONG_MAX;
		int32 Counter;
		int32 DisplayItemsToRemove = 0;
		int32 FirstDisplayItemToRemove = -1;
		for(Counter = fullListIndex; Counter < fullListIndex+count; Counter++)
		{
			TheItem = (CLVListItem*)fFullItemList.ItemAt(Counter);
			if(TheItem->fOutlineLevel < LastSuperItemLevel)
				LastSuperItemLevel = TheItem->fOutlineLevel;
			if(BListView::HasItem(TheItem))
			{
				DisplayItemsToRemove++;
				if(FirstDisplayItemToRemove == -1)
					FirstDisplayItemToRemove = BListView::IndexOf(TheItem);
			}
		}
		while(true)
		{
			TheItem = FullListItemAt(Counter);
			if(TheItem && TheItem->fOutlineLevel > LastSuperItemLevel)
			{
				count++;
				Counter++;
				if(BListView::HasItem(TheItem))
				{
					DisplayItemsToRemove++;
					if(FirstDisplayItemToRemove == -1)
						FirstDisplayItemToRemove = BListView::IndexOf(TheItem);
				}
			}
			else
				break;
		}
		while(DisplayItemsToRemove > 0)
		{
			if(BListView::RemoveItem(FirstDisplayItemToRemove) == NULL)
				return false;
			DisplayItemsToRemove--;
		}
		return fFullItemList.RemoveItems(fullListIndex,count);
	}
	else
		return BListView::RemoveItems(fullListIndex,count);
}


CLVListItem* ColumnListView::FullListItemAt(int32 fullListIndex) const
{
	return (CLVListItem*)fFullItemList.ItemAt(fullListIndex);
}


int32 ColumnListView::FullListIndexOf(const CLVListItem* item) const
{
	return fFullItemList.IndexOf((void*)item);
}


int32 ColumnListView::FullListIndexOf(BPoint point) const
{
	int32 DisplayListIndex = IndexOf(point);
	CLVListItem* TheItem = (CLVListItem*)ItemAt(DisplayListIndex);
	if(TheItem)
		return FullListIndexOf(TheItem);
	else
		return -1;
}


CLVListItem* ColumnListView::FullListFirstItem() const
{
	return (CLVListItem*)fFullItemList.FirstItem();
}


CLVListItem* ColumnListView::FullListLastItem() const
{
	return (CLVListItem*)fFullItemList.LastItem();
}


bool ColumnListView::FullListHasItem(const CLVListItem* item) const
{
	return fFullItemList.HasItem((void*)item);
}


int32 ColumnListView::FullListCountItems() const
{
	return fFullItemList.CountItems();
}


void ColumnListView::MakeEmpty()
{
	fFullItemList.MakeEmpty();
	BListView::MakeEmpty();
}


void ColumnListView::MakeEmptyPrivate()
{
	fFullItemList.MakeEmpty();
	BListView::MakeEmpty();
}


bool ColumnListView::FullListIsEmpty() const
{
	return fFullItemList.IsEmpty();
}


int32 ColumnListView::FullListCurrentSelection(int32 index) const
{
	int32 Selection = CurrentSelection(index);
	CLVListItem* SelectedItem = (CLVListItem*)ItemAt(Selection);
	return FullListIndexOf(SelectedItem);
}


void ColumnListView::FullListDoForEach(bool (*func)(CLVListItem *))
{
	int32 NumberOfItems = fFullItemList.CountItems();
	for(int32 Counter = 0; Counter < NumberOfItems; Counter++)
		if(func((CLVListItem*)fFullItemList.ItemAt(Counter)) == true)
			return;
}


void ColumnListView::FullListDoForEach(bool (*func)(CLVListItem*, void*), void* arg2)
{
	int32 NumberOfItems = fFullItemList.CountItems();
	for(int32 Counter = 0; Counter < NumberOfItems; Counter++)
		if(func((CLVListItem*)fFullItemList.ItemAt(Counter),arg2) == true)
			return;
}


CLVListItem* ColumnListView::Superitem(const CLVListItem* item) const
{
	int32 SuperItemPos;
	uint32 ItemLevel = item->fOutlineLevel;
	if(ItemLevel == 0)
		SuperItemPos = -1;
	else
		SuperItemPos = fFullItemList.IndexOf((void*)item) - 1;
	CLVListItem* SuperItem;
	while(SuperItemPos >= 0)
	{
		SuperItem = (CLVListItem*)fFullItemList.ItemAt(SuperItemPos);
		if(SuperItem)
		{
			if(SuperItem->fOutlineLevel >= ItemLevel)
				SuperItemPos--;
			else
				break;
		}
		else
			return NULL;
	}
	if(SuperItemPos >= 0)
		return SuperItem;
	else
		return NULL;
}


int32 ColumnListView::FullListNumberOfSubitems(const CLVListItem* item) const
{
	if(!fHierarchical)
		return 0;
	int32 ItemPos = FullListIndexOf(item);
	int32 SubItemPos;
	uint32 SuperItemLevel = item->fOutlineLevel;
	if(ItemPos >= 0)
	{
		for(SubItemPos = ItemPos + 1; SubItemPos >= 1; SubItemPos++)
		{
			CLVListItem* TheItem = FullListItemAt(SubItemPos);
			if(TheItem == NULL || TheItem->fOutlineLevel <= SuperItemLevel)
				break;
		}
	}
	else
		return 0;
	return SubItemPos-ItemPos-1;
}


void ColumnListView::Expand(CLVListItem* item)
{
	BWindow* ParentWindow = Window();
	if(ParentWindow)
		ParentWindow->Lock();
	if(!(item->fSuperItem))
		item->fSuperItem = true;
	if(item->IsExpanded())
	{
		if(ParentWindow)
			ParentWindow->Unlock();
		return;
	}
	item->SetExpanded(true);
	if(!fHierarchical)
	{
		if(ParentWindow)
			ParentWindow->Unlock();
		return;
	}

	int32 DisplayIndex = IndexOf(item);
	if(DisplayIndex >= 0)
	{
		if(fExpanderColumn >= 0)
		{
			//Change the state of the arrow
			item->DrawItemColumn(this,item->fExpanderColumnRect,fExpanderColumn,true);
			SetDrawingMode(B_OP_OVER);
			DrawBitmap(&fDownArrow, BRect(0.0,0.0,item->fExpanderButtonRect.right-
				item->fExpanderButtonRect.left,10.0),item->fExpanderButtonRect);
			SetDrawingMode(B_OP_COPY);
		}

		//Add the items under it
		int32 FullListIndex = fFullItemList.IndexOf(item);
		uint32 ItemLevel = item->fOutlineLevel;
		int32 Counter = FullListIndex + 1;
		int32 AddPos = DisplayIndex + 1;
		while(true)
		{
			CLVListItem* NextItem = (CLVListItem*)fFullItemList.ItemAt(Counter);
			if(NextItem == NULL)
				break;
			if(NextItem->fOutlineLevel > ItemLevel)
			{
				BListView::AddItem(NextItem,AddPos++);
				if(NextItem->fSuperItem && !NextItem->IsExpanded())
				{
					//The item I just added is collapsed, so skip all its children
					uint32 SkipLevel = NextItem->fOutlineLevel + 1;
					while(true)
					{
						Counter++;
						NextItem = (CLVListItem*)fFullItemList.ItemAt(Counter);
						if(NextItem == NULL)
							break;
						if(NextItem->fOutlineLevel < SkipLevel)
							break;
					}
				}
				else
					Counter++;
			}
			else
				break;
		}
	}
	if(ParentWindow)
		ParentWindow->Unlock();
}


void ColumnListView::Collapse(CLVListItem* item)
{
	BWindow* ParentWindow = Window();
	if(ParentWindow)
		ParentWindow->Lock();
	if(!(item->fSuperItem))
		item->fSuperItem = true;
	if(!(item->IsExpanded()))
	{
		if(ParentWindow)
			ParentWindow->Unlock();
		return;
	}
	item->SetExpanded(false);
	if(!fHierarchical)
	{
		if(ParentWindow)
			ParentWindow->Unlock();
		return;
	}

	int32 DisplayIndex = IndexOf(item);
	if(DisplayIndex >= 0)
	{
		if(fExpanderColumn >= 0)
		{
			//Change the state of the arrow
			item->DrawItemColumn(this,item->fExpanderColumnRect,fExpanderColumn,true);
			SetDrawingMode(B_OP_OVER);
			DrawBitmap(&fRightArrow, BRect(0.0,0.0,item->fExpanderButtonRect.right-
				item->fExpanderButtonRect.left,10.0),item->fExpanderButtonRect);
			SetDrawingMode(B_OP_COPY);
		}

		//Remove the items under it
		int32 FullListIndex = fFullItemList.IndexOf(item);
		uint32 ItemLevel = item->fOutlineLevel;
		int32 NextItemIndex = DisplayIndex+1;
		while(true)
		{
			CLVListItem* NextItem = (CLVListItem*)ItemAt(NextItemIndex);
			if(NextItem)
			{
				if(NextItem->fOutlineLevel > ItemLevel)
					BListView::RemoveItem(NextItemIndex);
				else
					break;
			}
			else
				break;
		}
	}
	if(ParentWindow)
		ParentWindow->Unlock();
}


bool ColumnListView::IsExpanded(int32 fullListIndex) const
{
	CLVListItem* TheItem = (CLVListItem*)fFullItemList.ItemAt(fullListIndex);
	if(TheItem)
		return TheItem->IsExpanded();
	else
		return false;
}


void ColumnListView::SetSortFunctions(CLVCompareFuncPtr compare, CLVTrueFuncPtr istrue)
{
	fCompare = compare;
	fIsTrue = istrue;	
}


BList* CLVSortList;
Benaphore CLVSortProtectionBenaphore;
int CLVBListSortFunc(CLVListItem** item1, CLVListItem** item2);


void ColumnListView::SortItems()
{
	BWindow* ParentWindow = Window();
	if(ParentWindow)
		ParentWindow->Lock();

	BList NewList;
	int32 NumberOfItems;
	if(!fHierarchical)
		NumberOfItems = CountItems();
	else
		NumberOfItems = fFullItemList.CountItems();
	if(NumberOfItems == 0)
	{
		if(ParentWindow)
			ParentWindow->Unlock();
		return;
	}
	int32 Counter;
	BRect OldBounds;
	if(!fHierarchical)
	{
		//Plain sort
		CLVListItem** SortArray = new CLVListItem*[NumberOfItems];
		CLVListItem** ListItems = (CLVListItem**)Items();
		for(Counter = 0; Counter < NumberOfItems; Counter++)
			SortArray[Counter] = ListItems[Counter];
		SortListArray(SortArray,NumberOfItems);
		for(Counter = 0; Counter < NumberOfItems; Counter++)
			NewList.AddItem(SortArray[Counter]);
		delete[] SortArray;
	}
	else
	{
		//Block-by-block sort

		SortFullListSegment(0,0,&NewList);
		fFullItemList = NewList;
	}

	//Actually do the on-screen sort: let BListView do it since if I try to do it I'll get really bad
	//update flicker and the view position will move.  Ideally, I would just set the new BListView order
	//directly, but to do that you have to clear out the BListView using MakeEmpty() or RemoveItem().
	//BListView automatically scrolls to the top of the view with RemoveItem(), or worse still, to (0,0)
	//with MakeEmpty so it didn't work.  The only way to get it to work without flicker or moving the view
	//position is to let BListView do the sorting.  But here, since I'm doing multiparameter sorting,
	//possibly based on hierarchical position, sorted position is based on a lot more variables than a
	//straight comparison of two items, so I have to base the BListView sort on their positions in my own
	//sorted list.  The comparison function has to be a static global function, so unfortunately I have
	//to use a global sorted list and protect it with a benaphore.  At least sorting is fast and should be
	//infrequent.
	if(!CLVSortProtectionBenaphore.IsValid())
	{
		if(ParentWindow)
			ParentWindow->Unlock();
		return;
	}
	if(CLVSortProtectionBenaphore.Lock() == B_NO_ERROR)
	{
		CLVSortList = &NewList;
		BListView::SortItems((int (*)(const void*, const void*))CLVBListSortFunc);
		CLVSortProtectionBenaphore.Unlock();
	}
	if(ParentWindow)
		ParentWindow->Unlock();
}


int CLVBListSortFunc(CLVListItem** item1, CLVListItem** item2)
{
	if(CLVSortList->IndexOf(*item1) < CLVSortList->IndexOf(*item2))
		return -1;
	else
		return 1;
}


void ColumnListView::SortListArray(CLVListItem** SortArray, int32 NumberOfItems)
{
	for(int32 Counter1 = 0; Counter1 < NumberOfItems-1; Counter1++)
		for(int32 Counter2 = Counter1+1; Counter2 < NumberOfItems; Counter2++)
		{
			int CompareResult = 0;
			if(fCompare && fSortKey && fSortMode != NoSort)
			{
				CompareResult = fCompare(SortArray[Counter1],SortArray[Counter2],
					fColumnList.IndexOf(fSortKey));
				if(fSortMode == Descending)
					CompareResult = 0-CompareResult;
			}
			if(fIsTrue && fBoolGroupKey && fBoolGroupMode != NoGroup)
			{
				bool IsTrue1 = fIsTrue(SortArray[Counter1],fColumnList.IndexOf(fBoolGroupKey));
				bool IsTrue2 = fIsTrue(SortArray[Counter2],fColumnList.IndexOf(fBoolGroupKey));
				bool SortOverridden = false;
				if(IsTrue1 == true && IsTrue2 == false)
				{
					CompareResult = -1;
					SortOverridden = true;
				}
				if(IsTrue1 == false && IsTrue2 == true)
				{
					CompareResult = 1;
					SortOverridden = true;
				}
				if(fBoolGroupMode == TrueLast && SortOverridden)
					CompareResult = 0-CompareResult;
			}
			if(CompareResult == 1)
			{
				CLVListItem* Temp = SortArray[Counter1];
				SortArray[Counter1] = SortArray[Counter2];
				SortArray[Counter2] = Temp;
			}
		}
}


void ColumnListView::SortFullListSegment(int32 OriginalListStartIndex, int32 InsertionPoint,
	BList* NewList)
{
	//Identify and sort the items at this level
	BList* ItemsInThisLevel = SortItemsInThisLevel(OriginalListStartIndex);
	int32 NewItemsStopIndex = InsertionPoint + ItemsInThisLevel->CountItems();
	NewList->AddList(ItemsInThisLevel,InsertionPoint);
	delete ItemsInThisLevel;

	//Identify and sort the subitems
	for(int32 Counter = InsertionPoint; Counter < NewItemsStopIndex; Counter++)
	{
		CLVListItem* ThisItem = (CLVListItem*)NewList->ItemAt(Counter);
		CLVListItem* NextItem = (CLVListItem*)fFullItemList.ItemAt(fFullItemList.IndexOf(ThisItem)+1);
		if(ThisItem->IsSuperItem() && NextItem && ThisItem->fOutlineLevel < NextItem->fOutlineLevel)
		{
			int32 OldListSize = NewList->CountItems();
			SortFullListSegment(fFullItemList.IndexOf(ThisItem)+1,Counter+1,NewList);
			int32 NewListSize = NewList->CountItems();
			NewItemsStopIndex += NewListSize - OldListSize;
			Counter += NewListSize - OldListSize;
		}
	}
}


BList* ColumnListView::SortItemsInThisLevel(int32 OriginalListStartIndex)
{
	uint32 ThisLevel = ((CLVListItem*)fFullItemList.ItemAt(OriginalListStartIndex))->fOutlineLevel;

	//Create a new BList of the items in this level
	int32 Counter = OriginalListStartIndex;
	int32 ItemsInThisLevel = 0;
	BList* ThisLevelItems = new BList(16);
	while(true)
	{
		CLVListItem* ThisItem = (CLVListItem*)fFullItemList.ItemAt(Counter);
		if(ThisItem == NULL)
			break;
		uint32 ThisItemLevel = ThisItem->fOutlineLevel;
		if(ThisItemLevel == ThisLevel)
		{
			ThisLevelItems->AddItem(ThisItem);
			ItemsInThisLevel++;
		}
		else if(ThisItemLevel < ThisLevel)
			break;
		Counter++;
	}

	//Sort the BList of the items in this level
	CLVListItem** SortArray = new CLVListItem*[ItemsInThisLevel];
	CLVListItem** ListItems = (CLVListItem**)ThisLevelItems->Items();
	for(Counter = 0; Counter < ItemsInThisLevel; Counter++)
		SortArray[Counter] = ListItems[Counter];
	ThisLevelItems->MakeEmpty();
	SortListArray(SortArray,ItemsInThisLevel);
	for(Counter = 0; Counter < ItemsInThisLevel; Counter++)
		ThisLevelItems->AddItem(SortArray[Counter]);
	return ThisLevelItems;
}


const CLVListItem** ColumnListView::Items() const
{
	if(fHierarchical)
		return (CLVListItem**)fFullItemList.Items();
	else
		return (CLVListItem**)BListView::Items();
}


