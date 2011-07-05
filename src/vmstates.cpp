#include "vmstates.h"
#include <stdlib.h>

const char* KStateWidgetTypeName = "StGtkWidgetPtr";

// User defined state, based on DEST expression
//
VmStateWidget::VmStateWidget(const string& aInstName, CAE_Object* aMan):
    CAE_State(aInstName.c_str(), sizeof(GObject*), aMan, TTransInfo())
{
    CAE_State::ConstructL();
    SetType(KStateWidgetTypeName);
}

VmStateWidget::~VmStateWidget()
{
    GtkWidget* widget = Value(); 
    if (widget != NULL) {
	gtk_widget_destroy(widget);
    }

}

VmStateWidget* VmStateWidget::New(const char* aInstName, CAE_Object* aMan)
{
    return new VmStateWidget(aInstName, aMan);
}

char* VmStateWidget::DataToStr(TBool aCurr) const
{
    TUint32 data = *((TUint32 *) (aCurr ? iCurr : iNew));
    char* buf = (char *) malloc(10);
    memset(buf, 0, 10);
    sprintf(buf, "%x ", (unsigned int ) data);
    return buf;
}

void VmStateWidget::DataFromStr(const char* aStr, void *aData) const
{
    sscanf(aStr, "%x", (unsigned int *) aData);
}

void *VmStateWidget::DoGetFbObj(const char *aName)
{
    if (strcmp(aName, Type()) == 0)
	return this;
    else
	return CAE_State::DoGetFbObj(aName);
}
