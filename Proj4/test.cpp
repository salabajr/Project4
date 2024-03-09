//
//  test.cpp
//  Proj4
//
//  Created by Nathan Wei on 3/8/24.
//

#include "HashMap.h"
#include "geodb.h"
#include "Router.h"
#include "geotools.h"
using namespace std;

int main()
{
    /*
     // Define a hashmap that maps strings to doubles and has a maximum
     // load factor of 0.2. It will initially have 10 buckets when empty.
     HashMap<double> nameToGPA(1.0);
     // Add new items to the hashmap
     nameToGPA.insert("Carey", 3.5); // Carey has a 3.5 GPA
     nameToGPA.insert("David", 2.99); // David needs to up his game
     nameToGPA.insert("Dawei", 2.19); // David needs to up his game
     nameToGPA.insert("Divik", 4.0); // David needs to up his game
     
     
     // You can also use brackets like C++'s unordered_map!
     nameToGPA["Annie"] = 3.85; // Adds Annie, who has the highest GPA of all.
     // Since adding Annie would push the load factor from 0.2 to 0.3, exceeding the
     // maximum load factpr of 0.2, adding Annie caused the hashmap to increase the
     // number of buckets, forcing a rehash of all items.
     double* davidsGPA = nameToGPA.find("David");
     if (davidsGPA != nullptr)
     *davidsGPA = 3.1; // after a re-grade of David's exam, update 2.99 -> 3.1
     nameToGPA.insert("Carey", 4.0); // Carey deserves a 4.0
     // sees if linda is in the map; if not, creates a new entry for linda in map
     cout << nameToGPA["Linda"]; // prints zero
     */
    string pathName = "/Users/nathanwei/Desktop/mapdata.txt";
    GeoDatabase a;

     cerr << a.load(pathName) << endl;
/*
     vector<GeoPoint> hi = a.get_connected_points(midpoint(GeoPoint("34.0620596", "-118.4467237"), GeoPoint("34.0613323", "-118.4461140")));
     for (int i = 0; i < hi.size(); i++)
     cerr << hi[i].to_string() << endl;
     GeoPoint point = GeoPoint("34.0620596", "-118.4467237");
     if (a.get_poi_location("Fox Theatersad", point))
     cerr << "Diddy: " << point.to_string();
     */
    Router E = Router(a);
 //   vector<GeoPoint> fast = E.route(GeoPoint("34.0630614", "-118.4468781") , GeoPoint("34.0614911", "-118.4464410"));
    vector<GeoPoint> path2 = E.route(GeoPoint("34.0720093",  "-118.4458886") , GeoPoint("34.0630614", "-118.4468781"));
    
    for (int i = 0; i < path2.size(); i++)
        cerr << path2[i].to_string() << endl;
    
    
}
