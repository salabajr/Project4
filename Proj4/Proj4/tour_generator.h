//
//  TourGenerator.hpp
//  Proj4
//
//  Created by Nathan Wei on 3/9/24.
//

#ifndef TourGenerator_hpp
#define TourGenerator_hpp
#include "base_classes.h"
#include <iostream>
#include <vector>
class TourGenerator: public TourGeneratorBase
{
public:
 TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
    virtual ~TourGenerator() {}
 virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
    void compass(std::string& direction, const double& angle) const;
    const RouterBase& m_router;
    const GeoDatabaseBase& m_geodb;
};

#endif /* TourGenerator_hpp */
