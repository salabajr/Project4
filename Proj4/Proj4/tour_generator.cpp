//
//  TourGenerator.cpp
//  Proj4
//
//  Created by Nathan Wei on 3/9/24.
//

#include "tour_generator.h"
#include "geotools.h"
using namespace std;

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) : m_geodb(geodb), m_router(router)
{}

vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const
{
    vector<TourCommand> instructions_result(0);
    int i = 0;
    for (; i < stops.size() ; i++)
    {
        TourCommand commentary;
        string poiName;
        string poi_talkingpoints;
        if (stops.get_poi_data(i, poiName, poi_talkingpoints))
        {
            commentary.init_commentary(poiName, poi_talkingpoints);
            instructions_result.push_back(commentary);
            GeoPoint pt1;
            m_geodb.get_poi_location(poiName, pt1);
            string poiName2;
            string poi_talkingpoints2;
            // we can get to the next point
            if (stops.get_poi_data(i+1, poiName2, poi_talkingpoints2))
            {
                GeoPoint pt2;
                m_geodb.get_poi_location(poiName2, pt2);
                vector<GeoPoint> path = m_router.route(pt1, pt2);
                for (int j = 1; j < path.size() ; j++)
                {
                    if (j == 1)
                    {
                        GeoPoint subPt1 = path[j-1];
                        GeoPoint subPt2 = path[j];
                        string firstPath = m_geodb.get_street_name(subPt1, subPt2);;
                        double pathAngle = angle_of_line(subPt1, subPt2);
                        string direction;
                        double distance = distance_earth_miles(subPt1, subPt2);
                        compass(direction, pathAngle);
                        TourCommand proceed;
                        proceed.init_proceed(direction, firstPath, distance, subPt1, subPt2);
                        instructions_result.push_back(proceed);
                        continue;
                    }
                    
                    string prevPathName = m_geodb.get_street_name(path[j-2], path[j-1]);
                    // the two points share the same path
                    GeoPoint subPt1 = path[j-2];
                    GeoPoint subPt2 = path[j-1];
                    GeoPoint subPt3 = path[j];
                    double currentPathAngle = angle_of_line(subPt2, subPt3);
                    string direction;
                    double distance = distance_earth_miles(subPt2, subPt3);
                    compass(direction, currentPathAngle);
                    
                    string currentPathName = m_geodb.get_street_name(subPt2, subPt3);
                    if (prevPathName !=  currentPathName && angle_of_turn(subPt1, subPt2, subPt3) <= 359 && angle_of_turn(subPt1, subPt2, subPt3) >= 1)
                    {
                        double angle2 = angle_of_turn(subPt1, subPt2, subPt3);
                        TourCommand turn;
                        string turnDirection;
                        if (angle2 >= 1 && angle2 < 180)
                            turnDirection = "left";
                        if (angle2 >= 180 && angle2 <= 359)
                            turnDirection = "right";
                        turn.init_turn(turnDirection, m_geodb.get_street_name(subPt2, subPt3));
                        instructions_result.push_back(turn);
                    }
                    
                    TourCommand proceed;
                    proceed.init_proceed(direction, currentPathName, distance, subPt2, subPt3);
                    instructions_result.push_back(proceed);
                }
            }
        }
    }
    return instructions_result;
}

void TourGenerator::compass(string& direction, const double& angle) const
{
    if (angle >= 0 && angle < 22.5)
        direction = "east";
    else if (angle >= 22.5 && angle < 67.5)
        direction = "northeast";
    else if (angle >= 67.5 && angle < 112.5)
        direction = "north";
    else if (angle >= 112.5 && angle < 157.5)
        direction = "northwest";
    else if (angle >= 157.5 && angle < 202.5)
        direction = "west";
    else if (angle >= 202.5 && angle < 247.5)
        direction = "southwest";
    else if (angle >= 247.5 && angle < 292.5)
        direction = "south";
    else if (angle >= 292.5 && angle < 337.5)
        direction = "southeast";
    else if (angle >= 337.5)
        direction = "east";
    return;
}
