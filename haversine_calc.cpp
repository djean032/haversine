#include <math.h>

constexpr double PI = 3.1415926535897932384626433;

typedef double f64;

static f64 Square(f64 A)
{
    f64 result = A * A;
    return result;
}

static f64 Deg2Rad(f64 degrees)
{
    f64 result = PI * degrees / 180;
    return result;
}

static f64 RefHaversine(f64 x0, f64 y0, f64 x1, f64 y1, f64 EarthRadius)
{

    f64 lat1 = y0;
    f64 lat2 = y1;
    f64 lon1 = x0;
    f64 lon2 = x1;

    f64 dLat = Deg2Rad(lat2 - lat1);
    f64 dLon = Deg2Rad(lon2 - lon1);
    lat1 = Deg2Rad(lat1);
    lat2 = Deg2Rad(lat2);

    f64 a = Square(sin(dLat / 2.0)) + cos(lat1) * cos(lat2) * Square(sin(dLon / 2));

    f64 c = 2.0 * asin(sqrt(a));

    f64 Result = EarthRadius * c;

    return Result;
}
