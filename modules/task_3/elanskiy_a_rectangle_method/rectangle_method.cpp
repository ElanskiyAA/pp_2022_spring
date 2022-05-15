// Copyright 2022 Elanskiy Alexander
#include "../../modules/task_3/elanskiy_a_rectangle_method/rectangle_method.h"

double rec_seq(std::vector<std::vector<int>> integrals, int step, const std::function<double(const double&)>& f) {
    int size_integ = integrals.size();
    std::vector<double> h;
    std::vector<double> res;

    for (int i = 0; i < size_integ; i++) {
        float tmp = integrals[i][1] - integrals[i][0];
        tmp = tmp / step;
        h.push_back(tmp);
        double sum = 0;
        for (int j = 0; j < step; j++) {
            sum += f(integrals[i][0] + j * h[i]) * h[i];
        }
        res.push_back(sum);
    }

    double result = 1;
    for (int i = 0; i != size_integ; i++) {
        result *= res[i];
    }
    result = round(result * 10000) / 10000;
    return result;
}

double rec_par(std::vector<std::vector<int>> integrals, int step, const std::function<double(const double&)>& f) {
    int size_integ = integrals.size();
    std::vector<double> h;
    std::vector<double> res;
    //  tbb::task_scheduler_init init(16);

    for (int i = 0; i < size_integ; i++) {
        float tmp = integrals[i][1] - integrals[i][0];
        tmp = tmp / step;
        h.push_back(tmp);
        double sum = tbb::parallel_reduce(
            tbb::blocked_range<int>(0, step), 0.0,
            [&f, &integrals, &h, &i](tbb::blocked_range<int> r, double tmp_sum) {
                for (int j = r.begin(); j < r.end(); j++) {
                    tmp_sum += f(integrals[i][0] + j * h[i]) * h[i];
                }
                return tmp_sum;
            }, std::plus<double>());
        res.push_back(sum);
    }

    double result = 1;
    for (int i = 0; i != size_integ; i++) {
        result *= res[i];
    }
    result = round(result * 10000) / 10000;
    return result;
}
