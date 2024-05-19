#ifndef DRILLASSIGNATIONS_HPP
#define DRILLASSIGNATIONS_HPP
#include <vector>
#include <QPointF>

struct DrillAssignations
{
    double holeSize = 2.0f;
    std::vector<QPointF> coordinates;
public:
    DrillAssignations();
};


#endif // DRILLASSIGNATIONS_HPP
