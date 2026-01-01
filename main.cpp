/*
 * LibTorch LTSM Forecasting
 * Deepak Shenoy
 * December 2025
 *
 */

#include <torch/torch.h>
#include <torch/data/datasets/base.h>
#include <torch/data/dataloader.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#include "utils/DebugTimer.h"
#include "utils/DataManager.h"
#include "model/PassengerModel.h"

#include <torch/data/datasets/base.h>
#include <torch/data/dataloader.h>

#include "utils/plot.h"

#define DATA_FILE "../.artifacts/data/airline-passengers-sample.txt"
#define OUTPUT_01 "../.artifacts/output/results/01-passenger-input-data.png"

int main() {
    DebugTimer timer;
    timer.start();

    DataManager data_manager{};
    record raw_data = data_manager.read_file(DATA_FILE);

    plotcpp::Plot plt{};
    plt.SetTerminal("png");
    plt.SetOutput(OUTPUT_01);
    plt.SetTitle("Passenger Input Data");
    plt.SetXLabel("Month");
    plt.SetYLabel("Passengers");
    plt.SetAutoscale();
    plt.GnuplotCommand("set grid");

    std::vector<float> x;
    for (int i = 0; i < raw_data.count.size(); i ++) x.push_back(i);

    plt.Draw2D(plotcpp::Lines(x.begin(), x.end(), raw_data.count.begin(), "Passengers"),
             plotcpp::Points(x.begin(), x.end(), raw_data.count.begin(), ""));

    std::cout << "Completed in " << timer.stop() << "seconds" << std::endl;
    // Main template
    return 0;
}