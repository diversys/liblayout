//Column list header header file


#include <SupportDefs.h>

//******************************************************************************************************
//**** PROJECT HEADER FILES AND CLASS NAME DECLARATIONS
//******************************************************************************************************
class ColumnListView;
class CLVColumn;
class CLVListItem;


//******************************************************************************************************
//**** CONSTANTS
//******************************************************************************************************
//Flags
enum
{
	CLV_SORT_KEYABLE =			0x00000001,		//Can be used as the sorting key
	CLV_BOOL_GROUPABLE =		0x00000002,		//Can be used as the grouping key
	CLV_NOT_MOVABLE =			0x00000004,		//Column can't be moved by user
	CLV_NOT_RESIZABLE =			0x00000008,		//Column can't be resized by user
	CLV_LOCK_AT_BEGINNING =		0x00000010,		//Movable columns may not be placed or moved by the user
												//into a position before this one
	CLV_LOCK_AT_END =			0x00000020,		//Movable columns may not be placed or moved by the user
												//into a position after this one
	CLV_NOT_SHOWN =				0x00000040,		//This column is not shown initially
	CLV_MERGE_WITH_RIGHT =		0x00000080,		//Merge this column label with the one that follows it.
	CLV_LOCK_WITH_RIGHT =		0x00000100,		//Lock this column to the one that follows it such that
												//if the column to the right is moved by the user, this
												//one will move with it and vice versa
	CLV_EXPANDER =				0x00000200,		//Column contains an expander.  You may only use one
												//expander in a ColumnListView, and an expander may not be
												//added to a non-hierarchal ColumnListView.  It may not
												//have a label.  Its width is automatically set to 20.0.
												//The only flags that affect it are CLV_NOT_MOVABLE,
												//CLV_LOCK_AT_BEGINNING, CLV_NOT_SHOWN and
												//CLV_LOCK_WITH_RIGHT.  The others are set for you:
												//CLV_NOT_RESIZABLE | CLV_MERGE_WITH_RIGHT
	CLV_PUSH_PASS =				0x00000400		//Causes this column, if pushed by an expander to the
												//left, to pass that push on and also push the next
												//column to the right.
};


//******************************************************************************************************
//**** ColumnListView CLASS DECLARATION
//******************************************************************************************************
class CLVColumn
{
	public:
		//Constructor and destructor
		CLVColumn(	const char* label,
					float width = 20.0,
					uint32 flags = 0,
					float min_width = 20.0);
		virtual ~CLVColumn();

		//Archival stuff
		/* Not implemented yet
		CLVColumn(BMessage* archive);
		static CLVColumn* Instantiate(BMessage* data);
		virtual	status_t Archive(BMessage* data, bool deep = true) const;
		*/

		//Functions
		float Width() const;
		virtual void SetWidth(float width);	//Can be overridden to detect changes to the column width
												//however since you are probably overriding
												//ColumnListView and dealing with an array of columns
												//anyway, it is probably more useful to override
												//ColumnListView::ColumnWidthChanged to detect changes to
												//column widths
		uint32 Flags() const;
		bool IsShown() const;
		virtual void SetShown(bool shown);

		//Friend functions
		#ifdef ColumnListView_CPP
			friend void ColumnListView::UpdateColumnSizesDataRectSizeScrollBars();
			friend bool ColumnListView::AddColumn(CLVColumn* Column);
			friend bool ColumnListView::AddColumnList(BList* NewColumns);
			friend bool ColumnListView::RemoveColumn(CLVColumn* Column);
			friend bool ColumnListView::RemoveColumns(CLVColumn* Column, int16 Count);	
			friend void ColumnListView::SetSortKey(int16 ColumnIndex);
			friend void ColumnListView::SetSortMode(CLVSortMode Mode);
			friend void ColumnListView::SetBoolGroupKey(int16 ColumnIndex);
			friend void ColumnListView::SetBoolGroupMode(CLVGroupMode Mode);
		#endif
		#ifdef CLVColumnLabelView_CPP
			friend void CLVColumnLabelView::Draw(BRect UpdateRect);
			friend void CLVColumnLabelView::MouseDown(BPoint Point);
			friend void CLVColumnLabelView::MessageReceived(BMessage* message);
			friend void CLVColumnLabelView::UpdateDragGroups();
			friend void CLVColumnLabelView::SetSnapMinMax();
		#endif
		#ifdef CLVListItem_CPP
			friend void CLVListItem::DrawItem(BView *owner, BRect itemRect, bool complete);
		#endif

	private:
		const char *fLabel;
		float fWidth;
		float fMinWidth;
		float fColumnBegin;
		float fColumnEnd;
		uint32 fFlags;
		bool fPushedByExpander;
		ColumnListView* fParent;
};
