/// **triangle side**
///
/// for adding, finding, removing, etc star pairs to a triangle side.
///
#ifndef TRIANGLE_SIDE_H
#define TRIANGLE_SIDE_H
#include <unordered_map>
#include "pairs_over_whole_sky.h"

namespace rules {

class TriangleSide {

public:
    TriangleSide();
    TriangleSide(double ang, double tol_radius, rules::PairsOverWholeSky& pairs);

private:

    /// **stars** each star is a map key, containing a map of twins, paired stars
    ///
    using inner_map = std::unordered_map<int, int>;
    using outer_map = std::unordered_map<int, inner_map>;
    outer_map stars;
    // inner_map &pairs = stars->second;
    // for (auto &star : stars) {
    //   for (auto &pair : star.second) {

    void add_pair(int star1, int star2);

};

}

#endif // TRIANGLE_SIDE_H