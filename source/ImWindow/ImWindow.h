// ImWindow - Single file format
#ifndef __IM_WINDOW_HEADER__
#define __IM_WINDOW_HEADER__
#include "ImwConfig.h"

namespace ImWindow {
	class ImwContainer;
	class ImwPlatformWindow;
	class ImWindowManager;

	enum EDockOrientation
	{
		E_DOCK_ORIENTATION_CENTER,
		//E_DOCK_ORIENTATION_TABBED = E_DOCK_ORIENTATION_CENTER,
		E_DOCK_ORIENTATION_TOP,
		E_DOCK_ORIENTATION_LEFT,
		E_DOCK_ORIENTATION_RIGHT,
		E_DOCK_ORIENTATION_BOTTOM,
	};

	class IMGUI_API ImwWindow
	{
		friend class ImwWindowManager;
		friend class ImwContainer;
	protected:
		ImwWindow();
		virtual					~ImwWindow();
	public:
		virtual void			OnGui() = 0;
		virtual void			OnMenu() {};

		ImU32					GetId() const;
		const char*				GetIdStr() const;

		void					Destroy();

		void					SetTitle(const char* pTitle);
		const char*				GetTitle() const;

		void					SetClosable( bool bClosable );
		bool					IsClosable() const;

		const ImVec2&			GetLastPosition() const;
		const ImVec2&			GetLastSize() const;

	protected:

		char*					m_pTitle;
		ImU32					m_iId;
		char					m_pId[11];
		bool					m_bClosable;

		ImVec2					m_oLastPosition;
		ImVec2					m_oLastSize;

		static int				s_iNextId;
	};

	typedef ImwList<ImwWindow*> ImwWindowList;

	class IMGUI_API ImwContainer
	{
		friend class ImwPlatformWindow;
	public:

		void							Dock(ImwWindow* pWindow, EDockOrientation eOrientation = E_DOCK_ORIENTATION_CENTER);
		bool							UnDock(ImwWindow* pWindow);
		void							DockToBest(ImwWindow* pWindow);

		bool							IsEmpty() const;
		bool							IsSplit() const;
		bool							HasWindowTabbed() const;
		ImwContainer*					HasWindow(const ImwWindow* pWindow);
		ImwPlatformWindow*				GetPlatformWindowParent() const;
		ImwContainer*					GetBestDocking(const ImVec2 oCursorPosInContainer, EDockOrientation& oOutOrientation, ImVec2& oOutAreaPos, ImVec2& oOutAreaSize, bool bLargeCheck);
		bool							HasUnclosableWindow() const;
	protected:
										ImwContainer(ImwContainer* pParent);
										ImwContainer(ImwPlatformWindow* pParent);
										~ImwContainer();

		void							CreateSplits();

		void							Paint();

		bool							Tab(const ImwWindow* pWindow, bool bFocused, float fStartLinePos, float fEndLinePos, float fMaxSize = -1.f);
		void							DrawTab(const char* pText, bool bFocused, ImVec2 oPos, float fStartLinePos, float fEndLinePos, float fMaxSize = -1.f, ImVec2* pSizeOut = NULL);

		ImwContainer*					m_pParent;
		ImwPlatformWindow*				m_pParentWindow;
		ImwWindowList					m_lWindows;
		ImwContainer*					m_pSplits[2];

		float							m_fSplitRatio;
		bool							m_bVerticalSplit;
		int								m_iActiveWindow;

		bool							m_bIsDrag;

		ImVec2							m_oLastPosition;
		ImVec2							m_oLastSize;

		static const float				c_fTabHeight;
	};

	class IMGUI_API ImwPlatformWindow
	{
		friend class ImwWindowManager;
	public:
											ImwPlatformWindow(bool bMainWindow, bool bIsDragWindow, bool bCreateState);
		virtual								~ImwPlatformWindow();

		virtual bool						Init(ImwPlatformWindow* pParent);

		virtual const ImVec2&				GetPosition() const;
		virtual const ImVec2&				GetSize() const;

		virtual void						Show();
		virtual void						Hide();
		virtual void						SetSize(int iWidth, int iHeight);
		virtual void						SetPosition(int iX, int iY);
		virtual void						SetTitle(const char* pTtile);

		bool								IsMain();

		void								Dock(ImwWindow* pWindow);
		bool								UnDock(ImwWindow* pWindow);

		ImwContainer*						GetContainer();
		ImwContainer*						HasWindow(ImwWindow* pWindow);
		bool								IsStateSet();
	protected:
		void								SetState();
		void								RestoreState();
		void								OnLoseFocus();
		virtual void						PreUpdate();
		virtual void						Paint();
		virtual void						Destroy();
		virtual void						StartDrag();
		virtual void						StopDrag();
		virtual bool						IsDraging();

		void								PaintContainer();
		void								OnClose();

		bool								m_bMain;
		bool								m_bIsDragWindow;
		ImwContainer*						m_pContainer;
		void*								m_pState;
		void*								m_pPreviousState;
	};

	typedef ImwList<ImwPlatformWindow*> ImwPlatformWindowList;

	class IMGUI_API ImwWindowManager
	{
		friend class ImwWindow;
		friend class ImwPlatformWindow;
		friend class ImwContainer;

		enum EPlatformWindowAction
		{
			E_PLATFORM_WINDOW_ACTION_DESTROY			= 1,
			E_PLATFORM_WINDOW_ACTION_SHOW			= 2,
			E_PLATFORM_WINDOW_ACTION_HIDE			= 4,
			E_PLATFORM_WINDOW_ACTION_SET_POSITION	= 8,
			E_PLATFORM_WINDOW_ACTION_SET_SIZE		= 16,
		};

		struct PlatformWindowAction
		{
			ImwPlatformWindow*		m_pPlatformWindow;
			unsigned int			m_iFlags;
			ImVec2					m_oPosition;
			ImVec2					m_oSize;
		};

		struct DockAction
		{
			ImwWindow*				m_pWindow;
			// Is Dock or Float
			bool					m_bFloat;
			//For Docking
			ImwWindow*				m_pWith;
			EDockOrientation		m_eOrientation;
			ImwPlatformWindow*		m_pToPlatformWindow;
			ImwContainer*			m_pToContainer;
			//For Floating
			ImVec2					m_oPosition;
			ImVec2					m_oSize;
		};

		struct DrawWindowAreaAction
		{
			DrawWindowAreaAction( ImwPlatformWindow* pWindow, const ImVec2& oRectPos, const ImVec2& oRectSize, const ImColor& oColor );
			ImwPlatformWindow*		m_pWindow;
			ImVec2					m_oRectPos;
			ImVec2					m_oRectSize;
			ImColor					m_oColor;
		};
	public:
		struct Config
		{
			Config();
			float					m_fDragMarginRatio;
			float					m_fDragMarginSizeRatio;
			ImColor					m_oHightlightAreaColor;
		};
	public:
		ImwWindowManager();
		virtual								~ImwWindowManager();

		bool								Init();
		bool								Run();
		void								Exit();

		ImwPlatformWindow*					GetMainPlatformWindow();
		Config&								GetConfig();

		void								SetMainTitle(const char* pTitle);

		void								Dock(ImwWindow* pWindow, EDockOrientation eOrientation = E_DOCK_ORIENTATION_CENTER, ImwPlatformWindow* pToPlatformWindow = NULL);
		void								DockTo(ImwWindow* pWindow, EDockOrientation eOrientation = E_DOCK_ORIENTATION_CENTER, ImwContainer* pContainer = NULL);
		void								DockWith(ImwWindow* pWindow, ImwWindow* pWithWindow, EDockOrientation eOrientation = E_DOCK_ORIENTATION_CENTER);
		void								Float(ImwWindow* pWindow, const ImVec2& oPosition = ImVec2(-1, -1), const ImVec2& oSize = ImVec2(-1, -1));

		const ImwWindowList&				GetWindowList() const;
		ImwPlatformWindow*					GetCurrentPlatformWindow();
		ImwPlatformWindow*					GetWindowParent(ImwWindow* pWindow);

	protected:
		//To override for use multi window mode
		virtual bool						CanCreateMultipleWindow();
		virtual ImwPlatformWindow*			CreatePlatformWindow(bool bMain, ImwPlatformWindow* pParent, bool bDragWindow);
		virtual void						InternalRun();
		virtual ImVec2						GetCursorPos();
		virtual bool						IsLeftClickDown();

		void								AddWindow(ImwWindow* pWindow);
		void								RemoveWindow(ImwWindow* pWindow);
		void								DestroyWindow(ImwWindow* pWindow);

		void								UnDock(ImwWindow* pWindow);
		void								InternalDock(ImwWindow* pWindow, EDockOrientation eOrientation, ImwPlatformWindow* pToPlatformWindow);
		void								InternalDockTo(ImwWindow* pWindow, EDockOrientation eOrientation, ImwContainer* pToContainer);
		void								InternalDockWith(ImwWindow* pWindow, ImwWindow* pWithWindow, EDockOrientation eOrientation);
		void								InternalFloat(ImwWindow* pWindow, ImVec2 oPosition, ImVec2 oSize);
		void								InternalUnDock(ImwWindow* pWindow);

		void								OnClosePlatformWindow(ImwPlatformWindow* pWindow);

		void								DrawWindowArea( ImwPlatformWindow* pWindow, const ImVec2& oPos, const ImVec2& oSize, const ImColor& oColor );

		void								PreUpdate();
		void								Update();
		void								UpdatePlatformwWindowActions();
		void								UpdateDockActions();
		void								UpdateOrphans();

		void								Paint(ImwPlatformWindow* pWindow);

		void								StartDragWindow(ImwWindow* pWindow, ImVec2 oOffset);
		void								StopDragWindow();
		void								UpdateDragWindow();
		ImwContainer*						GetBestDocking(ImwPlatformWindow* pPlatformWindow, const ImVec2 oCursorPos, EDockOrientation& oOutOrientation, ImVec2& oOutAreaPos, ImVec2& oOutAreaSize, bool bLargeCheck);
		
		Config								m_oConfig;
		ImwPlatformWindow*					m_pMainPlatformWindow;
		ImwPlatformWindowList				m_lPlatformWindows;
		ImwPlatformWindow*					m_pDragPlatformWindow;
		ImwWindowList						m_lWindows;
		ImwWindowList						m_lOrphanWindows;
		ImwWindowList						m_lToDestroyWindows;
		ImwPlatformWindowList				m_lToDestroyPlatformWindows;
		ImwList<PlatformWindowAction*>		m_lPlatformWindowActions;
		ImwList<DockAction*>				m_lDockActions;
		ImwList<DrawWindowAreaAction>		m_lDrawWindowAreas;

		ImwPlatformWindow*					m_pCurrentPlatformWindow;
		ImwWindow*							m_pDraggedWindow;

		ImVec2								m_oDragPreviewOffset;

		// Static
	public:
		static ImwWindowManager*			GetInstance();
	protected:
		static ImwWindowManager*			s_pInstance;
	};

}
#endif // __IM_WINDOW_HEADER__
