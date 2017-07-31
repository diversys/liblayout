#ifndef _MNEXT_H
#define _MNEXT_H

#include "MPictureButton.h"

#if __POWERPC__
#pragma simple_class_byval off
#endif

class IMPEXPLIBLAYOUT MNext : public MPictureButton
{
	public:		MNext(BHandler *id=NULL);
				MNext(BHandler *id, BMessage*);
				MNext(BMessage*);
				virtual ~MNext();
				virtual status_t Archive(BMessage *archive, bool deep=true) const;
				static BArchivable *Instantiate(BMessage *archive);
				
				virtual void MakePictures();
};

extern const IMPEXPLIBLAYOUT char M_BUTTON_POINTER[];

#endif
