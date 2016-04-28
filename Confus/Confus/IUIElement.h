#pragma once
class IUIElement
{
public:
	virtual void update() = 0;
	virtual ~IUIElement() = default;
};

