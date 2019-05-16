//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiObject.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UIOBJECT_H__
#define __GELIB_UI_UIOBJECT_H__

#include <string>
#include <map>
#include <GELib/Math/GeMath.h>
#include "UiEvent.h"
#include "UiClass.h"
#include "UiTypes.h"

namespace GELib
{

class IPlotter2D;
class CClipRectInfo;
class CIOStream;


class CUiSystem;
class CUiAlignment;
class CUiAnimation;
class CUiAnimationTrack;
class CUiAnimationClip;
class CUiAnimationPlayState;

enum EUiFlag
{
	UIFLAG_VISIBLE = 1 << 0,
	UIFLAG_ENABLE = 1 << 1,
	UIFLAG_DRAGGABLE = 1 << 2,
	UIFLAG_ACCEPT_DROP = 1 << 3,

	// implementation detail
	//UIFLAG_REAL_ENABLE = 1 << 10,
	UIFLAG_CAN_HIT = 1 << 11,
	UIFLAG_CLIP_CHILDREN = 1 << 12,
	UIFLAG_CLIP_SELF = 1 << 13,
	UIFLAG_CHILDREN_OUT_OF_BOUND = 1 << 14,
	UIFLAG_OVERLAP = 1 << 15,
	UIFLAG_FOCUSABLE = 1 << 16,
	UIFLAG_HAS_TIP = 1 << 17,
	//UIFLAG_MOUSE_INSIDE = 1 << 18,
	//UIFLAG_LAYOUT_DIRTY = 1 << 19,
	UIFLAG_BUBBLE_LAYOUT_DIRTY = 1 << 20,
	UIFLAG_IGNORE_LAYOUT_DIRTY = 1 << 21,
	//UIFLAG_STYLE_STATE_DIRTY = 1 << 22,
	///UIFLAG_ANIMATION_VISIBLE = 1 << 23,
	UIFLAG_REQUIRE_POST_DRAW = 1 << 24,
	UIFLAG_IGNORE_SAVE_PROPERTY = 1 << 25,
	UIFLAG_IGNORE_SAVE_CHILDREN = 1 << 26,
	UIFLAG_EMBEDED = 1 << 27,
	UIFLAG_NOTIFY_CHILDREN_SIZE_CHANGED = 1 << 28,
	//UIFLAG_DESTROYED = 1 << 31,
};

enum EUiFlag2
{
	UIFLAG2_REAL_ENABLE = 1 << 1,
	UIFLAG2_LAYOUT_DIRTY = 1 << 2,
	UIFLAG2_STYLE_STATE_DIRTY = 1 << 3,
	UIFLAG2_ANIMATION_VISIBLE = 1 << 4,
	UIFLAG2_MOUSE_INSIDE = 1 << 5,
	UIFLAG2_FILE_ROOT = 1 << 6,
	UIFLAG2_DESTROYED = 1 << 31,
};

class CUiObject
{
public:

	UICLASS_DECLARE(CUiObject);

	CUiObject(CUiSystem *system);
	virtual ~CUiObject();

	CUiSystem* GetSystem() { return m_pSystem; }

	void Destroy();
	bool IsDestroyed();

	// hierarchical relationship
	CUiObject* GetParent() { return m_pParent; }
	CUiObject* GetFirstChild() { return m_pFirstChild; }
	CUiObject* GetLastChild() { return m_pLastChild; }
	CUiObject* GetNext() { return m_pNext; }
	CUiObject* GetPrev() { return m_pPrev; }
	bool IsAncestorOf(CUiObject *u);

	void InsertChildAfter(CUiObject *c, CUiObject *after);		// if after == null, insert at front
	void InsertChildBefore(CUiObject *c, CUiObject *before);	// if before == null, insert at back
	void InsertChildFront(CUiObject *c);
	void InsertChildBack(CUiObject *c);

	int GetNumChildren();
	int GetChildIndex(CUiObject *c);	// return -1 if not child
	CUiObject* GetChildAt(int index);
	CUiObject* GetChild(const std::string &name);
	void InsertChildAt(CUiObject *c, int index);

	std::string GetRelativePath(CUiObject *u);
	CUiObject* GetByPath(const std::string &path);

	// unlink from parent
	void Remove();

	// z order
	void MoveToFront();
	void MoveToBack();
	void MoveForward();
	void MoveBackward();

	// properties
	void SetName(const char *name) { m_Name = name; }
	void SetName(const std::string &name) { m_Name = name; }
	const std::string& GetName() { return m_Name; }

	void SetPosition(float x, float y);
	void SetPosition(const CVector2 &p) { SetPosition(p.x, p.y); }
	const CVector2& GetPosition() const { return m_Position; }
	const CVector2& GetAbsPosition() const { return m_AbsPosition; }

	void SetSize(float x, float y);
	void SetSize(const CVector2 &s) { SetSize(s.x, s.y); }
	const CVector2& GetSize() const { return m_Size; }

	// depth test
	void SetDepth(float z) { m_fZ = z; }
	float GetDepth() { return m_fZ; }

	void SetColor(int r, int g, int b) { m_Color.r = (char)r; m_Color.g = (char)g; m_Color.b = (char)b; }
	void SetColor(CRGBAColor c) { m_Color = c; }
	CRGBAColor GetColor() const { return m_Color; }

	void SetOpacity(float opacity) { m_fOpacity = opacity; }
	float GetOpacity() const { return m_fOpacity; }
	float GetAbsOpacity() const { return m_fAbsOpacity; }

	// flags
	void SetFlag(EUiFlag f, bool on);
	bool GetFlag(EUiFlag f) { return (m_iFlags & f) != 0; }

	void SetVisible(bool v=true);
	bool IsVisible() { return GetFlag(UIFLAG_VISIBLE); }
	void Enable(bool enable=true);
	bool IsEnabled()  { return GetFlag(UIFLAG_ENABLE); }
	void SetDraggable(bool drag=true)  { SetFlag(UIFLAG_DRAGGABLE, drag); }
	bool IsDraggable()  { return GetFlag(UIFLAG_DRAGGABLE); }
	void SetAcceptDrop(bool drop=true)  { SetFlag(UIFLAG_ACCEPT_DROP, drop); }
	bool IsAcceptDrop()  { return GetFlag(UIFLAG_ACCEPT_DROP); }

	void SetCanHit(bool hit=true)  { SetFlag(UIFLAG_CAN_HIT, hit); }
	bool GetCanHit()  { return GetFlag(UIFLAG_CAN_HIT); }
	void SetClipChildren(bool clip=true) {	SetFlag(UIFLAG_CLIP_CHILDREN, clip); }
	bool IsClipChildren()  { return GetFlag(UIFLAG_CLIP_CHILDREN); }
	void SetClipSelf(bool clip=true) {	SetFlag(UIFLAG_CLIP_SELF, clip); }
	bool IsClipSelf()  { return GetFlag(UIFLAG_CLIP_SELF); }
	void SetChildrenOutOfBound(bool clip=true)  { SetFlag(UIFLAG_CHILDREN_OUT_OF_BOUND, clip); }
	bool IsChildrenOutOfBound()  { return GetFlag(UIFLAG_CHILDREN_OUT_OF_BOUND); }
	void SetOverlap(bool overlap=true)  { SetFlag(UIFLAG_OVERLAP, overlap); }
	bool IsOverlap()  { return GetFlag(UIFLAG_OVERLAP); }
	void SetFocusable(bool focus=true)  { SetFlag(UIFLAG_FOCUSABLE, focus); }
	bool IsFocusable()  { return GetFlag(UIFLAG_FOCUSABLE); }
	void SetHasTip(bool show=true)  { SetFlag(UIFLAG_HAS_TIP, show); }
	bool IsHasTip()  { return GetFlag(UIFLAG_HAS_TIP); }
	void SetBubbleLayoutDirty(bool dirty=true) { SetFlag(UIFLAG_BUBBLE_LAYOUT_DIRTY, dirty); }
	bool IsBubbleLayoutDirty() { return GetFlag(UIFLAG_BUBBLE_LAYOUT_DIRTY); }
	void SetIgnoreLayoutDirty(bool dirty=true) { SetFlag(UIFLAG_IGNORE_LAYOUT_DIRTY, dirty); }
	bool IsIgnoreLayoutDirty() { return GetFlag(UIFLAG_IGNORE_LAYOUT_DIRTY); }
	void SetRequirePostDraw(bool r=true) { SetFlag(UIFLAG_REQUIRE_POST_DRAW, r); }
	bool IsRequirePostDraw() { return GetFlag(UIFLAG_REQUIRE_POST_DRAW); }
	void SetIgnoreSaveProperty(bool ignore=true) { SetFlag(UIFLAG_IGNORE_SAVE_PROPERTY, ignore); }
	bool IsIgnoreSaveProperty() { return GetFlag(UIFLAG_IGNORE_SAVE_PROPERTY); }
	void SetIgnoreSaveChildren(bool ignore=true) { SetFlag(UIFLAG_IGNORE_SAVE_CHILDREN, ignore); }
	bool IsIgnoreSaveChildren() { return GetFlag(UIFLAG_IGNORE_SAVE_CHILDREN); }
	void SetEmbeded(bool e=true) { SetFlag(UIFLAG_EMBEDED, e); }
	bool IsEmbeded() { return GetFlag(UIFLAG_EMBEDED); }
	void SetNotifyChildrenSizeChanged(bool notify=true) { SetFlag(UIFLAG_NOTIFY_CHILDREN_SIZE_CHANGED, notify); }
	bool IsNotifyChildrenSizeChanged() { return GetFlag(UIFLAG_NOTIFY_CHILDREN_SIZE_CHANGED); }


	// runtime flags, not saved
	void SetFlag2(EUiFlag2 f, bool on);
	bool GetFlag2(EUiFlag2 f) { return (m_iFlags2 & f) != 0; }

	void RealEnable(bool enable=true);
	bool IsRealEnabled()  { return IsEnabled() && GetFlag2(UIFLAG2_REAL_ENABLE); }
	void SetLayoutDirty(bool dirty=true);
	bool IsLayoutDirty() { return GetFlag2(UIFLAG2_LAYOUT_DIRTY); }
	void SetStyleStateDirty(bool dirty=true) { SetFlag2(UIFLAG2_STYLE_STATE_DIRTY, dirty); }
	bool IsStyleStateDirty() { return GetFlag2(UIFLAG2_STYLE_STATE_DIRTY); }
	void SetAnimationVisible(bool visible=true) { SetFlag2(UIFLAG2_ANIMATION_VISIBLE, visible); }
	bool IsAnimationVisible() { return GetFlag2(UIFLAG2_ANIMATION_VISIBLE); }
	void SetMouseInside(bool inside=true) { SetFlag2(UIFLAG2_MOUSE_INSIDE, inside); }
	bool IsMouseInside() { return GetFlag2(UIFLAG2_MOUSE_INSIDE); }
	void SetFileRoot(bool root=true) { SetFlag2(UIFLAG2_FILE_ROOT, root); }
	bool IsFileRoot() { return GetFlag2(UIFLAG2_FILE_ROOT); }


	// user data
	void SetUserData(int d) { m_iUserData = d; }
	int GetUserData() { return m_iUserData; };
	void SetUserPointer(void *p) { m_pUserData = p; };
	void* GetUserPointer() { return m_pUserData; }
	void SetUserProperty(const std::string &key, const std::string &value);
	const std::string& GetUserProperty(const std::string &key);
	const std::string& GetInheritedUserProperty(const std::string &key);
	void RemoveUserProperty(const std::string &key);
	void GetUserPropertyKeys(std::vector<std::string> &keys);

	// ui system related
	void CaptureMouse();
	bool IsMouseEnter();
	void SetFocus();
	bool IsFocused();
	void StartDragDrop(CUiObject *icon, const CVector2 &iconOffset);

	// return this or descendant if name match, don't care type
	CUiObject* FindObject(const std::string &name);

	// return this or descendant if name and type match
	template<typename T> T* FindObject(const std::string &name);

	// return this or descendant if type match, don't care name
	template<typename T> T* FindObject();

	// convert from global coordinates to local coordinates
	CVector2 PointFromScreen(const CVector2 &p);
	// convert from local coordinates to global coordinates
	CVector2 PointToScreen(const CVector2 &p);

	// return this or descendant if hit, p is in screen coordinates
	CUiObject* HitObject(const CVector2 &p, int *hitCode=0);
	virtual bool IsHit(const CVector2 &p, int *hitCode=0);
	virtual void DrawHitArea(IPlotter2D &canvas);

	// event
	void RaiseEvent(CUiEvent &e, bool bubble=true);
	void AddEventHandler(EUiEventType et, const CUiEventHandler &h);
	void RemoveEventHandler(EUiEventType et, const CUiEventHandler &h);

	// filter is invoked before OnEvent
	void AddEventFilter(EUiEventType et, const CUiEventHandler &h);
	void RemoveEventFilter(EUiEventType et, const CUiEventHandler &h);

	// class override
	void SetClassOverride(const char *cls);
	const char * GetClassOverride();

	// alignment
	CUiAlignment* CreateAlignment();
	void DeleteAlignment();
	CUiAlignment* GetAlignment();

	// animation
	CUiAnimation* CreateAnimation();
	void DeleteAnimation();
	CUiAnimation* GetAnimation();
	void BindAnimation();
	void ApplyAnimation(float t);
	virtual void ApplyAnimationTrack(CUiAnimationTrack &tk, float t);
	void PlayAnimation(const char *name);
	void StopAnimation();
	void SetAnimationTime(float t);
	float GetAnimationTime();

	// editor support
	virtual CUiObject* GetContainer();

	// called by ui system
	void UpdateAll(float t);
	void PostUpdateAll();
	void DrawAll(IPlotter2D &canvas);
	void DebugDrawAll(IPlotter2D &canvas);
	void DrawStyleAll(IPlotter2D &canvas);

    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream);
	virtual bool PostLoad();

	virtual void ClosePopup();
	virtual void DoLayout();			// called if layout dirty
	virtual void ChooseStyleState();	// called if style state dirty

	virtual void Update(float t);
	virtual void Draw(IPlotter2D &canvas);
	virtual void PostDraw(IPlotter2D &canvas);
	virtual void DebugDraw(IPlotter2D &canvas);

protected:

	bool IsInside(const CVector2 &p);		// p is in screen coordinates
	void PlayAnimation(CUiAnimationClip *clip);
	void IntersectClipRect(const CClipRectInfo &oldClip, CClipRectInfo &newClip);
	void DeleteChildren();

	virtual void SizeChanged();
	virtual void ParentSizeChanged();
	virtual void EnabledChanged(bool enable);

	virtual void OnEvent(CUiEvent &e);

	void ExecuteEventHandler(CUiEvent &e);
	void ExecuteEventFilter(CUiEvent &e);

	CUiSystem *m_pSystem;
	CUiObject *m_pParent;
	CUiObject *m_pFirstChild;
	CUiObject *m_pLastChild;
	CUiObject *m_pNext;
	CUiObject *m_pPrev;

	std::string m_Name;
	CVector2 m_Position;
	CVector2 m_Size;
	CVector2 m_AbsPosition;
	CRGBAColor m_Color;	// alpha is ignored
	float m_fOpacity;
	float m_fAbsOpacity;
	float m_fZ;
	int m_iFlags;
	int m_iFlags2;	// runtime use, not saved

	CUiAlignment *m_pAlignment;
	CUiAnimation *m_pAnimation;
	CUiAnimationPlayState *m_pAnimationState;
	CUiEventHandlerList *m_pEventHandlers;
	char *m_pClassOverride;

	int m_iUserData;
	void *m_pUserData;
	typedef std::map<std::string, std::string> UserProperty;
	UserProperty *m_pUserProperties;
};


template<typename T> T* DynamicCast(CUiObject *u)
{
	if (u && u->GetClass()->IsA(T::s_Class))
		return (T*)u;
	else
		return 0;
}

template<typename T> T* CUiObject::FindObject(const std::string &name)
{
	if (GetClass()->IsA(T::s_Class) && m_Name == name)
		return (T*)this;
	for (CUiObject *c = m_pFirstChild; c != 0; c = c->m_pNext)
	{
		T *u = c->FindObject<T>(name);
		if (u)
			return u;
	}
	return 0;
}

// return this or descendant if type match, don't care name
template<typename T> T* CUiObject::FindObject()
{
	if (GetClass()->IsA(T::s_Class))
		return (T*)this;
	for (CUiObject *c = m_pFirstChild; c != 0; c = c->m_pNext)
	{
		T *u = c->FindObject<T>();
		if (u)
			return u;
	}
	return 0;
}

inline void CUiObject::SetFlag(EUiFlag f, bool on)
{
	if (on)
		m_iFlags |= f;
	else
		m_iFlags &= ~f;
}

inline void CUiObject::SetFlag2(EUiFlag2 f, bool on)
{
	if (on)
		m_iFlags2 |= f;
	else
		m_iFlags2 &= ~f;
}


}//GELib

#endif//__GELIB_UI_UIOBJECT_H__
