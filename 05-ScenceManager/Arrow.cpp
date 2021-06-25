#include "Arrow.h"

void CArrow::SetState(int state)
{
	CGameObject::SetState(state);
}

void CArrow::Render()
{
	int ani;
	if (!isBlack && !isP) ani = ARROW_ANI_WHITE;
	else if (isBlack && !isP) ani = ARROW_ANI_BLACK;
	else if (!isBlack && isP) ani = ARROW_ANI_WHITE_P;
	else if (isBlack && isP) ani = ARROW_ANI_BLACK_P;
	animation_set->at(ani)->Render(x, y);
}


void CListArrow::SetPositionListArrow(float x, float y)
{
	listArrow[0]->SetPosition(x, y);
	listArrow[1]->SetPosition(x + ARROW_POS_X_PLUS_8, y);
	listArrow[2]->SetPosition(x + ARROW_POS_X_PLUS_16, y);
	listArrow[3]->SetPosition(x + ARROW_POS_X_PLUS_24, y);
	listArrow[4]->SetPosition(x + ARROW_POS_X_PLUS_32, y);
	listArrow[5]->SetPosition(x + ARROW_POS_X_PLUS_40, y);
	listArrow[6]->SetPosition(x + ARROW_POS_X_PLUS_48, y);
}

void CListArrow::SetWhiteListArrow()
{
	// DebugOut(L"Set white arrow");
	for (unsigned int i = 0; i < listArrow.size(); i++)
	{
		if (listArrow[i]->GetArrowIsBlack())
		{
			listArrow[i]->SetWhiteState();
			break;
		}
	}
}

void CListArrow::SetBlackListArrow()
{
// 	DebugOut(L"Set black arrow");
	for (int i = listArrow.size() - 1; i >= 0; i--)
	{
		if (!listArrow[i]->GetArrowIsBlack())
		{
			listArrow[i]->SetBlackState();
			break;
		}
	}
}

// chuyển tất cả các mũi tên thành trạng thái ban đầu đó là màu đen
void CListArrow::ResetListArrow()
{
	// DebugOut(L"Reset arrow");
	// vòng for từ dưới lên vì mũi trên trắng sẽ chuyển sang màu đen từ phải sang trái
	for (int i = listArrow.size() - 1; i >= 0; i--)
	{
		if (!listArrow[i]->GetArrowIsBlack())
		{
			// set i trại vị trí i trong list arrow thành màu đen
			listArrow[i]->SetBlackState();
		}
	}
}
// 
int CListArrow::GetPState()
{
	return listArrow[6]->GetArrowIsBlack();
}

void CListArrow::Render()
{
}

void CListArrow::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// kiểm tra P có null or not null
	if (listArrow[6] != NULL)
	{
		// nếu mũi tên cuối cùng là mày trắng
		if (GetPState()) {
			SetPStart((DWORD)GetTickCount64());
		}
		// nếu chữ P là màu đen thì bắt đầu reset những mũi tên về trạng thái màu đen
		else if (!GetPState())
		{
			if (GetTickCount64() - timePStart > ARROW_RESET_P_TIME) ResetListArrow();
		}
	}
}

void CListArrow::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}



