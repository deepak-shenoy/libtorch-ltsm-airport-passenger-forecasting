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

#include "utils/Plotter.h"

#define DATA_FILE "../.artifacts/data/airline-passengers-sample.txt"
#define OUTPUT_01 "../.artifacts/output/results/01-passenger-input-data.png"
#define OUTPUT_02 "../.artifacts/output/results/02-forecast-only.png"
#define LOOPBACKW 4

int main() {
    DebugTimer timer;
    timer.start();

    // Read data
    DataManager data_manager{};
    record raw_data = data_manager.read_file(DATA_FILE);

    // Plot what's read in
    Plotter plotter{};
    std::vector<float> x;
    for (int i = 0; i < raw_data.count.size(); i ++) x.push_back(i);
    plotter.draw_plot(OUTPUT_01,
        "Passenger Input Data",
        "Month",
        "Number of Passengers",
        "Passengers",
        x,
        raw_data.count
        );

    const int lookback = LOOPBACKW;

    // Training data
    torch::Tensor X_train, y_train;
    int start_0 = 0 , end_0 = raw_data.count.size() * 0.67;
    data_manager.create_data_sets(raw_data, lookback, start_0, end_0, X_train, y_train);
    auto train_dataset = torch::data::datasets::TensorDataset({X_train, y_train});
    torch::data::DataLoaderOptions options;
    options.batch_size(8); // options.batch_size(8).shuffle(true);
    auto data_loader = torch::data::make_data_loader(std::move(train_dataset), options);

    // Test data
    torch::Tensor X_test, y_test;
    int start_1 = end_0, end_1 = raw_data.count.size();
    data_manager.create_data_sets(raw_data, lookback, start_1, end_1, X_test, y_test);

    // Model infrastructure
    auto passenger_model = std::make_shared<PassengerModel>();
    torch::optim::Adam optimizer(passenger_model->parameters(), torch::optim::AdamOptions(0.001)); // Learning rate 0.001 is standard default
    torch::nn::MSELoss loss_fn;

    // Training
    int n_epochs = 8000;
    for (int epoch = 0; epoch < n_epochs; ++epoch) {
        passenger_model->train(true);
        for (auto &batch : *data_loader) {
            auto X_batch = batch.data();
            auto y_batch = batch[1];

            optimizer.zero_grad();
            torch::Tensor y_pred = passenger_model->forward(*X_batch);
            torch::Tensor loss = loss_fn(y_pred, y_batch);
            loss.backward();
            optimizer.step();
        }

        // Validation
        if (epoch % 500 != 0) continue;
        passenger_model->eval();
        torch::NoGradGuard no_grad;

        // Train RSME
        torch::Tensor y_pred_train = passenger_model->forward(X_train);
        torch::Tensor train_loss = loss_fn(y_pred_train, y_train);
        float train_rmse = std::sqrt(train_loss.item<float>());

        // Test RSME
        torch::Tensor y_pred_test = passenger_model->forward(X_test);
        torch::Tensor test_loss = loss_fn(y_pred_test, y_test);
        float test_rmse = std::sqrt(test_loss.item<float>());

        std::cout << "Epoch " << epoch << ": train RMSE " << std::fixed << std::setprecision(4)
                  << train_rmse << ", test RMSE " << test_rmse << std::endl;
    }

    passenger_model->eval();
    torch::Tensor y_pred_test = passenger_model->forward(X_test);

    int length_of_data = end_1 - start_1;
    std::vector<float> x_vals;
    for (int i = 0; i < length_of_data; i ++) {
        x_vals.push_back(static_cast<float>(i));
    }

    std::vector<float> y_vals;
    for (int i = 0; i < end_1 - start_1; i ++) {
        // TODO - Use an assessor
        for (int j = 0 ; j < 1; j++) {
            // std::cout << y_pred_test[i][j][0] << " ";
            y_vals.push_back(y_pred_test[i][0][0].item<float>());
        }
        std::cout << std::endl;
    }

    plotter.draw_plot(OUTPUT_02,
        "Forecast",
        "Month",
        "Number of Passengers",
        "Passengers",
        x_vals,
        y_vals
    );

    std::cout << "Completed in " << timer.stop() << "seconds" << std::endl;
    return 0;
}