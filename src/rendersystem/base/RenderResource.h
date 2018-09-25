#ifndef RENDERRESOURCE_H_
#define RENDERRESOURCE_H_

//------------------------------------------------------------------------------
namespace RenderBase
{
class RenderResource 
{
public:
    /// resource usage flags
    enum Usage
    {
        UsageImmutable,      //> can only be read by GPU, not written, cannot be accessed by CPU
        UsageDynamic,        //> can only be read by GPU, can only be written by CPU
        UsageCpu,            //> a resource which is only accessible by the CPU and can't be used for rendering
    };

    // cpu access flags
    enum Access
    {
        AccessNone,         // CPU does not require access to the resource (best)
        AccessWrite,        // CPU has write access
        AccessRead,         // CPU has read access
        AccessReadWrite,    // CPU has read/write access
    };

    // mapping types
    enum MapType
    {
        MapRead,                // gain read access, must be UsageDynamic and AccessRead
        MapWrite,               // gain write access, must be UsageDynamic and AccessWrite
        MapReadWrite,           // gain read/write access, must be UsageDynamic and AccessReadWrite
        MapWriteDiscard,        // gain write access, discard previous content, must be UsageDynamic and AccessWrite
        MapWriteNoOverwrite,    // gain write access, must be UsageDynamic and AccessWrite, see D3D10 docs for details
    };

    /// constructor
    RenderResource();
    /// destructor
    virtual ~RenderResource();
	virtual void OnDeviceLost();
	virtual void OnDeviceReset();

    /// set resource usage type
    void SetUsage(Usage usage);
    /// get resource usage type
    Usage GetUsage() const;
    /// set resource cpu access type
    void SetAccess(Access access);
    /// get cpu access type
    Access GetAccess() const;

protected:
	void copyFrom(const RenderResource& rr);
    Usage usage;
    Access access;
};

//------------------------------------------------------------------------------
/**
*/
inline void
RenderResource::SetUsage(Usage u)
{
    this->usage = u;
}

//------------------------------------------------------------------------------
/**
*/
inline RenderResource::Usage
RenderResource::GetUsage() const
{
    return this->usage;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderResource::SetAccess(Access a)
{
    this->access = a;
}

//------------------------------------------------------------------------------
/**
*/
inline RenderResource::Access
RenderResource::GetAccess() const
{
    return this->access;
}
//------------------------------------------------------------------------------
/**
*/
inline void
RenderResource::copyFrom(const RenderResource& rr)
{
	this->access = rr.access;
	this->usage = rr.usage;
}

} // namespace RenderBase
//------------------------------------------------------------------------------
#endif//RENDERRESOURCE_H_