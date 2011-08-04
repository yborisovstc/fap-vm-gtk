
#ifndef __FAPVM_WIN_H
#define __FAPVM_WIN_H

#include <fapbase.h>
#include <fapfact.h>

class CProvider: public CAE_ProviderBase
{
    public:
	CProvider();
	virtual ~CProvider();
    public:
	// From MAE_Provider
	virtual CAE_StateBase* CreateStateL(const char *aTypeUid, const char* aInstName, CAE_Object* aMan) const;
	virtual CAE_EBase* CreateObjectL(TUint32 aTypeUid) const;
	virtual CAE_EBase* CreateObjectL(const char *aName) const;
	virtual const TTransInfo* GetTransf(const char *aName) const;
	virtual void RegisterState(const TStateInfo *aInfo);
	virtual void RegisterStates(const TStateInfo **aInfos);
	virtual void RegisterTransf(const TTransInfo *aTrans);
	virtual void RegisterTransfs(const TTransInfo **aTransfs);
	virtual const CAE_Formatter* GetFormatter(int aUid) const;
	virtual void RegisterFormatter(CAE_Formatter *aForm);
	virtual CAE_ChromoBase* CreateChromo() const;
	virtual CAE_TranExBase* CreateTranEx(MCAE_LogRec* aLogger) const;
	virtual MAE_Opv* CreateViewProxy();
	virtual void GetRegisteredStates(vector<const TStateInfo*>& aInfo) const;
    private:
	// Register of transitions
	map<string, const TTransInfo*> iTransfs;
	map<string, const TStateInfo*> iStates;
};

#endif
