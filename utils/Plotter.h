//
// Created by Deepak Shenoy on 1/1/26.
//

#ifndef AIRPORT_PASSENGER_FORECASTING_PLOTTER_H
#define AIRPORT_PASSENGER_FORECASTING_PLOTTER_H

#include "plot.h"

class Plotter {
public:
    void draw_plot(std::string output_file,
        std::string title,
        std::string x_axis_description,
        std::string y_axis_description,
        std::string y_line_description,
        std::vector<float> x, std::vector<float> y) {
        plotcpp::Plot plt{};
        plt.SetTerminal("png");
        plt.SetOutput(output_file);
        plt.SetTitle(title);
        plt.SetXLabel(x_axis_description);
        plt.SetYLabel(y_axis_description);
        plt.SetAutoscale();
        plt.GnuplotCommand("set grid");
        plt.Draw2D(plotcpp::Lines(x.begin(), x.end(), y.begin(), y_line_description),
                 plotcpp::Points(x.begin(), x.end(), y.begin(), ""));
    }
};


#endif //AIRPORT_PASSENGER_FORECASTING_PLOTTER_H