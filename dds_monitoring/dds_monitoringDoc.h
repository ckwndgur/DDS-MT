
// dds_monitoringDoc.h : interface of the Cdds_monitoringDoc class
//


#pragma once


class Cdds_monitoringDoc : public CDocument
{
protected: // create from serialization only
	Cdds_monitoringDoc();
	DECLARE_DYNCREATE(Cdds_monitoringDoc)

// Attributes
public:
	
private:
	
// Operations
public:
	CView* SwitchToView(CView* pNewView);
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~Cdds_monitoringDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
