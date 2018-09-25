
#include "stdneb.h"
#include "math/plane.h"
#include "math/matrix44.h"

namespace Math
{

//------------------------------------------------------------------------------
/**
*/
plane
plane::transform(const matrix44& m, __PlaneArg p)
{
	return matrix44::transform(m, p);
    //return XMPlaneTransform(p.vec, m.mx);
}

} // namespace Math