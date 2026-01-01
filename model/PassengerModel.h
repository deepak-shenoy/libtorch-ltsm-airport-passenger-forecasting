//
// Created by Deepak Shenoy on 1/1/26.
//

#ifndef AIRPORT_PASSENGER_FORECASTING_PASSENGERMODEL_H
#define AIRPORT_PASSENGER_FORECASTING_PASSENGERMODEL_H

#include <torch/torch.h>

class PassengerModel : torch::nn::Module {
public:
    torch::nn::LSTM lstm;
    torch::nn::Linear linear;

    PassengerModel()
        // Input size 1, Hidden size 50, Num layers 1, batch_first=True is default in C++ LSTMOptions
        : lstm(torch::nn::LSTMOptions(1, 50).num_layers(1).batch_first(true)),
          linear(50, 1) {
        // Register modules to track parameters
        register_module("lstm", lstm);
        register_module("linear", linear);
    }

    // torch::Tensor forward(torch::Tensor x) {
    //     // LSTM output returns a tuple of (output, (hidden, cell))
    //     x = std::get<0>(lstm(x));
    //     x = linear(x);
    //     return x;
    // }
    torch::Tensor forward(torch::Tensor x) {
        torch::Tensor output;
        std::tie(output, std::ignore) = lstm->forward(x);

        // Pass the output through the linear layer.
        x = linear(output);
        return x;
    }
};

#endif //AIRPORT_PASSENGER_FORECASTING_PASSENGERMODEL_H