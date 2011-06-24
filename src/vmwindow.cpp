#include "vmwindow.h"
#include "fapstext.h"
#include <deslbase.h>
#include <gtk/gtk.h>

extern "C" 
{
    CAE_ProviderBase* init() 
    {
	return new CProvider();
    }
}

void trans_window_init(CAE_Object* aObject, CAE_StateBase* aState);

void trans_drawing_area_init(CAE_Object* aObject, CAE_StateBase* aState);
void trans_drawing_area_coord_pf(CAE_Object* aObject, CAE_StateBase* aState);
void trans_drawing_area_size(CAE_Object* aObject, CAE_StateBase* aState);

void trans_fixed_init(CAE_Object* aObject, CAE_StateBase* aState);

const TTransInfo KTi_window_init = TTransInfo(trans_window_init, "vm_window_init");
const TTransInfo KTi_drawing_area_init = TTransInfo(trans_drawing_area_init, "vm_drawing_area_init");
const TTransInfo KTi_drawing_area_coord_pf = TTransInfo(trans_drawing_area_coord_pf, "vm_drawing_area_coord_pf");
const TTransInfo KTi_drawing_area_size = TTransInfo(trans_drawing_area_size, "vm_drawing_area_size");
const TTransInfo KTi_fixed_init = TTransInfo(trans_fixed_init, "vm_fixed_init");

static const TTransInfo* tinfos[] = {
    &KTi_window_init, 
    &KTi_drawing_area_init, 
    &KTi_drawing_area_coord_pf, 
    &KTi_drawing_area_size, 
    &KTi_fixed_init, 
    NULL};


CProvider::CProvider(): CAE_ProviderBase("vmgtk")
{
    RegisterTransfs(tinfos);
}

CProvider::~CProvider() 
{
}


CAE_StateBase* CProvider::CreateStateL(const char *aTypeUid, const char* aInstName, CAE_Object* aMan) const
{
    return NULL;
}

CAE_EBase* CProvider::CreateObjectL(TUint32 aTypeUid) const
{
    return NULL;
}

CAE_EBase* CProvider::CreateObjectL(const char *aName) const
{
    return NULL;
}

const TTransInfo* CProvider::GetTransf(const char *aName) const
{
    _FAP_ASSERT(aName != NULL);
    const TTransInfo *res = NULL;
    if (iTransfs.count(aName) > 0) {
	res = iTransfs.find(aName)->second;
    }
    return res;
}

void CProvider::RegisterState(const TStateInfo *aInfo)
{
}

void CProvider::RegisterStates(const TStateInfo **aInfos)
{
}

void CProvider::RegisterTransf(const TTransInfo *aTrans)
{
    _FAP_ASSERT(aTrans != NULL);
    iTransfs[aTrans->iId] = aTrans;
}

void CProvider::RegisterTransfs(const TTransInfo **aTransfs)
{
    _FAP_ASSERT(aTransfs != NULL);
    for (int i = 0; aTransfs[i] != NULL; i++) {
	RegisterTransf(aTransfs[i]);
    }
}

const CAE_Formatter* CProvider::GetFormatter(int aUid) const
{
    return NULL;
}

void CProvider::RegisterFormatter(CAE_Formatter *aForm)
{
}

CAE_ChromoBase* CProvider::CreateChromo() const
{
}

CAE_TranExBase* CProvider::CreateTranEx(MCAE_LogRec* aLogger) const
{
}

MAE_Opv* CProvider::CreateViewProxy()
{
} 


void trans_window_init(CAE_Object* aObject, CAE_StateBase* aState)
{
    CAE_TState<TUint32>& self = (CAE_TState<TUint32>&) *aState;
    if (~self == 0) {
	GtkWidget* widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(widget);
	gtk_window_maximize(GTK_WINDOW(widget));
	self = (TUint32) widget;
    }
}

gboolean drawing_area_handle_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    //gdk_draw_rectangle(widget->window, widget->style->fg_gc[GTK_STATE_ACTIVE], TRUE, 0, 0, alloc.width/2, alloc.height/2);
    gdk_draw_arc (widget->window, widget->style->fg_gc[GTK_STATE_ACTIVE], TRUE, 0, 0, alloc.width, alloc.height, 0, 64 * 360);
    return FALSE;
}

void trans_drawing_area_init(CAE_Object* aObject, CAE_StateBase* aState)
{
    CAE_TState<TUint32>& self = (CAE_TState<TUint32>&) *aState;
    const TUint32& parent = self.Inp("parent");
    if (~self == 0 && parent != 0) {
	const CF_TdVectF& size = self.Inp("size");
	GtkWidget* widget = gtk_drawing_area_new();
	GtkDrawingArea* area = GTK_DRAWING_AREA(widget);
	gtk_widget_show(widget);
	gtk_widget_set_size_request(widget, size.iX, size.iY);
	GtkContainer* pcont = GTK_CONTAINER(parent);
	GtkWidget* pwidget = GTK_WIDGET(parent);
	gtk_container_add(pcont, widget);
	GtkAllocation paralloc;
	gtk_widget_get_allocation(pwidget, &paralloc);
	TInt isy = paralloc.height - size.iY;
	gtk_fixed_move(GTK_FIXED(pwidget), widget, 0, isy);
	g_signal_connect(G_OBJECT(widget), "expose_event", G_CALLBACK(drawing_area_handle_expose_event), NULL);
	self = (TUint32) widget;
    }
}

void trans_drawing_area_size(CAE_Object* aObject, CAE_StateBase* aState)
{
    CAE_TState<CF_TdVectF>& self = (CAE_TState<TUint32>&) *aState;
    const TUint32& iwidget = self.Inp("widget");
    CF_TdVectF size = ~self;
    CAE_StateBase* sinp = self.Input("size");
    if (sinp != NULL) {
	const CF_TdVectF& ssize = self.Inp("size");
	size = ssize;
	self = size;
    }
    if (iwidget != 0) {
	GtkWidget* widget = GTK_WIDGET(iwidget);
	gtk_widget_set_size_request(widget, size.iX, size.iY);
    }
}

void trans_drawing_area_coord_pf(CAE_Object* aObject, CAE_StateBase* aState)
{
    CAE_TState<CF_TdVectF>& self = (CAE_TState<TUint32>&) *aState;
    CAE_StateBase* sinp = self.Input("value");
    if (sinp != NULL) {
	CF_TdVectF value;
	CAE_StateEx* seinp = sinp->GetFbObj(seinp);
	if (seinp != NULL) {
	    // Assuming it is compatible ti TVectF
	    string datas = seinp->Value().ToString();
	    CSL_EfVectF::FromStr(value, datas);
	}
	else {
	    CAE_TState<CF_TdVectF>* sinpt = sinp->GetFbObj(sinpt);
	    if (sinpt != NULL) {
		const CF_TdVectF& valr = self.Inp("value");
		value = valr;
	    }
	    else {
		self.Logger()->WriteFormat("vm_drawing_area_coord_pf:: ERROR: Inp [value] is of not supported type");
	    }
	}
	const TUint32& iwidget = self.Inp("widget");
	if (iwidget != 0) {
	    // Size
	    const CF_TdVectF& asize = self.Inp("area_size");
	    GtkWidget* widget = GTK_WIDGET(iwidget);
	    GtkWidget* parent = gtk_widget_get_parent(widget);
	    GtkFixed* fixed = GTK_FIXED(parent);
	    GtkAllocation alloc;
	    gtk_widget_get_allocation(widget, &alloc);
	    GtkAllocation paralloc;
	    gtk_widget_get_allocation(parent, &paralloc);
	    // Limit the coords
	    float lx = (value.iX < asize.iX) ? value.iX : asize.iX;
	    float ly = (value.iY < asize.iY) ? value.iY : asize.iY;
	    // Scale the coords
	    float rx = lx/asize.iX;
	    float ry = ly/asize.iY;
	    // Map to native coords
	    TInt x = paralloc.width * rx;
	    TInt y = paralloc.height * ry;
	    TInt xlim = paralloc.width - alloc.width;
	    TInt ylim = paralloc.height - alloc.height;
	    x = (x > xlim) ? xlim : x;
	    y = (y > ylim) ? ylim : y;
	    TInt isy = paralloc.height - y;
	    gtk_fixed_move(fixed, widget, x, isy);
	    self = value;
	}
    }
    else {
	self.Logger()->WriteFormat("vm_drawing_area_coord_pf:: ERROR: Inp [value] not connected");
    }
}


void fixed_handle_size_request_event( GtkWidget *widget, GtkRequisition *req, gpointer data)
{
    GtkWidget* parent = gtk_widget_get_parent(widget);
    GtkAllocation paralloc;
    gtk_widget_get_allocation(widget, &paralloc);
}


void fixed_handle_size_allocate_event( GtkWidget *widget, GtkAllocation *allc, gpointer data)
{
    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    gint w = allc->width; 
}

void trans_fixed_init(CAE_Object* aObject, CAE_StateBase* aState)
{
    CAE_TState<TUint32>& self = (CAE_TState<TUint32>&) *aState;
    const TUint32& parent = self.Inp("parent");
    if (~self == 0 && parent != 0) {
	GtkWidget* widget = gtk_fixed_new();
	gtk_widget_show(widget);
	GtkContainer* pwidget = GTK_CONTAINER(parent);
	gtk_container_add(pwidget, widget);
//	g_signal_connect(G_OBJECT(widget), "size_request", G_CALLBACK (fixed_handle_size_request_event), NULL);
//	g_signal_connect(G_OBJECT(widget), "size_allocate", G_CALLBACK (fixed_handle_size_allocate_event), NULL);
	self = (TUint32) widget;
    }
}
