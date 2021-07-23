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
	return left; 
}
