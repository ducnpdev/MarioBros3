#pragma once

class CNode;
typedef CNode* LPNODE;

class CNode
{
	int type;
	float top, right, left, bottom;

public:
	CNode(float l, float t, float r, float b, int type);
	float GetLeft() { return left; }
	float GetTop() { return top; }
	float GetRight() { return right; }
	float GetBottom() { return bottom; }
	int GetType() { return type; }
};
