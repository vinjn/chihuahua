#include "ImWindow.h"

namespace ImWindow {
	int ImwWindow::s_iNextId = 0;

	ImwWindow::ImwWindow()
	{
		m_pTitle = NULL;
		m_bClosable  = true;
		m_iId = s_iNextId++;
	
		//Write Id to string
		int iIndex = 0;
		int iNumber = m_iId;
		do
		{
			m_pId[iIndex++] = iNumber % 10 + '0';
		}
		while ((iNumber /= 10) > 0 && iIndex <= 10);
		m_pId[iIndex] = '\0';

		ImwWindowManager::GetInstance()->AddWindow(this);
	}

	ImwWindow::~ImwWindow()
	{
		ImwWindowManager::GetInstance()->RemoveWindow(this);
		ImwSafeFree(m_pTitle);
	}

	ImU32 ImwWindow::GetId() const
	{
		return m_iId;
	}

	const char* ImwWindow::GetIdStr() const
	{
		return m_pId;
	}

	void ImwWindow::Destroy()
	{
		ImwWindowManager::GetInstance()->DestroyWindow(this);
	}

	void ImwWindow::SetTitle(const char* pTitle)
	{
		ImwSafeFree(m_pTitle);
		if (NULL != pTitle)
		{
			size_t iLen = strlen(pTitle) + 1;
			m_pTitle = (char*)ImwMalloc(sizeof(char) * iLen);
			strcpy(m_pTitle, pTitle);
		}
	}

	const char* ImwWindow::GetTitle() const
	{
		return m_pTitle;
	}

	void ImwWindow::SetClosable( bool bClosable )
	{
		m_bClosable = bClosable;
	}

	bool ImwWindow::IsClosable() const
	{
		return m_bClosable;
	}

	const ImVec2& ImwWindow::GetLastPosition() const
	{
		return m_oLastPosition;
	}

	const ImVec2& ImwWindow::GetLastSize() const
	{
		return m_oLastSize;
	}

	const float ImwContainer::c_fTabHeight = 25.f;
	
	ImwContainer::ImwContainer(ImwContainer* pParent)
	{
		IM_ASSERT(NULL != pParent);
		m_pSplits[0] = NULL;
		m_pSplits[1] = NULL;
		m_bVerticalSplit = false;
		m_iActiveWindow = 0;
		m_fSplitRatio = 0.5f;
		m_bIsDrag = false;
		m_pParent = pParent;
		m_pParentWindow = (NULL != pParent) ? pParent->m_pParentWindow : NULL;
	}

	ImwContainer::ImwContainer(ImwPlatformWindow* pParent)
	{
		IM_ASSERT(NULL != pParent);
		m_pSplits[0] = NULL;
		m_pSplits[1] = NULL;
		m_bVerticalSplit = false;
		m_iActiveWindow = 0;
		m_fSplitRatio = 0.5f;
		m_bIsDrag = false;
		m_pParent = NULL;
		m_pParentWindow = pParent;
	}

	ImwContainer::~ImwContainer()
	{
		while ( m_lWindows.begin() != m_lWindows.end() )
		{
			ImwWindowManager::GetInstance()->RemoveWindow(*m_lWindows.begin());
			delete *m_lWindows.begin();
			m_lWindows.erase(m_lWindows.begin());
		}

		ImwSafeDelete(m_pSplits[0]);
		ImwSafeDelete(m_pSplits[1]);
	}

	void ImwContainer::CreateSplits()
	{
		m_pSplits[0] = new ImwContainer(this);
		m_pSplits[1] = new ImwContainer(this);
	}

	void ImwContainer::Dock(ImwWindow* pWindow, EDockOrientation eOrientation)
	{
		IM_ASSERT(NULL != pWindow);

		if ( NULL != pWindow )
		{
			IM_ASSERT(eOrientation != E_DOCK_ORIENTATION_CENTER || !IsSplit());

			if ( !IsSplit() )
			{
				if (m_lWindows.begin() ==  m_lWindows.end())
				{
					eOrientation = E_DOCK_ORIENTATION_CENTER;
				}

				switch (eOrientation)
				{
				case E_DOCK_ORIENTATION_CENTER:
					{
						m_lWindows.push_back(pWindow);
						m_iActiveWindow = (int)m_lWindows.size() - 1;
					}
					break;
				case E_DOCK_ORIENTATION_TOP:
					{
						m_bVerticalSplit = true;
						CreateSplits();
						m_pSplits[0]->Dock(pWindow);
						m_pSplits[1]->m_lWindows.insert(m_pSplits[1]->m_lWindows.begin(), m_lWindows.begin(), m_lWindows.end());
						m_lWindows.clear();
						m_iActiveWindow = 0;
					}
					break;
				case E_DOCK_ORIENTATION_LEFT:
					{
						m_bVerticalSplit = false;
						CreateSplits();
						m_pSplits[0]->Dock(pWindow);
						m_pSplits[1]->m_lWindows.insert(m_pSplits[1]->m_lWindows.begin(), m_lWindows.begin(), m_lWindows.end());
						m_lWindows.clear();
						m_iActiveWindow = 0;
					}
					break;
				case E_DOCK_ORIENTATION_RIGHT:
					{
						m_bVerticalSplit = false;
						CreateSplits();
						m_pSplits[0]->m_lWindows.insert(m_pSplits[0]->m_lWindows.begin(), m_lWindows.begin(), m_lWindows.end());
						m_pSplits[1]->Dock(pWindow);
						m_lWindows.clear();
						m_iActiveWindow = 0;
					}
					break;
				case E_DOCK_ORIENTATION_BOTTOM:
					{
						m_bVerticalSplit = true;
						CreateSplits();
						m_pSplits[0]->m_lWindows.insert(m_pSplits[0]->m_lWindows.begin(), m_lWindows.begin(), m_lWindows.end());
						m_pSplits[1]->Dock(pWindow);
						m_lWindows.clear();
						m_iActiveWindow = 0;
					}
					break;
				}
			}
			else
			{
				switch (eOrientation)
				{
				case E_DOCK_ORIENTATION_CENTER:
					IM_ASSERT(false);
					break;
				case E_DOCK_ORIENTATION_TOP:
					{
						ImwContainer* pSplit0 = m_pSplits[0];
						ImwContainer* pSplit1 = m_pSplits[1];
						CreateSplits();
						m_pSplits[0]->m_lWindows.push_back(pWindow);
						m_pSplits[1]->m_bVerticalSplit = m_bVerticalSplit;
						m_pSplits[1]->m_fSplitRatio = m_fSplitRatio;
						m_pSplits[1]->m_pSplits[0] = pSplit0;
						m_pSplits[1]->m_pSplits[1] = pSplit1;
						m_pSplits[1]->m_pSplits[0]->m_pParent = m_pSplits[1];
						m_pSplits[1]->m_pSplits[1]->m_pParent = m_pSplits[1];
						m_fSplitRatio = ImwWindowManager::GetInstance()->GetConfig().m_fDragMarginSizeRatio;
						m_bVerticalSplit = true;
					}
					break;
				case E_DOCK_ORIENTATION_LEFT:
					{
						ImwContainer* pSplit0 = m_pSplits[0];
						ImwContainer* pSplit1 = m_pSplits[1];
						CreateSplits();
						m_pSplits[0]->m_lWindows.push_back(pWindow);
						m_pSplits[1]->m_bVerticalSplit = m_bVerticalSplit;
						m_pSplits[1]->m_fSplitRatio = m_fSplitRatio;
						m_pSplits[1]->m_pSplits[0] = pSplit0;
						m_pSplits[1]->m_pSplits[1] = pSplit1;
						m_pSplits[1]->m_pSplits[0]->m_pParent = m_pSplits[1];
						m_pSplits[1]->m_pSplits[1]->m_pParent = m_pSplits[1];
						m_fSplitRatio = ImwWindowManager::GetInstance()->GetConfig().m_fDragMarginSizeRatio;
						m_bVerticalSplit = false;
					}
					break;
				case E_DOCK_ORIENTATION_RIGHT:
					{
						ImwContainer* pSplit0 = m_pSplits[0];
						ImwContainer* pSplit1 = m_pSplits[1];
						CreateSplits();
						m_pSplits[1]->m_lWindows.push_back(pWindow);
						m_pSplits[0]->m_bVerticalSplit = m_bVerticalSplit;
						m_pSplits[0]->m_fSplitRatio = m_fSplitRatio;
						m_pSplits[0]->m_pSplits[0] = pSplit0;
						m_pSplits[0]->m_pSplits[1] = pSplit1;
						m_pSplits[0]->m_pSplits[0]->m_pParent = m_pSplits[0];
						m_pSplits[0]->m_pSplits[1]->m_pParent = m_pSplits[0];
						m_fSplitRatio = 1.f - ImwWindowManager::GetInstance()->GetConfig().m_fDragMarginSizeRatio;
						m_bVerticalSplit = false;
					}
					break;
				case E_DOCK_ORIENTATION_BOTTOM:
					{
						ImwContainer* pSplit0 = m_pSplits[0];
						ImwContainer* pSplit1 = m_pSplits[1];
						CreateSplits();
						m_pSplits[1]->m_lWindows.push_back(pWindow);
						m_pSplits[0]->m_bVerticalSplit = m_bVerticalSplit;
						m_pSplits[0]->m_fSplitRatio = m_fSplitRatio;
						m_pSplits[0]->m_pSplits[0] = pSplit0;
						m_pSplits[0]->m_pSplits[1] = pSplit1;
						m_pSplits[0]->m_pSplits[0]->m_pParent = m_pSplits[0];
						m_pSplits[0]->m_pSplits[1]->m_pParent = m_pSplits[0];
						m_fSplitRatio = 1.f - ImwWindowManager::GetInstance()->GetConfig().m_fDragMarginSizeRatio;
						m_bVerticalSplit = true;
					}
					break;
				}
			}
		}
	}

	bool ImwContainer::UnDock(ImwWindow* pWindow)
	{
		if (std::find(m_lWindows.begin(), m_lWindows.end(), pWindow) != m_lWindows.end())
		{
			m_lWindows.remove(pWindow);
			if (m_iActiveWindow >= (int)m_lWindows.size())
			{
				m_iActiveWindow = (int)m_lWindows.size() - 1;
			}
			return true;
		}
		if (NULL != m_pSplits[0] && NULL != m_pSplits[1])
		{
			if ( m_pSplits[0]->UnDock(pWindow) )
			{
				if (m_pSplits[0]->IsEmpty())
				{
					if (m_pSplits[1]->IsSplit())
					{
						ImwContainer* pSplit = m_pSplits[1];
						m_bVerticalSplit = pSplit->m_bVerticalSplit;
						ImwSafeDelete(m_pSplits[0]);
						m_pSplits[0] = pSplit->m_pSplits[0];
						m_pSplits[1] = pSplit->m_pSplits[1];
						pSplit->m_pSplits[0] = NULL;
						pSplit->m_pSplits[1] = NULL;
						m_pSplits[0]->m_pParent = this;
						m_pSplits[1]->m_pParent = this;
						ImwSafeDelete(pSplit);
					}
					else
					{
						m_lWindows.insert(m_lWindows.end(), m_pSplits[1]->m_lWindows.begin(), m_pSplits[1]->m_lWindows.end());
						m_pSplits[1]->m_lWindows.clear();
						m_pSplits[1]->m_iActiveWindow = 0;
						ImwSafeDelete(m_pSplits[0]);
						ImwSafeDelete(m_pSplits[1]);
					}
				}
				return true;
			}

			if (m_pSplits[1]->UnDock(pWindow))
			{
				if (m_pSplits[1]->IsEmpty())
				{
					if (m_pSplits[0]->IsSplit())
					{
						ImwContainer* pSplit = m_pSplits[0];
						m_bVerticalSplit = pSplit->m_bVerticalSplit;
						ImwSafeDelete(m_pSplits[1]);
						m_pSplits[0] = pSplit->m_pSplits[0];
						m_pSplits[1] = pSplit->m_pSplits[1];
						pSplit->m_pSplits[0] = NULL;
						pSplit->m_pSplits[1] = NULL;
						m_pSplits[0]->m_pParent = this;
						m_pSplits[1]->m_pParent = this;
						ImwSafeDelete(pSplit);
					}
					else
					{
						m_lWindows.insert(m_lWindows.end(), m_pSplits[0]->m_lWindows.begin(), m_pSplits[0]->m_lWindows.end());
						m_pSplits[0]->m_lWindows.clear();
						m_pSplits[0]->m_iActiveWindow = 0;
						ImwSafeDelete(m_pSplits[0]);
						ImwSafeDelete(m_pSplits[1]);
					}
				}
				return true;
			}
		}

		return false;
	}

	void ImwContainer::DockToBest(ImwWindow* pWindow)
	{
		if (IsSplit())
		{
			if (m_fSplitRatio < 0.5f)
			{
				m_pSplits[0]->DockToBest(pWindow);
			}
			else
			{
				m_pSplits[1]->DockToBest(pWindow);
			}
		}
		else
		{
			Dock(pWindow);
		}
	}

	bool ImwContainer::IsEmpty() const
	{
		//IM_ASSERT(IsSplit() != HasWindowTabbed());
		return !(IsSplit() || HasWindowTabbed());
	}

	bool ImwContainer::IsSplit() const
	{
		IM_ASSERT((NULL == m_pSplits[0]) == (NULL == m_pSplits[1]));
		return (NULL != m_pSplits[0] && NULL != m_pSplits[1]);
	}

	bool ImwContainer::HasWindowTabbed() const
	{
		return m_lWindows.size() > 0;
	}

	ImwContainer* ImwContainer::HasWindow( const ImwWindow* pWindow)
	{
		if (std::find(m_lWindows.begin(), m_lWindows.end(), pWindow) != m_lWindows.end())
		{
			return this;
		}
		else
		{
			if (NULL != m_pSplits[0])
			{
				ImwContainer* pContainer = m_pSplits[0]->HasWindow(pWindow);
				if (NULL != pContainer)
				{
					return pContainer;
				}
			}
			if (NULL != m_pSplits[1])
			{
				ImwContainer* pContainer = m_pSplits[1]->HasWindow(pWindow);
				if (NULL != pContainer)
				{
					return pContainer;
				}
			}
		}
		return NULL;
	}

	ImwPlatformWindow* ImwContainer::GetPlatformWindowParent() const
	{
		return m_pParentWindow;
	}

	void ImwContainer::Paint(/* int iX, int iY, int iWidth, int iHeight */)
	{
		ImwWindowManager* pWindowManager = ImwWindowManager::GetInstance();
		ImGuiWindow* pWindow = ImGui::GetCurrentWindow();
		const ImGuiStyle& oStyle = ImGui::GetStyle();
		ImDrawList* pDrawList = ImGui::GetWindowDrawList();

		const ImVec2 oPos = ImGui::GetWindowPos();
		const ImVec2 oSize = ImGui::GetWindowSize();
		const ImVec2 oMin = ImVec2(oPos.x + 1, oPos.y + 1);
		const ImVec2 oMax = ImVec2(oPos.x + oSize.x - 2, oPos.y + oSize.y - 2);

		m_oLastPosition = oPos;
		m_oLastSize = oSize;

		const int iSeparatorHalfSize = 2;
		const int iSeparatorSize = iSeparatorHalfSize * 2;

		if (IsSplit())
		{
			if (m_bVerticalSplit)
			{
				float iFirstHeight = oSize.y * m_fSplitRatio - iSeparatorHalfSize - pWindow->WindowPadding.x;
				//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
				//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
				//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
				/*ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0,0));
				ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0);*/
			

				ImGui::BeginChild("Split1", ImVec2(0, iFirstHeight), false, ImGuiWindowFlags_NoScrollbar);
				//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,4));
				m_pSplits[0]->Paint(/*iX, iY, iWidth, iFirstHeight*/);
				//ImGui::PopStyleVar(1);
				ImGui::EndChild();


				ImRect oSeparatorRect( 0, iFirstHeight, oSize.x, iFirstHeight + iSeparatorSize);
				ImGui::Button("",oSeparatorRect.GetSize());
				if (ImGui::IsItemHovered() || m_bIsDrag)
				{
					 ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
				}
				if (ImGui::IsItemActive())
				{
					if (!m_bIsDrag)
					{
						m_bIsDrag = true;
					}
					m_fSplitRatio += ImGui::GetIO().MouseDelta.y / oSize.y;
					m_fSplitRatio = ImClamp( m_fSplitRatio, 0.05f, 0.95f );

				}
				else
				{
					m_bIsDrag = false;
				}

				ImGui::BeginChild("Split2", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
				//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,4));
				m_pSplits[1]->Paint(/*iX, iY + iFirstHeight, iWidth, iSecondHeight*/);
				//ImGui::PopStyleVar(1);
				ImGui::EndChild();

				//ImGui::PopStyleVar(1);
			}
			else
			{
				float iFirstWidth = oSize.x * m_fSplitRatio - iSeparatorHalfSize - pWindow->WindowPadding.y;
				ImGui::BeginChild("Split1", ImVec2(iFirstWidth, 0), false, ImGuiWindowFlags_NoScrollbar);
				//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,4));
				m_pSplits[0]->Paint();
				//ImGui::PopStyleVar(1);
				ImGui::EndChild();

				ImGui::SameLine();

				ImRect oSeparatorRect( iFirstWidth, 0, iFirstWidth + iSeparatorSize, oSize.y);
				ImGui::Button("",oSeparatorRect.GetSize());
				if (ImGui::IsItemHovered() || m_bIsDrag)
				{
					ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
				}
				if (ImGui::IsItemActive())
				{
					if (!m_bIsDrag)
					{
						m_bIsDrag = true;
					}
				
					m_fSplitRatio += ImGui::GetIO().MouseDelta.x / oSize.x;
					m_fSplitRatio = ImClamp( m_fSplitRatio, 0.05f, 0.95f );
				}
				else
				{
					m_bIsDrag = false;
				}

				ImGui::SameLine();

				ImGui::BeginChild("Split2", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
				//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,4));
				m_pSplits[1]->Paint();
				//ImGui::PopStyleVar(1);
				ImGui::EndChild();

				//ImGui::PopStyleVar(1);
			}
		}
		else if (HasWindowTabbed())
		{
			ImGui::InvisibleButton("TabListButton", ImVec2(16, 16));
			ImGui::SameLine();

			if (ImGui::BeginPopupContextItem("TabListMenu", 0))
			{
				int iIndex = 0;
				for (ImwWindowList::const_iterator itWindow = m_lWindows.begin(); itWindow != m_lWindows.end(); ++itWindow, ++iIndex)
				{
					if (ImGui::Selectable((*itWindow)->GetTitle()))
					{
						m_iActiveWindow = iIndex;
					}
				}
				ImGui::EndPopup();
			}

			ImColor oLinesColor = ImColor(160, 160, 160, 255);
			if (ImGui::IsItemHovered())
			{
				oLinesColor = ImColor(255, 255, 255, 255);
			}
			ImVec2 oButtonMin = ImGui::GetItemRectMin();
			ImVec2 oButtonMax = ImGui::GetItemRectMax();
			ImVec2 oButtonSize = ImVec2(oButtonMax.x - oButtonMin.x, oButtonMax.y - oButtonMin.y);
			pDrawList->AddLine(
				ImVec2(oButtonMin.x + 1, oButtonMin.y + oButtonSize.y / 2),
				ImVec2(oButtonMax.x - 1, oButtonMin.y + oButtonSize.y / 2),
				oLinesColor);

			pDrawList->AddLine(
				ImVec2(oButtonMin.x + 1, oButtonMin.y + oButtonSize.y / 2 - 4),
				ImVec2(oButtonMax.x - 1, oButtonMin.y + oButtonSize.y / 2 - 4),
				oLinesColor);

			pDrawList->AddLine(
				ImVec2(oButtonMin.x + 1, oButtonMin.y + oButtonSize.y / 2 + 4),
				ImVec2(oButtonMax.x - 1, oButtonMin.y + oButtonSize.y / 2 + 4),
				oLinesColor);

			pDrawList->ChannelsSplit(2);

			//Tabs
			int iIndex = 0;
			int iNewActive = m_iActiveWindow;
			int iSize = (int)m_lWindows.size();
			float fMaxTabSize = (oSize.x - 50.f) / iSize;
			for (ImwWindowList::iterator it = m_lWindows.begin(); it != m_lWindows.end(); ++it)
			{
				ImGui::PushID(iIndex);

				bool bSelected = iIndex == m_iActiveWindow;
				if (Tab(*it, bSelected, oMin.x, oMax.x, fMaxTabSize))
				{
					iNewActive = iIndex;
				}

				if (iIndex < (iSize - 1))
				{
					ImGui::SameLine();
				}

				if (ImGui::IsItemActive())
				{
					if (ImGui::IsMouseDragging())
					{
						ImVec2 oOffset = ImVec2(oPos.x - ImGui::GetIO().MousePos.x, oPos.y - ImGui::GetIO().MousePos.y);
						pWindowManager->StartDragWindow(*it, oOffset);
					}
				}

				if (ImGui::BeginPopupContextItem("TabMenu"))
				{
					if ((*it)->IsClosable() && ImGui::Selectable("Close"))
					{
						(*it)->Destroy();
					}

					if (ImGui::BeginMenu("Dock to"))
					{
						int iIndex = 0;

						if (pWindowManager->GetMainPlatformWindow()->GetContainer()->IsEmpty())
						{
							ImGui::PushID(0);
							if (ImGui::Selectable("Main")) pWindowManager->Dock((*it));
							ImGui::PopID();
							++iIndex;
						}
						const ImwWindowList& lWindows = pWindowManager->GetWindowList();
						for (ImwWindowList::const_iterator itWindow = lWindows.begin(); itWindow != lWindows.end(); ++itWindow)
						{
							if ((*it) != (*itWindow))
							{
								ImGui::PushID(iIndex);
								if (ImGui::BeginMenu((*itWindow)->GetTitle()))
								{
									bool bHovered = false;
									ImwPlatformWindow* pPlatformWindow = pWindowManager->GetWindowParent((*itWindow));
								
									ImVec2 oLastWinPos = (*itWindow)->GetLastPosition();
									ImVec2 oLastWinSize = (*itWindow)->GetLastSize();
								
									ImGui::PushID(0);
									if (ImGui::Selectable("Tab")) pWindowManager->DockWith((*it), (*itWindow), E_DOCK_ORIENTATION_CENTER);
									if (ImGui::IsItemHovered() && NULL != pPlatformWindow)
									{
										bHovered = true;
										pWindowManager->DrawWindowArea(pPlatformWindow, oLastWinPos, oLastWinSize, ImColor(0.f, 0.5f, 1.f, 0.5f));
									}
									ImGui::PopID();

									ImGui::PushID(1);
									if (ImGui::Selectable("Top")) pWindowManager->DockWith((*it), (*itWindow), E_DOCK_ORIENTATION_TOP);
									if (ImGui::IsItemHovered() && NULL != pPlatformWindow)
									{
										bHovered = true;
										pWindowManager->DrawWindowArea(pPlatformWindow, oLastWinPos, ImVec2(oLastWinSize.x, oLastWinSize.y / 2.f), ImColor(0.f, 0.5f, 1.f, 0.5f));
									}
									ImGui::PopID();

									ImGui::PushID(2);
									if (ImGui::Selectable("Left")) pWindowManager->DockWith((*it), (*itWindow), E_DOCK_ORIENTATION_LEFT);
									if (ImGui::IsItemHovered() && NULL != pPlatformWindow)
									{
										bHovered = true;
										pWindowManager->DrawWindowArea(pPlatformWindow, oLastWinPos, ImVec2(oLastWinSize.x / 2.f, oLastWinSize.y), ImColor(0.f, 0.5f, 1.f, 0.5f));
									}
									ImGui::PopID();

									ImGui::PushID(3);
									if (ImGui::Selectable("Right")) pWindowManager->DockWith((*it), (*itWindow), E_DOCK_ORIENTATION_RIGHT);
									if (ImGui::IsItemHovered() && NULL != pPlatformWindow)
									{
										bHovered = true;
										pWindowManager->DrawWindowArea(pPlatformWindow, ImVec2(oLastWinPos.x + oLastWinSize.x / 2.f, oLastWinPos.y), ImVec2(oLastWinSize.x / 2.f, oLastWinSize.y), ImColor(0.f, 0.5f, 1.f, 0.5f));
									}
									ImGui::PopID();

									ImGui::PushID(4);
									if (ImGui::Selectable("Bottom")) pWindowManager->DockWith((*it), (*itWindow), E_DOCK_ORIENTATION_BOTTOM);
									if (ImGui::IsItemHovered() && NULL != pPlatformWindow)
									{
										bHovered = true;
										pWindowManager->DrawWindowArea(pPlatformWindow, ImVec2(oLastWinPos.x, oLastWinPos.y + oLastWinSize.y / 2.f), ImVec2(oLastWinSize.x, oLastWinSize.y / 2.f), ImColor(0.f, 0.5f, 1.f, 0.5f));
									}
									ImGui::PopID();

									if (!bHovered)
									{
										if ( NULL != pPlatformWindow )
										{
											pWindowManager->DrawWindowArea(pPlatformWindow, oLastWinPos, oLastWinSize, ImColor(0.f, 0.5f, 1.f, 0.5f));
										}
									}

									ImGui::EndMenu();
								}
								ImGui::PopID();
							}
							++iIndex;
						}
					
						ImGui::EndMenu();
					}
					if (ImGui::Selectable("Float")) pWindowManager->Float((*it));
					ImGui::EndPopup();
				}

				ImGui::PopID();

				++iIndex;
			}
			m_iActiveWindow = iNewActive;
			pDrawList->ChannelsMerge();


			ImwWindowList::iterator itActiveWindow = m_lWindows.begin();
			std::advance(itActiveWindow, m_iActiveWindow);
		
			//Draw active
			IM_ASSERT(itActiveWindow != m_lWindows.end());
			if (itActiveWindow != m_lWindows.end())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, oStyle.WindowPadding);
				//ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(59, 59, 59, 255));
				ImGui::BeginChild((*itActiveWindow)->GetId(), ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
			

				ImVec2 oWinPos = ImGui::GetWindowPos();
				ImVec2 oWinSize = ImGui::GetWindowSize();

				for (ImwWindowList::iterator it = m_lWindows.begin(); it != m_lWindows.end(); ++it)
				{
					(*it)->m_oLastPosition = oWinPos;
					(*it)->m_oLastSize = oWinSize;
				}
				(*itActiveWindow)->OnGui();
			
				ImGui::EndChild();
				//ImGui::PopStyleColor(1);
				ImGui::PopStyleVar(1);
			}
		}
		else
		{
			// This case can happened only where it's main container
			IM_ASSERT(m_pParent == NULL);
		}
	}

	bool ImwContainer::Tab(const ImwWindow* pWindow, bool bFocused, float fStartLinePos, float fEndLinePos, float fMaxSize)
	{

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		//ImVec2 oRectMin = ImGui::GetItemBoxMin();
		//ImVec2 oRectMax = ImGui::GetItemBoxMax();

		ImVec2 oTabSize;
		DrawTab(pWindow->GetTitle(), bFocused, window->DC.CursorPos, fStartLinePos, fEndLinePos, fMaxSize, &oTabSize);

		return ImGui::InvisibleButton(pWindow->GetIdStr(), oTabSize);
	}

	//bool ImwContainer::DrawTab(const ImwWindow* pWindow, bool bFocused, ImVec2 oPos, float fMaxSize, ImVec2* pSizeOut)
	void ImwContainer::DrawTab(const char* pText, bool bFocused, ImVec2 oPos, float fStartLinePos, float fEndLinePos, float fMaxSize, ImVec2* pSizeOut)
	{
		ImDrawList* pDrawList = ImGui::GetWindowDrawList();

		//Calculate text size
		const ImVec2 oTextSize = ImGui::CalcTextSize(pText);

		//Clamp fMaxSize to a minimum for avoid glitch
		if (fMaxSize < 30.f)
		{
			fMaxSize = 30.f;
		}

		//Calculate tab size
		ImVec2 oTabSize(oTextSize.x + 15, c_fTabHeight);
		if (fMaxSize != 1.f && oTabSize.x > fMaxSize)
		{
			oTabSize.x = fMaxSize;
		}

		if (pSizeOut != NULL)
		{
			*pSizeOut = oTabSize;
		}

		ImColor oNormalTab(50, 50, 50, 255); // normal
		ImColor oSelectedTab(37, 37, 37, 255); // selected
		ImColor oBorderColor(72, 72, 72, 255); // border

		ImVec2 oRectMin = oPos;
		ImVec2 oRectMax = ImVec2(oPos.x + oTabSize.x, oPos.y + oTabSize.y);

		const float fOverlap = 10.f;
		const float fSlopWidth = 30.f;
		const float sSlopP1Ratio = 0.6f;
		const float fSlopP2Ratio = 0.4f;
		const float fSlopHRatio = 0.f;
		const float fShadowDropSize = 15.f;
		const float fShadowSlopRatio = 0.6f;
		const float fShadowAlpha = 0.75f;

		pDrawList->PathClear();
		if (bFocused)
		{
			pDrawList->ChannelsSetCurrent(1);
		}
		else
		{
			pDrawList->ChannelsSetCurrent(0);
		}

		//Drop shadows
		const ImVec2 uv = GImGui->FontTexUvWhitePixel;
		pDrawList->PrimReserve(3, 3);
		pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx)); pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 1)); pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 2));
		pDrawList->PrimWriteVtx(ImVec2(oRectMin.x - fOverlap - fShadowDropSize, oRectMax.y), uv, ImColor(0.f, 0.f, 0.f, 0.f));
		pDrawList->PrimWriteVtx(ImVec2(oRectMin.x - fOverlap + fSlopWidth * fShadowSlopRatio, oRectMin.y), uv, ImColor(0.f, 0.f, 0.f, 0.f));
		pDrawList->PrimWriteVtx(ImVec2(oRectMin.x - fOverlap + fSlopWidth * fShadowSlopRatio, oRectMax.y), uv, ImColor(0.f, 0.f, 0.f, fShadowAlpha));
		if (bFocused)
		{
			pDrawList->PrimReserve(3, 3);
			pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx)); pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 1)); pDrawList->PrimWriteIdx((ImDrawIdx)(pDrawList->_VtxCurrentIdx + 2));
			pDrawList->PrimWriteVtx(ImVec2(oRectMax.x + fOverlap + fShadowDropSize, oRectMax.y), uv, ImColor(0.f, 0.f, 0.f, 0.f));
			pDrawList->PrimWriteVtx(ImVec2(oRectMax.x + fOverlap - fSlopWidth * fShadowSlopRatio, oRectMin.y), uv, ImColor(0.f, 0.f, 0.f, 0.f));
			pDrawList->PrimWriteVtx(ImVec2(oRectMax.x + fOverlap - fSlopWidth * fShadowSlopRatio, oRectMax.y), uv, ImColor(0.f, 0.f, 0.f, fShadowAlpha));
		}

		// Draw tab and border
		if (bFocused && fStartLinePos < oPos.x)
		{
			pDrawList->PathLineTo(ImVec2(fStartLinePos, oRectMax.y));
		}
		pDrawList->PathLineTo(ImVec2(oRectMin.x - fOverlap, oRectMax.y));
		pDrawList->PathBezierCurveTo(
			ImVec2(oRectMin.x + fSlopWidth * sSlopP1Ratio - fOverlap, oRectMin.y + (oRectMax.y - oRectMin.y) * fSlopHRatio),
			ImVec2(oRectMin.x + fSlopWidth * fSlopP2Ratio - fOverlap, oRectMin.y),
			ImVec2(oRectMin.x + fSlopWidth - fOverlap, oRectMin.y)
			);
		pDrawList->PathLineTo(ImVec2(oRectMax.x - fSlopWidth + fOverlap, oRectMin.y));
		pDrawList->PathBezierCurveTo(
			ImVec2(oRectMax.x - fSlopWidth * fSlopP2Ratio + fOverlap, oRectMin.y),
			ImVec2(oRectMax.x - fSlopWidth * sSlopP1Ratio + fOverlap, oRectMin.y + (oRectMax.y - oRectMin.y) * fSlopHRatio),
			ImVec2(oRectMax.x + fOverlap, oRectMax.y)
			);

		if (bFocused)
		{
			pDrawList->AddConvexPolyFilled(pDrawList->_Path.Data + 1, pDrawList->_Path.Size - 1, bFocused ? oSelectedTab : oNormalTab, true);
			if (fEndLinePos > oRectMax.x)
			{
				pDrawList->PathLineTo(ImVec2(fEndLinePos, oRectMax.y));
			}
			pDrawList->AddPolyline(pDrawList->_Path.Data, pDrawList->_Path.Size, oBorderColor, false, 1.5f, true);
		}
		else
		{
			pDrawList->AddConvexPolyFilled(pDrawList->_Path.Data, pDrawList->_Path.Size, bFocused ? oSelectedTab : oNormalTab, true);
		}

		pDrawList->PathClear();

		ImVec2 oTextRectMin(oRectMin.x + 5, oRectMin.y);
		ImVec2 oTextRectMax(oRectMax.x - 5, oRectMax.y);
		ImGui::RenderTextClipped(oTextRectMin, oTextRectMax, pText, NULL, &oTextSize, ImGuiAlign_Center | ImGuiAlign_VCenter);
	}

	ImwContainer* ImwContainer::GetBestDocking(const ImVec2 oCursorPos, EDockOrientation& oOutOrientation, ImVec2& oOutAreaPos, ImVec2& oOutAreaSize, bool bLargeCheck)
	{
		if (m_pParent == NULL || 
			(oCursorPos.x >= m_oLastPosition.x && oCursorPos.x <= (m_oLastPosition.x + m_oLastSize.x) &&
			oCursorPos.y >= m_oLastPosition.y && oCursorPos.y <= (m_oLastPosition.y + m_oLastSize.y)))
		{
			if (IsSplit())
			{
				ImwContainer* pBestContainer = NULL;
				pBestContainer = m_pSplits[0]->GetBestDocking(oCursorPos, oOutOrientation, oOutAreaPos, oOutAreaSize, bLargeCheck);
				if (NULL != pBestContainer)
				{
					return pBestContainer;
				}
				pBestContainer = m_pSplits[1]->GetBestDocking(oCursorPos, oOutOrientation, oOutAreaPos, oOutAreaSize, bLargeCheck);
				if (NULL != pBestContainer)
				{
					return pBestContainer;
				}
			}
			else
			{
				const float c_fBoxHalfSize = 20.f;
				const float c_fBoxSize = c_fBoxHalfSize * 2.f;
				const float c_fMinSize = c_fBoxSize * 4.f;
				const float c_fSplitRatio = 0.5f;
				//const float c_fSplitRatio = oConfig.m_fDragMarginSizeRatio;
				const ImColor oBoxColor(200, 200, 255, 255);
				const ImColor oBoxHightlightColor(100, 100, 255, 255);

				if (m_oLastSize.x < c_fMinSize && m_oLastSize.y < c_fMinSize)
				{
					oOutOrientation = E_DOCK_ORIENTATION_CENTER;
					oOutAreaPos = m_oLastPosition;
					oOutAreaSize = m_oLastSize;
					return this;
				}
				else
				{
					const ImVec2 oCenter = ImVec2(m_oLastPosition.x + m_oLastSize.x / 2.f, m_oLastPosition.y + m_oLastSize.y / 2.f);

					bool bIsInCenter = false;
					bool bIsInTop = false;
					bool bIsInLeft = false;
					bool bIsInRight = false;
					bool bIsInBottom = false;

					if (bLargeCheck)
					{
						ImRect oRectCenter(ImVec2(oCenter.x - c_fBoxHalfSize * 2.f, oCenter.y - c_fBoxHalfSize * 2.f), ImVec2(oCenter.x + c_fBoxHalfSize * 2.f, oCenter.y + c_fBoxHalfSize * 2.f));

						ImRect oRectTop = ImRect(ImVec2(m_oLastPosition.x, m_oLastPosition.y), ImVec2(m_oLastPosition.x + m_oLastSize.x, oCenter.y - c_fBoxHalfSize * 2.f));
						ImRect oRectBottom = ImRect(ImVec2(m_oLastPosition.x, oCenter.y + c_fBoxHalfSize * 2.f), ImVec2(m_oLastPosition.x + m_oLastSize.x, m_oLastPosition.y + m_oLastSize.y));

						ImRect oRectLeft = ImRect(ImVec2(m_oLastPosition.x, m_oLastPosition.y), ImVec2(oCenter.x - c_fBoxHalfSize * 2.f, m_oLastPosition.y + m_oLastSize.y));
						ImRect oRectRight = ImRect(ImVec2(oCenter.x + c_fBoxHalfSize * 2.f, m_oLastPosition.y), ImVec2(m_oLastPosition.x + m_oLastSize.x, m_oLastPosition.y + m_oLastSize.y));

						bIsInCenter = oRectCenter.Contains(oCursorPos);

						if (m_oLastSize.y >= c_fMinSize)
						{
							bIsInTop = oRectTop.Contains(oCursorPos);
							bIsInBottom = oRectBottom.Contains(oCursorPos);
						}

						if (m_oLastSize.x >= c_fMinSize)
						{
							bIsInLeft = oRectLeft.Contains(oCursorPos);
							bIsInRight = oRectRight.Contains(oCursorPos);
						}
					}
					else
					{
						//Center
						ImRect oRectCenter(ImVec2(oCenter.x - c_fBoxHalfSize, oCenter.y - c_fBoxHalfSize), ImVec2(oCenter.x + c_fBoxHalfSize, oCenter.y + c_fBoxHalfSize));
						bIsInCenter = oRectCenter.Contains(oCursorPos);
						ImwWindowManager::GetInstance()->DrawWindowArea(m_pParentWindow, oRectCenter.Min, oRectCenter.GetSize(), bIsInCenter ? oBoxHightlightColor : oBoxColor);

						if (m_oLastSize.y >= c_fMinSize)
						{
							//Top
							ImRect oRectTop(ImVec2(oCenter.x - c_fBoxHalfSize, oCenter.y - c_fBoxHalfSize * 4.f), ImVec2(oCenter.x + c_fBoxHalfSize, oCenter.y - c_fBoxHalfSize * 2.f));
							bIsInTop = oRectTop.Contains(oCursorPos);
							ImwWindowManager::GetInstance()->DrawWindowArea(m_pParentWindow, oRectTop.Min, oRectTop.GetSize(), bIsInTop ? oBoxHightlightColor : oBoxColor);

							//Bottom
							ImRect oRectBottom(ImVec2(oCenter.x - c_fBoxHalfSize, oCenter.y + c_fBoxHalfSize * 2.f), ImVec2(oCenter.x + c_fBoxHalfSize, oCenter.y + c_fBoxHalfSize * 4.f));
							bIsInBottom = oRectBottom.Contains(oCursorPos);
							ImwWindowManager::GetInstance()->DrawWindowArea(m_pParentWindow, oRectBottom.Min, oRectBottom.GetSize(), bIsInBottom ? oBoxHightlightColor : oBoxColor);
						}

						if (m_oLastSize.x >= c_fMinSize)
						{
							//Left
							ImRect oRectLeft(ImVec2(oCenter.x - c_fBoxHalfSize * 4.f, oCenter.y - c_fBoxHalfSize), ImVec2(oCenter.x - c_fBoxHalfSize * 2.f, oCenter.y + c_fBoxHalfSize));
							bIsInLeft = oRectLeft.Contains(oCursorPos);
							ImwWindowManager::GetInstance()->DrawWindowArea(m_pParentWindow, oRectLeft.Min, oRectLeft.GetSize(), bIsInLeft ? oBoxHightlightColor : oBoxColor);

							//Right
							ImRect oRectRight(ImVec2(oCenter.x + c_fBoxHalfSize * 2.f, oCenter.y - c_fBoxHalfSize), ImVec2(oCenter.x + c_fBoxHalfSize * 4.f, oCenter.y + c_fBoxHalfSize));
							bIsInRight = oRectRight.Contains(oCursorPos);
							ImwWindowManager::GetInstance()->DrawWindowArea(m_pParentWindow, oRectRight.Min, oRectRight.GetSize(), bIsInRight ? oBoxHightlightColor : oBoxColor);
						}
					}

					if (bIsInCenter)
					{
						oOutOrientation = E_DOCK_ORIENTATION_CENTER;
						oOutAreaPos = m_oLastPosition;
						oOutAreaSize = m_oLastSize;
						return this;
					}
					else if (bIsInTop)
					{
						oOutOrientation = E_DOCK_ORIENTATION_TOP;
						oOutAreaPos = m_oLastPosition;
						oOutAreaSize = ImVec2(m_oLastSize.x, m_oLastSize.y * c_fSplitRatio);
						return this;
					}
					else if (bIsInLeft)
					{
						oOutOrientation = E_DOCK_ORIENTATION_LEFT;
						oOutAreaPos = m_oLastPosition;
						oOutAreaSize = ImVec2(m_oLastSize.x * c_fSplitRatio, m_oLastSize.y);
						return this;
					}
					else if (bIsInRight)
					{
						oOutOrientation = E_DOCK_ORIENTATION_RIGHT;
						oOutAreaPos = ImVec2(m_oLastPosition.x + m_oLastSize.x * (1.f - c_fSplitRatio), m_oLastPosition.y);
						oOutAreaSize = ImVec2(m_oLastSize.x * c_fSplitRatio, m_oLastSize.y);
						return this;
					}
					else if (bIsInBottom)
					{
						oOutOrientation = E_DOCK_ORIENTATION_BOTTOM;
						oOutAreaPos = ImVec2(m_oLastPosition.x, m_oLastPosition.y + m_oLastSize.y * (1.f - c_fSplitRatio));
						oOutAreaSize = ImVec2(m_oLastSize.x, m_oLastSize.y * c_fSplitRatio);
						return this;
					}
				}
			}
		}

		return NULL;
	}

	bool ImwContainer::HasUnclosableWindow() const
	{
		for (ImwWindowList::const_iterator itWindow = m_lWindows.begin(); itWindow != m_lWindows.end(); ++itWindow)
		{
			if ( !(*itWindow)->IsClosable() )
			{
				return true;
			}
		}
		if (IsSplit())
		{
			return m_pSplits[0]->HasUnclosableWindow() || m_pSplits[1]->HasUnclosableWindow();
		}
		return false;
	}

	ImwPlatformWindow::ImwPlatformWindow(bool bMain, bool bIsDragWindow, bool bCreateState)
	{
		m_bMain = bMain;
		m_bIsDragWindow = bIsDragWindow;
		m_pContainer = new ImwContainer(this);
		m_pState = NULL;
		m_pPreviousState = NULL;

		if (bCreateState)
		{
			void* pTemp = ImGui::GetInternalState();
			m_pState = ImwMalloc(ImGui::GetInternalStateSize());
			ImGui::SetInternalState(m_pState, true);
			ImGui::GetIO().IniFilename = NULL;
			ImGui::SetInternalState(pTemp);
		}
	}

	ImwPlatformWindow::~ImwPlatformWindow()
	{
		ImwSafeDelete(m_pContainer);

		SetState();
		if (!IsMain())
		{
			ImGui::GetIO().Fonts = NULL;
		}
		ImGui::Shutdown();
		RestoreState();
		ImwSafeDelete(m_pState);
	}

	bool ImwPlatformWindow::Init(ImwPlatformWindow* /*pParent*/)
	{
		return true;
	}

	const ImVec2 c_oVec2_0 = ImVec2(0,0);
	const ImVec2& ImwPlatformWindow::GetPosition() const
	{
		return c_oVec2_0;
	}

	const ImVec2& ImwPlatformWindow::GetSize() const
	{
		return ImGui::GetIO().DisplaySize;
	}

	void ImwPlatformWindow::Show()
	{
	}

	void ImwPlatformWindow::Hide()
	{
	}

	void ImwPlatformWindow::SetSize(int /*iWidth*/, int /*iHeight*/)
	{
	}

	void ImwPlatformWindow::SetPosition(int /*iX*/, int /*iY*/)
	{
	}

	void ImwPlatformWindow::SetTitle(const char* /*pTtile*/)
	{
	}

	void ImwPlatformWindow::OnClose()
	{
		ImwWindowManager::GetInstance()->OnClosePlatformWindow(this);
	}

	static bool s_bStatePush = false;

	bool ImwPlatformWindow::IsStateSet()
	{
		return s_bStatePush;
	}

	void ImwPlatformWindow::SetState()
	{
		IM_ASSERT(s_bStatePush == false);
		s_bStatePush = true;
		if (m_pState != NULL)
		{
			m_pPreviousState = ImGui::GetInternalState();
			ImGui::SetInternalState(m_pState);
			memcpy(&((ImGuiState*)m_pState)->Style, &((ImGuiState*)m_pPreviousState)->Style, sizeof(ImGuiStyle));
		}
	}

	void ImwPlatformWindow::RestoreState()
	{
		IM_ASSERT(s_bStatePush == true);
		s_bStatePush = false;
		if (m_pState != NULL)
		{
			memcpy(&((ImGuiState*)m_pPreviousState)->Style, &((ImGuiState*)m_pState)->Style, sizeof(ImGuiStyle));
			ImGui::SetInternalState(m_pPreviousState);
		}
	}

	void ImwPlatformWindow::OnLoseFocus()
	{
		ImGuiState& g = *((ImGuiState*)m_pState);
		g.SetNextWindowPosCond = g.SetNextWindowSizeCond = g.SetNextWindowContentSizeCond = g.SetNextWindowCollapsedCond = g.SetNextWindowFocus = 0;
	}

	void ImwPlatformWindow::PreUpdate()
	{
	}

	void ImwPlatformWindow::Destroy()
	{
	}

	void ImwPlatformWindow::StartDrag()
	{
	}

	void ImwPlatformWindow::StopDrag()
	{
	}

	bool ImwPlatformWindow::IsDraging()
	{
		return false;
	}

	void ImwPlatformWindow::Paint()
	{
		ImwWindowManager::GetInstance()->Paint(this);
	}

	bool ImwPlatformWindow::IsMain()
	{
		return m_bMain;
	}

	void ImwPlatformWindow::Dock(ImwWindow* pWindow)
	{
		m_pContainer->Dock(pWindow);
	}

	bool ImwPlatformWindow::UnDock(ImwWindow* pWindow)
	{
		return m_pContainer->UnDock(pWindow);
	}

	ImwContainer* ImwPlatformWindow::GetContainer()
	{
		return m_pContainer;
	}

	ImwContainer* ImwPlatformWindow::HasWindow(ImwWindow* pWindow)
	{
		return m_pContainer->HasWindow(pWindow);
	}

	void ImwPlatformWindow::PaintContainer()
	{
		m_pContainer->Paint();
	}

	ImwWindowManager::DrawWindowAreaAction::DrawWindowAreaAction( ImwPlatformWindow* pWindow, const ImVec2& oRectPos, const ImVec2& oRectSize, const ImColor& oColor )
		: m_oColor( oColor )
	{
		m_pWindow = pWindow;
		m_oRectPos = oRectPos;
		m_oRectSize = oRectSize;
	}

	ImwWindowManager* ImwWindowManager::s_pInstance = 0;

	//////////////////////////////////////////////////////////////////////////

	ImwWindowManager::Config::Config()
		: m_fDragMarginRatio( 0.1f )
		, m_fDragMarginSizeRatio( 0.25f )
		, m_oHightlightAreaColor( 0.f, 0.5f, 1.f, 0.5f )
	{
	}

	//////////////////////////////////////////////////////////////////////////

	ImwWindowManager::ImwWindowManager()
	{
		s_pInstance = this;
		m_pMainPlatformWindow = NULL;
		m_pDragPlatformWindow = NULL;
		m_pCurrentPlatformWindow = NULL;
		m_pDraggedWindow = NULL;
		m_oDragPreviewOffset = ImVec2(-20, -10);
	}

	ImwWindowManager::~ImwWindowManager()
	{
		ImwSafeDelete(m_pMainPlatformWindow);
		ImwSafeDelete(m_pDragPlatformWindow);
		s_pInstance = 0;
		ImGui::Shutdown();
	}

	bool ImwWindowManager::Init()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		io.IniFilename = NULL;

		//io.Fonts->AddFontFromFileTTF( "res/DroidSans.ttf", 16 ) || io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF( "res/DroidSans-Bold.ttf", 16 ) || io.Fonts->AddFontDefault();

		style.FrameRounding = 2.f;
		style.WindowRounding = 0.f;
		style.ScrollbarRounding = 0.f;

		m_pMainPlatformWindow = CreatePlatformWindow(true, NULL, false);
		if (NULL != m_pMainPlatformWindow)
		{
			m_pMainPlatformWindow->Show();

			if (CanCreateMultipleWindow())
			{
				m_pDragPlatformWindow = CreatePlatformWindow(false, m_pMainPlatformWindow, true);
			}
		
			return true;
		}
		return false;
	}

	bool ImwWindowManager::Run()
	{
		PreUpdate();
		InternalRun();
		Update();
		return m_pMainPlatformWindow != NULL;
	}

	void ImwWindowManager::Exit()
	{
		//TODO : Manual exit
	}

	ImwPlatformWindow* ImwWindowManager::GetMainPlatformWindow()
	{
		return m_pMainPlatformWindow;
	}

	ImwWindowManager::Config& ImwWindowManager::GetConfig()
	{
		return m_oConfig;
	}

	void ImwWindowManager::SetMainTitle(const char* pTitle)
	{
		ImwIsSafe(m_pMainPlatformWindow)->SetTitle(pTitle);
	}

	void ImwWindowManager::UnDock(ImwWindow* pWindow)
	{
		DockAction* pAction = new DockAction();
		pAction->m_bFloat = false;
		pAction->m_pWindow = pWindow;
		pAction->m_pWith = NULL;
		pAction->m_eOrientation = E_DOCK_ORIENTATION_CENTER;
		pAction->m_pToPlatformWindow = NULL;
		pAction->m_pToContainer = NULL;
		m_lDockActions.push_back(pAction);
	}

	void ImwWindowManager::Dock(ImwWindow* pWindow, EDockOrientation eOrientation, ImwPlatformWindow* pToPlatformWindow)
	{
		DockAction* pAction = new DockAction();
		pAction->m_bFloat = false;
		pAction->m_pWindow = pWindow;
		pAction->m_pWith = NULL;
		pAction->m_eOrientation = eOrientation;
		pAction->m_pToPlatformWindow = (pToPlatformWindow != NULL) ? pToPlatformWindow : m_pMainPlatformWindow;
		pAction->m_pToContainer = NULL;
		m_lDockActions.push_back(pAction);
	}

	void ImwWindowManager::DockTo(ImwWindow* pWindow, EDockOrientation eOrientation, ImwContainer* pContainer)
	{
		IM_ASSERT(NULL != pContainer);
		if (NULL != pContainer)
		{
			DockAction* pAction = new DockAction();
			pAction->m_bFloat = false;
			pAction->m_pWindow = pWindow;
			pAction->m_pWith = NULL;
			pAction->m_eOrientation = eOrientation;
			pAction->m_pToPlatformWindow = NULL;
			pAction->m_pToContainer = pContainer;
			m_lDockActions.push_back(pAction);
		}
	}

	void ImwWindowManager::DockWith(ImwWindow* pWindow, ImwWindow* pWithWindow, EDockOrientation eOrientation)
	{
		DockAction* pAction = new DockAction();
		pAction->m_bFloat = false;
		pAction->m_pWindow = pWindow;
		pAction->m_pWith = pWithWindow;
		pAction->m_eOrientation = eOrientation;
		m_lDockActions.push_back(pAction);
	}

	void ImwWindowManager::Float(ImwWindow* pWindow, const ImVec2& oPosition, const ImVec2& oSize)
	{
		DockAction* pAction = new DockAction();
		pAction->m_bFloat = true;
		pAction->m_pWindow = pWindow;
		pAction->m_oPosition = oPosition;
		pAction->m_oSize = oSize;
		m_lDockActions.push_back(pAction);
	}

	const ImwWindowList& ImwWindowManager::GetWindowList() const
	{
		return m_lWindows;
	}

	ImwPlatformWindow* ImwWindowManager::GetCurrentPlatformWindow()
	{
		return m_pCurrentPlatformWindow;
	}

	ImwPlatformWindow* ImwWindowManager::GetWindowParent(ImwWindow* pWindow)
	{
		ImwContainer* pContainer = m_pMainPlatformWindow->HasWindow(pWindow);
		if (NULL != pContainer)
		{
			return m_pMainPlatformWindow;
		}

		for ( ImwList<ImwPlatformWindow*>::iterator it = m_lPlatformWindows.begin(); it != m_lPlatformWindows.end(); ++it )
		{
			pContainer = (*it)->HasWindow(pWindow);
			if (NULL != pContainer)
			{
				return *it;
			}
		}
		IM_ASSERT(false);
		return NULL;
	}

	bool ImwWindowManager::CanCreateMultipleWindow()
	{
		return false;
	}

	ImwPlatformWindow* ImwWindowManager::CreatePlatformWindow(bool bMain, ImwPlatformWindow* /*pParent*/, bool bDragWindow)
	{
		if (bMain)
		{
			return (ImWindow::ImwPlatformWindow*)new ImwPlatformWindow(bMain, bDragWindow, CanCreateMultipleWindow());
		}
		return NULL;
	}

	void ImwWindowManager::InternalRun()
	{

	}

	ImVec2 ImwWindowManager::GetCursorPos()
	{
		return ImGui::GetIO().MousePos;
	}

	bool ImwWindowManager::IsLeftClickDown()
	{
		return ImGui::GetIO().MouseDown[0];
	}

	void ImwWindowManager::PreUpdate()
	{
		ImwIsSafe(m_pMainPlatformWindow)->PreUpdate();

		for (ImwList<ImwPlatformWindow*>::iterator it = m_lPlatformWindows.begin(); it != m_lPlatformWindows.end(); ++it)
		{
			(*it)->PreUpdate();
		}
	}

	void ImwWindowManager::Update()
	{
		UpdatePlatformwWindowActions();
		UpdateDockActions();
		UpdateOrphans();

		while (m_lToDestroyWindows.begin() != m_lToDestroyWindows.end())
		{
			ImwWindow* pWindow = *m_lToDestroyWindows.begin();

			m_lToDestroyWindows.remove(pWindow);
			m_lOrphanWindows.remove(pWindow);
			m_lWindows.remove(pWindow);

			InternalUnDock(pWindow);

			delete pWindow;
		}

		while (m_lToDestroyPlatformWindows.begin() != m_lToDestroyPlatformWindows.end())
		{
			ImwPlatformWindow* pPlatformWindow = *m_lToDestroyPlatformWindows.begin();
			m_lToDestroyPlatformWindows.remove(pPlatformWindow);
			m_lPlatformWindows.remove(pPlatformWindow);
			delete pPlatformWindow;
		}

		UpdateDragWindow();
	
		m_pCurrentPlatformWindow = m_pMainPlatformWindow;
		ImwIsSafe(m_pMainPlatformWindow)->Paint();

		for ( ImwList<ImwPlatformWindow*>::iterator it = m_lPlatformWindows.begin(); it != m_lPlatformWindows.end(); ++it )
		{
			m_pCurrentPlatformWindow = (*it);
			(*it)->Paint();
		}

		m_pCurrentPlatformWindow = NULL;

	
	}

	void ImwWindowManager::UpdatePlatformwWindowActions()
	{
		while (m_lPlatformWindowActions.begin() != m_lPlatformWindowActions.end())
		{
			PlatformWindowAction* pAction = *m_lPlatformWindowActions.begin();
		
			IM_ASSERT((pAction->m_iFlags & E_PLATFORM_WINDOW_ACTION_SHOW & E_PLATFORM_WINDOW_ACTION_HIDE) == 0); // Can't show and hide		

			if (pAction->m_iFlags & E_PLATFORM_WINDOW_ACTION_DESTROY)
			{
				//pAction->m_pPlatformWindow->Show();
				//todo destroy
				bool bFound = false;
				if (m_pMainPlatformWindow == pAction->m_pPlatformWindow)
				{
					while (m_lPlatformWindows.begin() != m_lPlatformWindows.end())
					{
						delete *m_lPlatformWindows.begin();
						m_lPlatformWindows.erase(m_lPlatformWindows.begin());
					}
					delete m_pMainPlatformWindow;
					m_pMainPlatformWindow = NULL;
					bFound = true;
				}
				else
				{
					for (ImwList<ImwPlatformWindow*>::iterator it = m_lPlatformWindows.begin(); it != m_lPlatformWindows.end(); ++it)
					{
						if (*it == pAction->m_pPlatformWindow)
						{
							delete *it;
							m_lPlatformWindows.erase(it);
							bFound = true;
							break;
						}
					}
				}

				if (!bFound)
				{
					IM_ASSERT(false); // ImwPlatformWindow not found, maybe already closed
				}
			}
			if (pAction->m_iFlags & E_PLATFORM_WINDOW_ACTION_SHOW)
			{
				pAction->m_pPlatformWindow->Show();
			}
			if (pAction->m_iFlags & E_PLATFORM_WINDOW_ACTION_HIDE)
			{
				pAction->m_pPlatformWindow->Hide();
			}
			if (pAction->m_iFlags & E_PLATFORM_WINDOW_ACTION_SET_POSITION)
			{
				pAction->m_pPlatformWindow->SetPosition((int)pAction->m_oPosition.x, (int)pAction->m_oPosition.y);
			}
			if (pAction->m_iFlags & E_PLATFORM_WINDOW_ACTION_SET_SIZE)
			{
				pAction->m_pPlatformWindow->SetSize((int)pAction->m_oSize.x, (int)pAction->m_oSize.y);
			}

			delete *m_lPlatformWindowActions.begin();
			m_lPlatformWindowActions.erase(m_lPlatformWindowActions.begin());
		}
	}

	void ImwWindowManager::UpdateDockActions()
	{
		while (m_lDockActions.begin() != m_lDockActions.end())
		{
			DockAction* pAction = *m_lDockActions.begin();

			InternalUnDock(pAction->m_pWindow);

			if (pAction->m_bFloat)
			{
				InternalFloat(pAction->m_pWindow, pAction->m_oPosition, pAction->m_oSize);
			}
			else
			{
				if (NULL != pAction->m_pWith)
				{
					InternalDockWith(pAction->m_pWindow, pAction->m_pWith, pAction->m_eOrientation);
				}
				else if (NULL != pAction->m_pToContainer)
				{
					InternalDockTo(pAction->m_pWindow, pAction->m_eOrientation, pAction->m_pToContainer);
				}
				else if (NULL != pAction->m_pToPlatformWindow)
				{
					InternalDock(pAction->m_pWindow, pAction->m_eOrientation, pAction->m_pToPlatformWindow);
				}
			}

			m_lOrphanWindows.remove(pAction->m_pWindow);

			delete pAction;
			m_lDockActions.erase(m_lDockActions.begin());
		}
	}

	void ImwWindowManager::UpdateOrphans()
	{
		while (m_lOrphanWindows.begin() != m_lOrphanWindows.end())
		{
			if (m_pMainPlatformWindow->m_pContainer->IsEmpty())
			{
				InternalDock(*m_lOrphanWindows.begin(), E_DOCK_ORIENTATION_CENTER, m_pMainPlatformWindow);
			}
			else if (CanCreateMultipleWindow())
			{
				ImVec2 oSize = ImVec2(300, 300);
				ImVec2 oPos = m_pMainPlatformWindow->GetPosition();
				ImVec2 oMainSize = m_pMainPlatformWindow->GetSize();
				oPos.x += (oMainSize.x - oSize.x) / 2;
				oPos.y += (oMainSize.y - oSize.y) / 2;
				InternalFloat(*m_lOrphanWindows.begin(), oPos, oSize);
			}
			else
			{
				m_pMainPlatformWindow->m_pContainer->DockToBest(*m_lOrphanWindows.begin());
			}
			m_lOrphanWindows.erase(m_lOrphanWindows.begin());
		}
	}

	void ImwWindowManager::Paint(ImwPlatformWindow* pWindow)
	{
		float fY = 0.f;
		if (pWindow->IsMain())
		{
			ImGui::BeginMainMenuBar();
			for ( ImwWindowList::iterator it = m_lWindows.begin(); it != m_lWindows.end(); ++it )
			{
				(*it)->OnMenu();
			}
			fY = ImGui::GetWindowHeight();
			ImGui::EndMainMenuBar();
		}

		ImGui::SetNextWindowPos(ImVec2(0, fY), ImGuiSetCond_Always);
		ImGui::SetNextWindowSize(ImVec2(pWindow->GetSize().x, pWindow->GetSize().y - fY), ImGuiSetCond_Always);
		int iFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

		if (NULL != m_pDraggedWindow)
		{
			iFlags += ImGuiWindowFlags_NoInputs;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
		//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(40,40,40,0));
		//ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(59, 59, 59, 255));
		ImGui::Begin( "Main", NULL, iFlags );
		pWindow->PaintContainer();
		ImGui::End();

		//ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(1);

		ImGui::Begin("##Overlay", NULL, ImVec2(0, 0), 0.f, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);
		ImDrawList* pDrawList = ImGui::GetWindowDrawList();
		for (ImwList<DrawWindowAreaAction>::iterator it = m_lDrawWindowAreas.begin(); it != m_lDrawWindowAreas.end(); )
		{
			DrawWindowAreaAction& oAction = *it;
			//if (pWindow->HasWindow(oAction.m_pWindow))
			if (pWindow == oAction.m_pWindow)
			{
				ImVec2 oPosA = oAction.m_oRectPos;
				ImVec2 oPosB = oAction.m_oRectSize;
				oPosB.x += oPosA.x;
				oPosB.y += oPosA.y;

				pDrawList->PushClipRectFullScreen();
				//pDrawList->AddLine(ImGui::CalcItemRectClosestPoint(ImGui::GetIO().MousePos, true, -2.0f), ImGui::GetIO().MousePos, ImColor(ImGui::GetStyle().Colors[ImGuiCol_Button]), 4.0f);
				pDrawList->AddRectFilled(oPosA, oPosB, oAction.m_oColor);
				pDrawList->PopClipRect();
				ImwList<DrawWindowAreaAction>::iterator toRemove = it;
				++it;
				m_lDrawWindowAreas.erase(toRemove);
			}
			else
			{
				++it;
			}
		}
		ImGui::End();

	
		ImGui::Render();
	}

	void ImwWindowManager::StartDragWindow(ImwWindow* pWindow, ImVec2 oOffset)
	{
		if (NULL == m_pDraggedWindow)
		{
			m_pDraggedWindow = pWindow;
			m_oDragPreviewOffset = oOffset;

			if (NULL != m_pDragPlatformWindow)
			{
				PlatformWindowAction* pAction = new PlatformWindowAction();
				pAction->m_pPlatformWindow = m_pDragPlatformWindow;
				pAction->m_iFlags = E_PLATFORM_WINDOW_ACTION_SHOW | E_PLATFORM_WINDOW_ACTION_SET_POSITION | E_PLATFORM_WINDOW_ACTION_SET_SIZE;
				ImVec2 oCursorPos = GetCursorPos();
				pAction->m_oPosition = ImVec2(oCursorPos.x + m_oDragPreviewOffset.x, oCursorPos.y + m_oDragPreviewOffset.y);
				pAction->m_oSize = ImVec2(pWindow->GetLastSize().x, pWindow->GetLastSize().y);
				m_lPlatformWindowActions.push_back(pAction);
				Dock(pWindow, E_DOCK_ORIENTATION_CENTER, m_pDragPlatformWindow);
				((ImGuiState*)m_pDragPlatformWindow->m_pState)->IO.MouseDown[0] = true;
			}
			else
			{
				UnDock(pWindow);
			}
		}
	}

	void ImwWindowManager::StopDragWindow()
	{
		if ( NULL != m_pDragPlatformWindow )
		{
			PlatformWindowAction* pAction = new PlatformWindowAction();
			pAction->m_pPlatformWindow = m_pDragPlatformWindow;
			pAction->m_iFlags = E_PLATFORM_WINDOW_ACTION_HIDE;
			m_pDragPlatformWindow->Hide();
			m_lPlatformWindowActions.push_back(pAction);
		}
		m_pDraggedWindow = NULL;
	}

	void ImwWindowManager::UpdateDragWindow()
	{
		if (NULL != m_pDraggedWindow)
		{
			ImVec2 oCursorPos = GetCursorPos();
			if (NULL != m_pDragPlatformWindow)
			{
				m_pCurrentPlatformWindow = m_pDragPlatformWindow;
				m_pDragPlatformWindow->Paint();
				m_pCurrentPlatformWindow = NULL;
			
				m_pDragPlatformWindow->SetPosition((int)(oCursorPos.x + m_oDragPreviewOffset.x), (int)(oCursorPos.y + m_oDragPreviewOffset.y));
			}

			//Search best dock area
			EDockOrientation eBestDockOrientation;
			ImVec2 oHightlightPos;
			ImVec2 oHightlightSize;
			ImwContainer* pBestContainer = GetBestDocking(m_pMainPlatformWindow, oCursorPos, eBestDockOrientation, oHightlightPos, oHightlightSize, !CanCreateMultipleWindow());
			if (NULL == pBestContainer)
			{
				for (ImwList<ImwPlatformWindow*>::iterator it = m_lPlatformWindows.begin(); it != m_lPlatformWindows.end() && NULL == pBestContainer; ++it)
				{
					pBestContainer = GetBestDocking(*it, oCursorPos, eBestDockOrientation, oHightlightPos, oHightlightSize, false);
				}
			}
			if (pBestContainer)
			{
				DrawWindowArea(pBestContainer->GetPlatformWindowParent(), oHightlightPos, oHightlightSize, m_oConfig.m_oHightlightAreaColor);
			}

			//if (!((ImGuiState*)m_pDragPlatformWindow->m_pState)->IO.MouseDown[0])
			if (!IsLeftClickDown())
			{
				if (NULL != pBestContainer)
				{
					DockTo(m_pDraggedWindow, eBestDockOrientation, pBestContainer);
				}
				else if (NULL != m_pDragPlatformWindow)
				{
					Float(m_pDraggedWindow, m_pDragPlatformWindow->GetPosition(), m_pDragPlatformWindow->GetSize());
				}
				else
				{
					Dock(m_pDraggedWindow, E_DOCK_ORIENTATION_CENTER, m_pMainPlatformWindow);
				}

				StopDragWindow();
			}
		}
	}

	ImwContainer* ImwWindowManager::GetBestDocking(ImwPlatformWindow* pPlatformWindow, const ImVec2 oCursorPos, EDockOrientation& oOutOrientation, ImVec2& oOutAreaPos, ImVec2& oOutAreaSize, bool bLargeCheck)
	{
		ImVec2 oPos = pPlatformWindow->GetPosition();
		ImVec2 oSize = pPlatformWindow->GetSize();
		if (bLargeCheck || (oCursorPos.x >= oPos.x && oCursorPos.x <= (oPos.x + oSize.x) &&
			oCursorPos.y >= oPos.y && oCursorPos.y <= (oPos.y + oSize.y)))
		{
			ImVec2 oRectPos(oCursorPos.x - oPos.x, oCursorPos.y - oPos.y);

			ImwContainer* pBestContainer = pPlatformWindow->GetContainer()->GetBestDocking(oRectPos, oOutOrientation, oOutAreaPos, oOutAreaSize, bLargeCheck);
			if (NULL != pBestContainer)
			{
				return pBestContainer;
			}

			//Left
			if (oRectPos.x <= oSize.x * m_oConfig.m_fDragMarginRatio)
			{
				oOutOrientation = E_DOCK_ORIENTATION_LEFT;
				oOutAreaPos = ImVec2(0, 0);
				oOutAreaSize = ImVec2(oSize.x * m_oConfig.m_fDragMarginSizeRatio, oSize.y);
			}
			//Right
			else if (oRectPos.x >=  oSize.x * (1.f - m_oConfig.m_fDragMarginRatio))
			{
				oOutOrientation = E_DOCK_ORIENTATION_RIGHT;
				oOutAreaPos = ImVec2(oSize.x * (1.f - m_oConfig.m_fDragMarginSizeRatio), 0.f);
				oOutAreaSize = ImVec2(oSize.x * m_oConfig.m_fDragMarginSizeRatio, oSize.y);
			}
			//Top
			else if (oRectPos.y <= oSize.y * m_oConfig.m_fDragMarginRatio)
			{
				oOutOrientation = E_DOCK_ORIENTATION_TOP;
				oOutAreaPos = ImVec2(0, 0);
				oOutAreaSize = ImVec2(oSize.x, oSize.y * m_oConfig.m_fDragMarginSizeRatio);
			}
			//Bottom
			else if (oRectPos.y >=  oSize.y * (1.f - m_oConfig.m_fDragMarginRatio))
			{
				oOutOrientation = E_DOCK_ORIENTATION_BOTTOM;
				oOutAreaPos = ImVec2(0.f, oSize.y * (1.f - m_oConfig.m_fDragMarginSizeRatio));
				oOutAreaSize = ImVec2(oSize.x, oSize.y * m_oConfig.m_fDragMarginSizeRatio);
			}
			else
			{
				oOutOrientation = E_DOCK_ORIENTATION_CENTER;
				oOutAreaPos = ImVec2(0, 0);
				oOutAreaSize = ImVec2(oSize.x, oSize.y);
				//IM_ASSERT(false); //Best dock orientation not found
				return NULL;
			}
			return pPlatformWindow->GetContainer();
		}
		return NULL;
	}

	void ImwWindowManager::AddWindow(ImwWindow* pWindow)
	{
		m_lWindows.push_back(pWindow);

		m_lOrphanWindows.push_back(pWindow);
	}

	void ImwWindowManager::RemoveWindow(ImwWindow* pWindow)
	{
		m_lWindows.remove(pWindow);
		m_lOrphanWindows.remove(pWindow);
	}

	void ImwWindowManager::DestroyWindow(ImwWindow* pWindow)
	{
		if (NULL != pWindow && std::find(m_lToDestroyWindows.begin(), m_lToDestroyWindows.end(), pWindow) == m_lToDestroyWindows.end())
		{
			m_lToDestroyWindows.push_back(pWindow);
		}
	}

	void ImwWindowManager::InternalDock(ImwWindow* pWindow, EDockOrientation eOrientation, ImwPlatformWindow* pToPlatformWindow)
	{
		pToPlatformWindow->m_pContainer->Dock(pWindow, eOrientation);
	}

	void ImwWindowManager::InternalDockTo(ImwWindow* pWindow, EDockOrientation eOrientation, ImwContainer* pToContainer)
	{
		pToContainer->Dock(pWindow, eOrientation);
	}

	void ImwWindowManager::InternalDockWith(ImwWindow* pWindow, ImwWindow* pWithWindow, EDockOrientation eOrientation)
	{
		ImwContainer* pContainer = m_pMainPlatformWindow->HasWindow(pWithWindow);
		if (NULL != pContainer)
		{
			pContainer->Dock(pWindow, eOrientation);
		}

		for ( ImwList<ImwPlatformWindow*>::iterator it = m_lPlatformWindows.begin(); it != m_lPlatformWindows.end(); ++it )
		{
			pContainer = (*it)->HasWindow(pWithWindow);
			if (NULL != pContainer)
			{
				pContainer->Dock(pWindow, eOrientation);
				break;
			}
		}
	}

	void ImwWindowManager::InternalFloat(ImwWindow* pWindow, ImVec2 oPosition, ImVec2 oSize)
	{
		ImwPlatformWindow* pPlatformWindow = CreatePlatformWindow(false, m_pMainPlatformWindow, false);
		if (NULL != pPlatformWindow)
		{
			m_lPlatformWindows.push_back(pPlatformWindow);

			const ImVec2 oVec2_1 = ImVec2(1, 1);
			const ImVec2 oVec2_N1 = ImVec2(-1, -1);

			if (oSize.x == oVec2_1.x && oSize.y == oVec2_1.y)
			{
				oSize = pWindow->GetLastSize();
			}
			if (oPosition.x == oVec2_N1.x && oPosition.y == oVec2_N1.y)
			{
				oPosition = GetCursorPos();
				oPosition.x -= 20;
				oPosition.x -= 10;
			}
			pPlatformWindow->Dock(pWindow);
			pPlatformWindow->SetSize((int)oSize.x, (int)oSize.y);
			pPlatformWindow->SetPosition((int)oPosition.x, (int)oPosition.y);
			pPlatformWindow->Show();
		}
	}

	void ImwWindowManager::InternalUnDock(ImwWindow* pWindow)
	{
		if (m_pMainPlatformWindow->UnDock(pWindow))
		{
			return;
		}

		for ( ImwList<ImwPlatformWindow*>::iterator it = m_lPlatformWindows.begin(); it != m_lPlatformWindows.end(); ++it )
		{
			if ( (*it)->UnDock(pWindow) )
			{
				//Destroy empty platform window if not main window
				if ( !(*it)->IsMain() && (*it)->GetContainer()->IsEmpty() )
				{
					m_lToDestroyPlatformWindows.push_back(*it);
				}
				return;
			}
		}

		if ( NULL != m_pDragPlatformWindow )
		{
			m_pDragPlatformWindow->UnDock(pWindow);
		}
	}

	void ImwWindowManager::OnClosePlatformWindow(ImwPlatformWindow* pWindow)
	{
		if (NULL != pWindow && !pWindow->m_pContainer->HasUnclosableWindow())
		{
			PlatformWindowAction* pAction = new PlatformWindowAction();
			pAction->m_iFlags = E_PLATFORM_WINDOW_ACTION_DESTROY;
			pAction->m_pPlatformWindow = pWindow;
			m_lPlatformWindowActions.push_back(pAction);
		}
	}

	void ImwWindowManager::DrawWindowArea( ImwPlatformWindow* pWindow, const ImVec2& oPos, const ImVec2& oSize, const ImColor& oColor )
	{
		m_lDrawWindowAreas.push_back(DrawWindowAreaAction(pWindow, oPos, oSize, oColor));
	}

	// Static
	ImwWindowManager* ImwWindowManager::GetInstance()
	{
		return s_pInstance;
	}

}
