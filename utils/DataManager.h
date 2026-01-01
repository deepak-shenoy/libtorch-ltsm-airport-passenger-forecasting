//
// Created by Deepak Shenoy on 12/31/25.
//

#ifndef AIRPORT_PASSENGER_FORECASTING_DATAMANAGER_H
#define AIRPORT_PASSENGER_FORECASTING_DATAMANAGER_H

namespace datamgr {
#include "DebugTimer.h"

#include <fstream>
#include <chrono>

    struct record {
        std::vector<int> year;
        std::vector<int> month;
        std::vector<double> count;
    };

    class DataManager {
    public:
        record read_file(std::string filename) {
            std::vector<int> year;
            std::vector<int> month;
            std::vector<double> count;
            std::string line;
            std::size_t line_size = 0, start_of_delimeter = 0, end_of_delimeter = 0;

            DebugTimer timer;
            timer.start();

            std::ifstream file_ptr(filename);
            if (!file_ptr.is_open()) throw std::runtime_error("Cannot open file");
            while(!file_ptr.eof()) {
                std::getline(file_ptr, line);

                start_of_delimeter = line.find('-');
                if (start_of_delimeter == 0 || start_of_delimeter == std::string::npos) {
                    printf("Bad line %s\n", line.c_str());
                    continue;
                }

                end_of_delimeter = line.find(',', start_of_delimeter);
                if (end_of_delimeter == std::string::npos) {
                    std::cout << "Bad line2: " << line << " Position " << end_of_delimeter << std::endl;
                    continue;
                }

                std::string year_raw = line.substr(0, start_of_delimeter);
                std::string month_raw = line.substr(start_of_delimeter + 1, 2);
                std::string count_raw = line.substr(end_of_delimeter + 1);

                year.push_back(std::stoi(year_raw));
                month.push_back(std::stoi(month_raw));
                count.push_back(std::stod(count_raw));

            }
            record results(year, month, count);

            int number_of_years_records = results.year.size();
            printf("Number of year records: %i \n", number_of_years_records);
            int number_of_test_year_records = number_of_years_records * 0.67;
            // printf("Number of test year records: %i \n", number_of_test_year_records);

            int number_of_month_records = results.month.size();
            printf("Number of month records: %i \n", number_of_month_records);
            int number_of_test_month_records = number_of_month_records * 0.67;
            // printf("Number of test month records: %i \n", number_of_test_month_records);

            int number_of_count_records = results.month.size();
            printf("Number of count records: %i\n", number_of_count_records);
            int number_of_test_count_records = number_of_count_records * 0.67;
            // printf("Number of test count records: %i \n", number_of_test_count_records);

            std::cout << "Read file in " << timer.stop() << "seconds" << std::endl;

            return results;
        }

        void create_data_sets(record results, int lookback, int start_index, int end_index, torch::Tensor &t_X, torch::Tensor &t_y) {
            DebugTimer timer;
            timer.start();
            auto options = torch::TensorOptions().dtype(torch::kFloat32).layout(torch::kStrided).device(torch::kCPU).requires_grad(false);
            t_X = torch::empty({end_index - start_index, lookback, 1}, options);
            t_y = torch::empty({end_index - start_index, lookback, 1}, options);
            for (int i = start_index; i < end_index - lookback; i ++) {
                for (int j = 0 ; j < lookback; j ++) {
                    t_X[i - start_index][j] = results.count.at(i + j );
                    t_y[i - start_index][j] = results.count.at(i + j + 1);
                };
            }
            std::cout << "Created dataset in " << timer.stop() << "seconds" << std::endl;
        }

        int get_file_size() {
            // Continue
            return 0;
        }
    };
}

#endif //AIRPORT_PASSENGER_FORECASTING_DATAMANAGER_H