#include "linreg_predictor.h"

#include <numeric>
#include <cassert>
#include <stdexcept>


LinregPredictor::LinregPredictor(const std::vector<double>& coef)
    : coef_{coef}
{
}

double LinregPredictor::predict(const features& feat) const {
    if (feat.size() + 1 != coef_.size()) {
        throw std::invalid_argument{"Incorrect feature vector size"};
    }
    return  std::inner_product(feat.begin(), feat.end(), ++coef_.begin(), coef_.front());
}
