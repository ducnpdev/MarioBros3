#include "Arrow.h"

void CArrow::SetState(int state)
{
}

void CArrow::Render()
{
}


void CListArrow::SetPositionListArrow(float x, float y)
{
	for (unsigned int i = 0; i < listArrow.size(); i++)
	{
		listArrow[i]->SetPosition(x + 8*i, y);
	}
}

void CListArrow::SetWhiteListArrow()
{
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
		if (GetPState()) SetPStart((DWORD)GetTickCount64());

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



