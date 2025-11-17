#ifndef __SCREEN_MGR_H__
#define __SCREEN_MGR_H__

#include "Gui.h"

void screenMgrInit(void);
void screenMgrPostInit(void);

void screenMgrUpdate(int y_, int x_, int n_);//(GuiColor888 color);

#endif
