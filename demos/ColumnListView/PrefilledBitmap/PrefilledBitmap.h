
#ifndef _PREFILLEDBITMAP_H
#define _PREFILLEDBITMAP_H

#include <Bitmap.h>
class PrefilledBitmap : public BBitmap
{
	public:
		PrefilledBitmap(BRect bounds, color_space space, const void *data, bool acceptsViews,
			bool needsContiguousMemory);
		~PrefilledBitmap();
};
#endif
