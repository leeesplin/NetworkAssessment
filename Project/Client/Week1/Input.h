#pragma once

class Input
{
private:
	// structure to represent mouse
	struct Mouse
	{
		int x, y;
		bool left, right;
	};

public:
	void setKeyDown(int key);
	void setKeyUp(int key);
	bool isKeyDown(int key);

	void setMouseX(int lx);
	void setMouseY(int ly);
	void setMousePosition(int lx, int ly);
	int getMouseX();
	int getMouseY();
	void setMouseLeftDown(bool down);
	bool isMouseLeftDown();
	void setMouseRightDown(bool down);
	bool isMouseRightDown();

private:
	bool keys[256]{ false };
	Mouse mouse;
};