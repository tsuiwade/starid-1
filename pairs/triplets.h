#ifndef TRIPLETS_H
#define TRIPLETS_H

#include "../base/sensor.h"

namespace pairs {

struct Triplet {
    arma::vec uva;
    arma::vec uvb;
    arma::vec uvc;
    double ab;
    double ac;
    double bc;
};

class Triplets
{
public:
    Triplets();
    Triplets(base::Obs& obs, int ntrip);
    Triplet GetTriplet();
    bool IsMoreTriplets();
private:
    uint n;
    uint curtriplet;
    arma::mat mata;
    arma::mat matb;
    arma::mat matc;
};

}

#endif