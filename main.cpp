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
#include <fstream>
#include <string>
#include <plot.h>

#include "utils/DebugTimer.h"

int main() {
    DebugTimer timer;
    timer.start();
    std::cout << "Completed in " << timer.stop() << "seconds" << std::endl;
    // Main template
    return 0;
}