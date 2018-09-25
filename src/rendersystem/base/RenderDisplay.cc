

#include "stdneb.h"
#include "RenderDisplay.h"

namespace RenderBase
{
//------------------------------------------------------------------------------
/**
*/
RenderDisplay::RenderDisplay() :
    adapter(Adapter::Primary),
    antiAliasQuality(AntiAliasQuality::None),
    fullscreen(false),
	m_bWebVersion(false),
    modeSwitchEnabled(true),
    tripleBufferingEnabled(false),
    alwaysOnTop(false),
    verticalSync(true),
    isOpen(false),
    windowTitle("Genesis Application Window"),
    iconName("GenesisIcon"),
    parentWindow(0),
	m_hWebWindow(0),
    inNotifyEventHandlers(false)
{
    //__ConstructThreadSingleton;
}

//------------------------------------------------------------------------------
/**
*/
RenderDisplay::~RenderDisplay()
{
    assert(!this->IsOpen());
    //__DestructThreadSingleton;
}

//------------------------------------------------------------------------------
/**
    Set the window title. An application should be able to change the
    window title at any time, that's why this is a virtual method, so that
    a subclass may override it.
*/
void
RenderDisplay::SetWindowTitle(const std::string& str)
{
    this->windowTitle = str;
}

//------------------------------------------------------------------------------
/**
    Open the display.
*/
bool
RenderDisplay::Open()
{
    assert(!this->IsOpen());

    this->isOpen = true;
    return true;
}

//------------------------------------------------------------------------------
/**
    Close the display.
*/
void
RenderDisplay::Close()
{
    assert(this->IsOpen());

    this->isOpen = false;
}

//------------------------------------------------------------------------------
/**
    Process window system messages. Override this method in a subclass.
*/
void
RenderDisplay::ProcessWindowMessages()
{
	
}

//------------------------------------------------------------------------------
/**
    Returns the display modes on the given adapter in the given pixel format.
*/
std::vector<DisplayMode>
RenderDisplay::GetAvailableDisplayModes(Adapter::Code adapter, PixelFormat::Code pixelFormat)
{
    // override this method in a subclass to do something useful
    std::vector<DisplayMode> emptyArray;
    return emptyArray;
}

//------------------------------------------------------------------------------
/**
    This method checks the available display modes on the given adapter
    against the requested display modes and returns true if the display mode
    exists.
*/
bool
RenderDisplay::SupportsDisplayMode(Adapter::Code adapter, const DisplayMode& requestedMode)
{
    return false;
}

//------------------------------------------------------------------------------
/**
    This method returns the current adapter display mode. It can be used
    to get the current desktop display mode.
*/
DisplayMode
RenderDisplay::GetCurrentAdapterDisplayMode(Adapter::Code adapter)
{
    DisplayMode emptyMode;
    return emptyMode;
}

//------------------------------------------------------------------------------
/**
    Checks if the given adapter exists.
*/
bool
RenderDisplay::AdapterExists(Adapter::Code adapter)
{
    return false;
}

//------------------------------------------------------------------------------
/**
    Returns information about the provided adapter.
*/
AdapterInfo
RenderDisplay::GetAdapterInfo(Adapter::Code adapter)
{
    AdapterInfo emptyAdapterInfo;
    return emptyAdapterInfo;
}

//------------------------------------------------------------------------------
	/**
	*/
	Adapter::Code
		Adapter::FromString(const std::string& str)
	{
		if ("None" == str) return None;
		else if ("Primary" == str) return Primary;
		else if ("Secondary" == str) return Secondary;
		else
		{
			printf("Invalid adapter string '%s'!", str.c_str());
			assert(0);
			return Primary;
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	std::string
		Adapter::ToString(Code code)
	{
		switch (code)
		{
		case None:      return "None";
		case Primary:   return "Primary";
		case Secondary: return "Secondary";
		default:
			printf("Invalid adapter code!");
			assert(0);
			return "";
		}
	}
} // namespace RenderBase
