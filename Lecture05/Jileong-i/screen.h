char ScreenBuffer[65536];

void setScreenSize(int width, int height);
void setCursorIcon(int isActive);
void setCursorPos(int x, int y);
void setCursorVisibility(int isVisible);
void SetColor(unsigned short backgroundColor, unsigned short textColor);
int getScreenWidth();
int getScreenHeight();
