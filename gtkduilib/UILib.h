#ifndef __UILIB__H__
#define __UILIB__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef WIN32
#include <Windows.h>
#else
#include "compat.h"
#endif

#include <gtk/gtk.h>
//#include <gdk-pixbuf/gdk-pixbuf.h>
//#include <pango/pango.h>
//#include <pango/pangocairo.h>


#include "UIUtils.h"
#include "UIString.h"
#include "UIDefine.h"
#include "NotifyPump.h"
#include "UIFont.h"
#include "UIEvent.h"
#include "UIRender.h"
#include "ResourceMgr.h"
#include "UIControl.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UIText.h"
#include "UIOption.h"
#include "UIProgress.h"
#include "UISlider.h"
#include "UIScrollBar.h"
#include "UIWindow.h"

#include "UIContainer.h"
#include "UIHorizontalLayout.h"
#include "UIVerticalLayout.h"
#include "UITabLayout.h"
#include "UITileLayout.h"
#include "DialogBuilder.h"
#include "UIChildLayout.h"

#include "UIList.h"

#include "WindowImpBase.h"

#endif