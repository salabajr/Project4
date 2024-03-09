//
//  geodb.cpp
//  Proj4
//
//  Created by Nathan Wei on 3/7/24.
//

#include "geodb.h"
#include "HashMap.h"
#include "geotools.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;


GeoDatabase::GeoDatabase()
{
    HashMap<GeoPoint> POItoGeo;
    HashMap<Relation> GEOtoConnected;
}

bool GeoDatabase::load(const string& map_data_file)
{
    ifstream infile(map_data_file);    // infile is a name of our choosing
    if (!infile )                // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    string line;
    while (getline(infile, line))
    {
        string streetName = line;
        
        getline(infile, line);
        istringstream issGeoPoints(line);
        string start_latitude, start_longitude, end_latitude, end_longitude;
        issGeoPoints >> start_latitude >> start_longitude >> end_latitude >> end_longitude;
        GeoPoint start = GeoPoint(start_latitude, start_longitude);
        GeoPoint end = GeoPoint(end_latitude, end_longitude);
        
        getline(infile, line);
        istringstream issPOI(line);
        int p;
        issPOI >> p;
        
        vector<GeoPoint> poiGeo;
        for (int i = 0; i < p; i++)
        {
            getline(infile, line);
            istringstream issPOIInfo(line);
            string poiName;
            string poiLatitude, poiLongitude;
            
            getline(issPOIInfo, poiName, '|');
            issPOIInfo >> poiLatitude >> poiLongitude;
            
            GeoPoint POICoord = GeoPoint(poiLatitude, poiLongitude);
            POItoGeo.insert(poiName, POICoord);
            poiGeo.push_back(POICoord);
        }
        if (p > 0)
        {
            addRelation(start, end, streetName, poiGeo);
        }
        else
        {
            GEOtoConnected[start.to_string()].push_back(Relation(end, streetName));
            GEOtoConnected[end.to_string()].push_back(Relation(start, streetName));
        }
    }
    cerr << "successfully loaded data";
    return true;

}

void GeoDatabase::addRelation(const GeoPoint startSeg, const GeoPoint endSeg, const string streetName, const vector<GeoPoint> poiGeo)
{
    GeoPoint mid = midpoint(startSeg, endSeg);
    Relation endConnection = Relation(endSeg, streetName);
    Relation midConnection = Relation(mid, streetName);
    Relation startConnection = Relation(startSeg, streetName);
    
    GEOtoConnected[startSeg.to_string()].push_back(midConnection);
    
    GEOtoConnected[mid.to_string()].push_back(startConnection);
    GEOtoConnected[mid.to_string()].push_back(endConnection);
    for (int i = 0; i < poiGeo.size(); i++)
    {
        GEOtoConnected[mid.to_string()].push_back(Relation(poiGeo[i], "a path"));
        GEOtoConnected[poiGeo[i].to_string()].push_back(Relation(mid, "a path"));
    }
    
    GEOtoConnected[endSeg.to_string()].push_back(midConnection);
}

bool GeoDatabase::get_poi_location(const string& poi, GeoPoint& point) const
{
    const GeoPoint* ptr = POItoGeo.find(poi);
    if (ptr == nullptr)
        return false;
    else
    {
        point = *ptr;
        return true;
    }
}

vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
    vector<GeoPoint> connected_points(0);
    const vector<Relation>* ptr = GEOtoConnected.find(pt.to_string());
    if (ptr == nullptr)
        return connected_points;
    for (int i = 0; i < ptr->size(); i++)
    {
        connected_points.push_back((*ptr)[i].gp);
    }
    return connected_points;
}

string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const
{
    string streetName = "";
    const vector<Relation>* ptr = GEOtoConnected.find(pt1.to_string());
    if (ptr == nullptr)
        return streetName;
    for (int i = 0; i < ptr->size(); i++)
    {
        if ((*ptr)[i].gp.to_string() == pt2.to_string())
        {
            streetName = (*ptr)[i].path;
            return streetName;
        }
    }
    return streetName;
}
