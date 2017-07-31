//Column list view header file



//******************************************************************************************************
//**** PROJECT HEADER FILES AND CLASS NAME DECLARATIONS
//******************************************************************************************************
#include "Colors.h"
class CLVColumn;
class CLVListItem;
class CLVColumnLabelView;
class CLVFillerView;
class CLVContainerView;
#include "PrefilledBitmap.h"


//******************************************************************************************************
//**** CONSTANTS AND TYPE DEFINITIONS
//******************************************************************************************************
enum CLVSortMode
{
	NoSort,
	Ascending,
	Descending
};

enum CLVGroupMode
{
	NoGroup,
	TrueFirst,
	TrueLast
};

typedef int (*CLVCompareFuncPtr)(const CLVListItem* item1, const CLVListItem* item2, int16 sort_key);
typedef bool (*CLVTrueFuncPtr)(const CLVListItem* item, int16 bool_group_key);


//******************************************************************************************************
//**** ColumnListView CLASS DECLARATION
//******************************************************************************************************
#include <ListView.h>
class ColumnListView : public BListView
{
	public:
		//Constructor and destructor
		ColumnListView(	BRect Frame,
						BScrollView** ContainerView,	//Used to get back a pointer to the container
														//view that will hold the ColumnListView, the
														//the CLVColumnLabelView, and the scrollbars.
														//If no scroll bars or border are asked for,
														//this will act like a plain BView container.
						const char* Name = NULL,
						uint32 ResizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
						uint32 flags = B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE,
						list_view_type Type = B_SINGLE_SELECTION_LIST,
						bool hierarchical = false,
						bool horizontal = true,					//Which scroll bars should I add, if any
						bool vertical = true,
						border_style border = B_NO_BORDER,		//What type of border to add, if any
						const BFont* LabelFont = be_plain_font);
		virtual ~ColumnListView();

		//Archival stuff
			/*** Not implemented yet
		ColumnListView(BMessage* archive);
		static ColumnListView* Instantiate(BMessage* data);
		virtual	status_t Archive(BMessage* data, bool deep = true) const;
			***/

		//Column setup functions
		virtual bool AddColumn(CLVColumn* Column);			//Note that a column may only be added to
															//one ColumnListView at a time, and may not
															//be added more than once to the same
															//ColumnListView without removing it
															//inbetween
		virtual bool AddColumnList(BList* NewColumns);
		virtual bool RemoveColumn(CLVColumn* Column);
		virtual bool RemoveColumns(CLVColumn* Column, int16 Count);	//Finds Column in ColumnList
																	//and removes Count columns and
																	//their data from the view
																	//and its items
		int16 CountColumns() const;
		int16 IndexOfColumn(CLVColumn* column) const;
		CLVColumn* ColumnAt(int16 column_index) const;
		virtual bool SetDisplayOrder(const int16* Order);
			//Sets the display order: each int16 in the Order list specifies the column index of the
			//next column to display.  Note that this DOES NOT get called if the user drags a
			//column, so overriding it will not inform you of user changes.  If you need that info,
			//override DisplayOrderChanged instead.  Also note that SetDisplayOrder does call
			//DisplayOrderChanged(false).
		virtual void ColumnWidthChanged(int16 ColumnIndex, float NewWidth);
		virtual void DisplayOrderChanged(const int16* order);
			//Override this if you want to find out when the display order changed.
		int16* DisplayOrder() const;	//Gets the display order in the same format as that used by
										//SetDisplayOrder.  The returned array belongs to the caller and
										//must be delete[]'d when done with it.
		virtual void SetSortKey(int16 ColumnIndex);				//Set it to -1 to remove the sort key
		virtual void SetSortMode(CLVSortMode Mode);
		void ReverseSortMode();
		CLVSortMode Sorting(int16* SortKey) const;	//Returns the sort mode and key.  Note: sorting
														//will only occur if the key column is shown.
														//SortKey is negative if no sort key is selected
		virtual void SetBoolGroupKey(int16);					//Set it to -1 to remove the group key
		virtual void SetBoolGroupMode(CLVGroupMode Mode);
		void ReverseBoolGroupMode();
		CLVGroupMode BoolGrouping(int16* GroupKey) const;	//Returns the boolean grouping mode and
																//key.  Note: grouping will only occur if
																//the key column is shown so that it will
																//be visually obvious to the user why items
																//can't be dragged to the other group.
																//GroupKey is negative if no group key is
																//selected

		//BView overrides
		virtual void FrameResized(float Width, float Height);
		virtual void AttachedToWindow();
 		virtual void ScrollTo(BPoint point);
 		virtual void MouseDown(BPoint point);
 
		//List functions
		virtual bool AddUnder(CLVListItem* item, CLVListItem* superitem);
		virtual bool AddItem(CLVListItem* item, int32 fullListIndex);
		virtual bool AddItem(CLVListItem* item);
		virtual bool AddList(BList* newItems);
		virtual bool AddList(BList* newItems, int32 fullListIndex);
		virtual bool RemoveItem(CLVListItem* item);
		virtual BListItem* RemoveItem(int32 fullListIndex);		//Actually returns CLVListItem*
		virtual bool RemoveItems(int32 fullListIndex, int32 count);
		virtual void MakeEmpty();
		CLVListItem* FullListItemAt(int32 fullListIndex)  const;
		int32 FullListIndexOf(const CLVListItem* item) const;
		int32 FullListIndexOf(BPoint point) const;
		CLVListItem* FullListFirstItem() const;
		CLVListItem* FullListLastItem() const;
		bool FullListHasItem(const CLVListItem* item) const;
		int32 FullListCountItems() const;
		bool FullListIsEmpty() const;
		int32 FullListCurrentSelection(int32 index = 0) const;
		void FullListDoForEach(bool (*func)(CLVListItem*));
		void FullListDoForEach(bool (*func)(CLVListItem*, void*), void* arg2);
		CLVListItem* Superitem(const CLVListItem* item) const;
		int32 FullListNumberOfSubitems(const CLVListItem* item) const;
		virtual void Expand(CLVListItem* item);
		virtual void Collapse(CLVListItem* item);
		bool IsExpanded(int32 fullListIndex) const;
		void SetSortFunctions(CLVCompareFuncPtr compare,CLVTrueFuncPtr istrue);
		void SortItems();
		const CLVListItem** Items() const;

		//Friends
		#ifdef CLVMainView_CPP
			friend void CLVMainView::ScrollTo(BPoint Point);
		#endif
		#ifdef CLVColumn_CPP
			friend void CLVColumn::SetWidth(float width);
			friend void CLVColumn::SetShown(bool shown);
		#endif
		#ifdef CLVColumnLabelView_CPP
			friend CLVColumnLabelView::CLVColumnLabelView(BRect Bounds,ColumnListView* parent,
				const BFont* Font);
			friend void CLVColumnLabelView::Draw(BRect UpdateRect);
			friend void CLVColumnLabelView::MouseDown(BPoint Point);
			friend void CLVColumnLabelView::MessageReceived(BMessage* message);
			friend void CLVColumnLabelView::ShiftDragGroup(int16 NewPos);
		#endif
		#ifdef CLVListItem_CPP
			friend void CLVListItem::DrawItem(BView* owner, BRect itemRect, bool complete);
			friend void CLVListItem::Update(BView* owner, const BFont* font);
		#endif

	private:
		void UpdateColumnSizesDataRectSizeScrollBars();
		void UpdateScrollBars();
		void ColumnsChanged();
		void CreateContainer(bool horizontal, bool vertical, border_style border, uint32 ResizingMode,
			uint32 flags);
		void SortListArray(CLVListItem** SortArray, int32 NumberOfItems);
		void MakeEmptyPrivate();
		bool AddListPrivate(BList* newItems, int32 fullListIndex);
		bool AddItemPrivate(CLVListItem* item, int32 fullListIndex);
		void SortFullListSegment(int32 OriginalListStartIndex, int32 InsertionPoint, BList* NewList);
		BList* SortItemsInThisLevel(int32 OriginalListStartIndex);

		CLVColumnLabelView* fColumnLabelView;
		CLVContainerView* fScrollView;
		CLVFillerView* fFillerView;
		bool fHierarchical;
		BList fColumnList;
		BList fColumnDisplayList;
		float fDataWidth,fDataHeight,fPageWidth,fPageHeight;
		CLVSortMode fSortMode;
		CLVColumn* fSortKey;
		CLVGroupMode fBoolGroupMode;
		CLVColumn* fBoolGroupKey;
		PrefilledBitmap fRightArrow;
		PrefilledBitmap fDownArrow;
		BList fFullItemList;
		int16 fExpanderColumn;
		CLVCompareFuncPtr fCompare;
		CLVTrueFuncPtr fIsTrue;
};


class CLVFillerView : public BView
{
	public:
		CLVFillerView(float Left,float Top);
		~CLVFillerView();
		void Draw(BRect Update);
};