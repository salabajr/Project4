//
//  geodb.h
//  Proj4
//
//  Created by Nathan Wei on 3/7/24.
//

#ifndef geodb_h
#define geodb_h
#include "base_classes.h"
#include "hashmap.h"
#include <iostream>
#include <fstream>

class GeoDatabase: public GeoDatabaseBase
{
public:
    GeoDatabase();
    virtual ~GeoDatabase() {}
    virtual bool load(const std::string& map_data_file);
    virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
    virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
    virtual std::string get_street_name(const GeoPoint& pt1,const GeoPoint& pt2) const;

private:
    // maps name of POI to GeoPoints
    HashMap<GeoPoint> POItoGeo;
    // maps GeoPoints to vector containing connected GeoPoints
    HashMap<std::vector<GeoPoint>> GEOtoConnected;
    HashMap<std::string> GEOtoStreet;
    void addRelation(const GeoPoint startSeg, const GeoPoint endSeg, const std::string streetName, const std::vector<GeoPoint> poiGeo);
};

#endif /* geodb_h */
