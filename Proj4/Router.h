//
//  Router.hpp
//  Proj4
//
//  Created by Nathan Wei on 3/8/24.
//

#ifndef Router_hpp
#define Router_hpp
#include "HashMap.h"
#include "base_classes.h"

class Router: public RouterBase
{
public:
 Router(const GeoDatabaseBase& geo_db);
 virtual ~Router() {}
 virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
    const GeoDatabaseBase& m_geo_db;
    std::vector<GeoPoint> reconstruct_path(HashMap<GeoPoint>& cameFrom, GeoPoint current) const;
};

#endif /* Router_hpp */
