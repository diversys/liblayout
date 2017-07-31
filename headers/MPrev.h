#ifndef _MPREV_H
#define _MPREV_H

#include "MPictureButton.h"

#if __POWERPC__
#pragma simple_class_byval off
#endif

class IMPEXPLIBLAYOUT MPrev : public MPictureButton
{
	public:		MPrev(BHandler*);
				MPrev(BHandler *id, BMessage*);
				MPrev(BMessage*);
				virtual ~MPrev();
				virtual status_t Archive(BMessage *archive, bool deep=true) const;
				static BArchivable *Instantiate(BMessage *archive);
				virtual void MakePictures();
};
extern const IMPEXPLIBLAYOUT char M_BUTTON_POINTER[];

#endif
