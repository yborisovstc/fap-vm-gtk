#ifndef __FAPVM_STATES_H
#define __FAPVM_STATES_H

#include <fapbase.h>
#include <gtk/gtk.h>

class VmStateWidget : public CAE_State
{
    public:
	static VmStateWidget* New(const char* aInstName, CAE_Object* aMan);
	const char* Type() { return "VmStateGoptr";} 
	VmStateWidget(const string& aInstName, CAE_Object* aMan);
	virtual ~VmStateWidget();
	GtkWidget* Value() { return GTK_WIDGET(*((GtkWidget**) iCurr));};
    protected:
	virtual char* DataToStr(TBool aCurr) const;
	virtual void DataFromStr(const char* aStr, void *aData) const;
	virtual void *DoGetFbObj(const char *aName);

};


#endif
