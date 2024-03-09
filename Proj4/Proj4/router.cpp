//
//  Router.cpp
//  Proj4
//
//  Created by Nathan Wei on 3/8/24.
//

#include "router.h"
#include "geotools.h"
#include <vector>
#include <queue>
#include <functional>
#include "HashMap.h"

using namespace std;

Router::Router(const GeoDatabaseBase& geo_db) : m_geo_db(geo_db) {}

vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const
{
    HashMap<GeoPoint> cameFrom;
    HashMap<double> g;
    auto comp = [&](const GeoPoint& left, const GeoPoint& right)
    {
        double leftf_n = *g.find(left.to_string()) + distance_earth_miles(left, pt2);
        double rightf_n = *g.find(right.to_string()) + distance_earth_miles(right, pt2);
        return leftf_n > rightf_n;
    };
    priority_queue<GeoPoint, vector<GeoPoint>, decltype(comp)> openSet(comp);
    
    openSet.push(pt1);
    g.insert(pt1.to_string(), 0.0);
    
    while (!openSet.empty())
    {
        GeoPoint current = openSet.top();
        openSet.pop();
        // equal to the end
        if (current.to_string() == pt2.to_string())
        {
            return reconstruct_path(cameFrom, current);
        }
        
        auto neighbors = m_geo_db.get_connected_points(current);
        for (const auto& neighbor : neighbors)
        {
            double tentative_GScore = *g.find(current.to_string()) + distance_earth_miles(current, neighbor);
            if (g.find(neighbor.to_string())  == nullptr || tentative_GScore < *g.find(neighbor.to_string()))
            {
                cameFrom[neighbor.to_string()] = current;
                g[neighbor.to_string()] = tentative_GScore;
                openSet.push(neighbor);
            }
        }
    }
    return {};
}

vector<GeoPoint> Router::reconstruct_path(HashMap<GeoPoint>& cameFrom, GeoPoint current) const
{
        vector<GeoPoint> path;
        while (cameFrom.find(current.to_string())) 
        {
            path.push_back(current);
            current = *cameFrom.find(current.to_string());
        }
        path.push_back(current);
        reverse(path.begin(), path.end());
        return path;
}

