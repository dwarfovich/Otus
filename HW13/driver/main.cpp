#include "linreg_predictor.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <limits>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

using Row    = std::vector<double>;
using Matrix = std::vector<Row>;

Matrix readCoefficientsMatrix(const std::filesystem::path& file, const std::string& delimiter)
{
    std::ifstream stream { file };
    std::string   line;
    Matrix        m;
    
    while (std::getline(stream, line)) {
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(delimiter));
        Row row(tokens.size());
        std::transform(tokens.cbegin(), tokens.cend(), row.begin(), boost::lexical_cast<double, std::string>);
        m.push_back(std::move(row));
       
    }

    return m;
}

std::pair<std::vector<int>, Matrix> readTestData(const std::filesystem::path& file, const std::string& delimiter){
    std::ifstream stream { file };
    std::string   line;
    Matrix        m;
    std::vector<int> classes;
    std::size_t      counter = 0;
    while (std::getline(stream, line)) {
        auto pos = line.find(delimiter);
        classes.push_back(std::stoi(line.substr(0, pos)));
        line.erase(line.begin(), line.begin() + pos + 1);
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(delimiter));
        Row row(tokens.size());
        std::transform(tokens.cbegin(), tokens.cend(), row.begin(), boost::lexical_cast<double, std::string>);
        m.push_back(std::move(row));
        if (counter % 1000 == 0) {
            std::cout << "Reading case " << counter << '\n';
        }
        ++counter;
    }

    return {std::move(classes), std::move(m)};
}

class Model{
public:
    Model(Matrix matrix) {
        for (auto& row : matrix){
            predictors_.push_back(std::make_unique<LinregPredictor>(std::move(row)));
        }
    }

    int predictClass(const Row& row){
        double probability    = std::numeric_limits<double>::lowest();
        int predictedClass = -1;
        for(int i = 0; i < predictors_.size(); ++i){
            auto result = predictors_[i]->predict(row);
            if(result > probability){
                probability = result;
                predictedClass = i;
            }

        }

        return predictedClass;
    }

        private:
            std::vector<std::unique_ptr<LinregPredictor>> predictors_;
};

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Wrong parameters count\n";
        return -1;
    }

    std::filesystem::path testFile { argv[1] };
    std::filesystem::path modelFile { argv[2] };

    std::cout << "Reading test data...\n";
    auto [classes, data]  = readTestData(testFile, ",");
    std::cout << "Creating model...\n";
    Model model { readCoefficientsMatrix(modelFile, " ") };

    std::cout << "Inference...\n";
    std::vector<int> predictions;
    std::size_t counter = 0;
    for(const auto& row : data){
        predictions.push_back(model.predictClass(row));
        if(counter % 1000 == 0){
            std::cout << "Testing case " << counter << '\n';
        }
        ++counter;
    }

    std::size_t errors = 0;
    for(std::size_t i = 0; i < predictions.size(); ++i){
        if(predictions[i] != classes[i]){
            ++errors;
        }
    }

    double accuracy = errors == 0 ? 100. : (predictions.size() - errors)*100./predictions.size();
    std::cout << "Accuracy: " << accuracy << '\n';
    return 0;
}