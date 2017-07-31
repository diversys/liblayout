//CLVListItem header file


#include <ListItem.h>
#include <Region.h>

//******************************************************************************************************
//**** CLVItem CLASS DECLARATION
//******************************************************************************************************
class CLVListItem : public BListItem
{
	public:
		//Constructor and destructor
		CLVListItem(uint32 level = 0, bool superitem = false, bool expanded = false, float minheight = 0.0);
		virtual ~CLVListItem();

		//Archival stuff
		/* Not implemented yet
		CLVItem(BMessage* archive);
		static CLVItem* Instantiate(BMessage* data);
		virtual	status_t Archive(BMessage* data, bool deep = true) const;
		*/
		
		virtual void DrawItemColumn(BView* owner, BRect item_column_rect, int16 column_index,
			bool complete) = 0;	//column_index (0-N) is based on the order in which the columns were added
								//to the ColumnListView, not the display order.  An index of -1 indicates
								//that the program needs to draw a blank area beyond the last column.  The
								//main purpose is to allow the highlighting bar to continue all the way to
								//the end of the ColumnListView, even after the end of the last column.
		virtual void Update(BView* owner, const BFont* font);
		bool IsSuperItem() const;
		void SetSuperItem(bool superitem);
		uint32 OutlineLevel() const;
		void SetOutlineLevel(uint32 level);

		#ifdef ColumnListView_CPP
			friend void ColumnListView::MouseDown(BPoint point);
			friend bool ColumnListView::AddUnder(CLVListItem* item, CLVListItem* superitem);
			friend bool ColumnListView::AddItemPrivate(CLVListItem* item, int32 fullListIndex);
			friend CLVListItem* ColumnListView::Superitem(const CLVListItem* item) const;
			friend void ColumnListView::Expand(CLVListItem* item);
			friend void ColumnListView::Collapse(CLVListItem* item);
			friend bool ColumnListView::RemoveItems(int32 fullListIndex, int32 count);
			friend int32 ColumnListView::FullListNumberOfSubitems(const CLVListItem* item) const;
			friend BList* ColumnListView::SortItemsInThisLevel(int32 OriginalListStartIndex);
			friend void ColumnListView::SortFullListSegment(int32 OriginalListStartIndex,
				int32 InsertionPoint,BList* NewList);
		#endif

	private:
		virtual void DrawItem(BView* owner, BRect itemRect, bool complete);

		bool fSuperItem;
		uint32 fOutlineLevel;
		float fMinHeight;
		BRect fExpanderButtonRect;
		BRect fExpanderColumnRect;
};