# The Layout Library, release 1.4.1

## What is it?

The Layout Libary (liblayout) is an award winning shared library that allows programmers to quickly and easily create fully font-sensitive and/or resizable GUI's for their programs. liblayout also supports color-drops from applications such as [roDesign](http://www.rodesign.com)'s roColour and [Beatware](http://www.beatware.com)'s Painter, and it supports font-drops from FontSelector or any other application that uses [Dianne Hackborn](http://www.angryredplanet.com/~hackbod/)'s FFont class for storing fonts in BMessages.

## Copyright and distribution

The layout library, all headerfiles and documentation are Copyright © 1996-1999 Marco Nelissen (marcone@xs4all.nl).

liblayout is freely distributable, and freely usable for non-commercial purposes. For commercial licensing, please see the documentation, or mail the author at [marcone@xs4all.nl](MAILTO:marcone@xs4all.nl).

## Warranty

**There is no warranty for liblayout. You use it entirely at your own risk. By using liblayout in your own software, you agree to do so at your own risk, and to hold the author of liblayout harmless from any losses, expenses, damages and costs incurred by the author of liblayout based on any claim, action, suit or proceeding brought forth against you by any third party.**  
(in other words: use it, but if it breaks anything, don't come asking me for money, not even if people ask _you_ for money because liblayout did a bad thing).

The latest version of liblayout and supporting files can be found at [Marco's BeOS Corner](https://web.archive.org/web/20071227072934/http://www.xs4all.nl:80/~marcone/be.html).

## Installation

To install, `pkgman install liblayout`
Or: move liblayout.so to /boot/home/config/non-packaged/lib  

## History

### Release 1.4.1

*   Haiku fixes

### Release 1.4.0

*   use ui_color() to get correct default color
*   correctly set alpha for default colors
*   removed some debug output and other cruft
*   make MBViewWrapper behave like the documentation says
*   make everytying behave correctly on Dano
*   make dynamic adding of views to LayeredGroup work nicer
*   strdup() label-string for MBorder
*   fix sizing of MCheckBox
*   various fixes to SpinButton class
*   fix several memory leaks

### Release 1.3.2

*   Progressbar now also includes position on release
*   Fixed LayeredGroup::layoutprefs()
*   MTextView handles pasting of text correctly now
*   Some documentation fixes
*   TabGroup will work properly if you don't specify an inital child view
*   Maximum size of MBViewWrapper set properly (was uninitialized)
*   Fixed crasher in marching ants highlight of MBorder
*   Fixed documentation of MDividable
*   Fixed target-bug in MRadioGroup
*   Removed unneeded B_FULL_UPDATE_ON_RESIZE from MBorder

### Release 1.3.1

*   Some fixes in MBViewWrapper
*   Fixed SpinButton headerfile
*   TabGroup can have spacing around its child-view
*   Added MPrev and MNext

### Release 1.3.0

*   MBorder label color will adapt to background
*   Added MSlider
*   Added SpinButton
*   Tapedeck-buttons now have a disabled state image as well
*   Added another constructor to tapedeck-buttons
*   Fixed memory leak in tapedeck-buttons
*   Fixed label-less MPopup

### Release 1.2.8

*   Fixed rounding bug in MBorder::layoutprefs()
*   Added MWindow::StartDragging()

### Release 1.2.7

*   Fixed rounding bug in MStringView::layoutprefs()
*   Fixed MMenuBar
*   Added MBorder::SetHighlightColors()
*   Added TabGroup::ActiveTab()
*   Added another constructor to MTextControl
*   Fixed MTextControl color

### Release 1.2.6

*   Fixed ctrl-alt-shift-click popup menu layout
*   Fixed MWindow::FrameResized to not show empty windows On Genki/4 and up
*   Workaround for switching workspaces while dragging an MDragBar

### Release 1.2.5

*   Fixed MWindow::RecalcSize to not show empty windows On Genki/4 and up

### Release 1.2.4

*   Slightly changed FILL_COLOR to match those Be classes that refuse to change colors.
*   Added MTabView (one of those classes that refuse to change color...)
*   Resizing speed of windows improved
*   Word-wrapping MTextView works correctly in MScrollView
*   Fixed dynamically adding tabs to a TabView

### Release 1.2.2

*   Fixed MVolume drawing against non-grey background

### Release 1.2.1

*   fixed alpha color handling for a bunch of classes

### Release 1.2.0

*   MDragbar now raises window if clicked without dragging.
*   Fixed memory leaks in MWindow, HGroup, VGroup and MVolume
*   MVolume now has two popup-modes
*   MVolume sets its target BHandler
*   TabGroup flickers less when switching tabs
*   Fixed refresh-bugs in MBorder and PropGadget
*   liblayout now exports _LIBLAYOUT_VERSION_ which describes its version.

### Release 1.1.0

*   Added workaround for bug 980417-124350 (BPictureButton::Invoke doesn't use BMessage)
*   Fixed rendering bug in MProgressBar
*   Removed debugging output from MRadioGroup::layout
*   Fixed layout bug in MPopup
*   MDragBar now sets FILL_COLOR once in the constructor, instead of every Draw()
*   Added look-and-feel constructor to MWindow

### Release 1.0.2

*   Fixed MTextView text-selection

### Release 1.0.1

*   Fixed TabGroup-rendering

### Release 1.0

*   New classes:
    *   MListView
    *   MScrollView
    *   MMenuBar
    *   MSplitter (formerly known as MSeperator)
    *   MProgressBar (formerly known as MStatusBar)
    *   MBViewWrapper (container for standard BView)
    *   MPictureButton
    *   MDividable
*   removed AsyncPictureButton (parts of it were assimilated by MPictureButton)
*   popups and textcontrols can now be aligned with each other
*   labeled border's minimum size now also depends on the size of the label
*   changed appearance of TabGroup
*   TabGroup now has a disabled state as well
*   font settings are now separate for each window
*   fixed MTextView
    *   it now works with MScrollView
    *   it now has a correct textrect.
    *   resizing speed improved
*   buttons no longer selectable when disabled
*   HGroup and VGroup size can be constrained in all directions
*   renamed MSeperator to MSplitter
*   swapped M_DEPRESSED_BORDER and M_ETCHED_BORDER, which where previously reversed.
*   Fixed M_ETCHED_BORDER, which previously had its colors reversed.
*   added support for app_server-resized views to MWindow
*   Moved some stuff out of the MView base class that had no business being there
*   Added Archive/Instantiate and various other stub functions. These functions are not yet fully functional, but were added now to be able to maintain binary compatibility in the future.
*   Fixed bug in PropGadget constructor (initial values were reset to 0.5)
*   Added popup menu to MWindow (ctrl-command-shift click to activate it)
*   Fixed MPopup, which now handles dynamic font-changes much better.
*   Fixed MRadioGroup, it now correctly works together with MSplitter
*   Fixed memory leak in MProgressBar
*   MPictureButtons can now, like MButton, be repeating and report the first click

### demo 3

*   color-handling changed:
    *   setcolorscheme() renamed to SetDefaultColors()
    *   colors copied from defaults at construction time
    *   setcolor() call to change colors afterwards
    *   all classes adapted to correctly handle color-changes
*   added MWindow class (handles resizing of content and color-drops)
*   support for run-time font-changes and outrageous font-sizes
*   added MApplication class (handles run-time font-changes)
*   removed navigation-highlight of MOutlineListView.
*   fixed usage of uninitialized variables in MBorder and MButton
*   fixed TabGroup class: previous version would act strange when layoutprefs was called multiple times
*   Fixed MVolume: previous version wouldn't draw correctly against non-grey background
*   MVolume now sends messages
*   MRadioGroup now has "infinite" maximum size by default
*   LayeredGroup can now be controlled through messages
*   MButtons and AsyncPictureButtons (which includes the "tapedeck"-buttons) now tell you if they were doubleclicked, and which button(s) they were clicked with.
*   MButtons can be repeating, and can report the first mouse-down
*   fixed some bugs in the asynchronous controls
*   grouping classes now derive from abstract "MGroup" class
*   group classes now handle centering of children, making it easier to create your own controls (and slightly harder to make your own grouping classes)
