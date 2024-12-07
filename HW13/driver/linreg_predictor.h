#pragma once

#include "predictor.h"

class LinregPredictor: public Predictor {
public:
    LinregPredictor(const std::vector<double>&);

    double predict(const features&) const override;
    
protected:
    std::vector<double> coef_;
};
