#include "Node.h"
#include "Utils.h"

CNode::CNode(float l, float t, float r, float b, int p)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
	type = p;
}

float CNode::GetLeft()
{
	/*CNode* nodes;
	if (!nodes->left) {
		return 0.0f;
	}*/
	// DebugOut(L"sss \n");
	if (&left ==  nullptr) {
		return 0.0f;
	}
	return left; 
}
