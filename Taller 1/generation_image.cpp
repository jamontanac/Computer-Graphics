#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <string>
#include <omp.h>

std::random_device rd;
std::mt19937 seq(rd());
int restriction(int val, int min_val, int max_val) {
    return std::max(std::min(val, max_val), min_val);
}

int generate_color(int mean, double std_dev) {
    std::normal_distribution<> d(mean, std_dev);
    int color = d(seq);
    return restriction(color, 0, 255);
}

void fill_random_matrix(int*** my_array, int width, int height, int mean, double std_dev) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            my_array[i][j][0] = generate_color(mean, std_dev);
            my_array[i][j][1] = generate_color(mean, std_dev);
            my_array[i][j][2] = generate_color(mean, std_dev);
        }
    }
}

int main(int argc, char* argv[]) {
    //In case openmp is used when compiling
    #ifdef _OPENMP
        int num_threads = omp_get_max_threads() / 2;
        omp_set_num_threads(num_threads);
    #endif

    if(argc < 5) {
        std::cout << "Usage: <output_filename> <width> <height> <percentage>..." << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    int width = std::stoi(argv[2]);
    int height = std::stoi(argv[3]);
    double* percentages = new double[argc - 4];

    for(int i = 4; i < argc; i++) {
        percentages[i - 4] = std::stod(argv[i]);
    }

    // Allocating memory for a 3D array
    int*** final = new int**[height];
    for(int i = 0; i < height; i++) {
        final[i] = new int*[width];
        for(int j = 0; j < width; j++) {
            final[i][j] = new int[3];
        }
    }

    std::ofstream f;
    f.open(filename);
    f << "P3\n" << width << " " << height << "\n255\n";

    int mean = 255 / 2;
    double std_dev = 100;

    fill_random_matrix(final, width, height, mean, std_dev);
    double cumulative_percentage = 1.0;

    // Updating the array

    for(int i = 0; i < argc - 4; i++) {
        cumulative_percentage *= percentages[i];
        int w = static_cast<int>(width * cumulative_percentage);
        int h = static_cast<int>(height * cumulative_percentage);
        std_dev = (100 * cumulative_percentage);
        fill_random_matrix(final, w, h, mean, std_dev);
    }
    // Writing the final image

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            f << final[i][j][0] << " " << final[i][j][1] << " " << final[i][j][2] << "\t";
        }
        f << "\n";
    }

    f.close();

    // Free memory
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            delete[] final[i][j];
        }
        delete[] final[i];
    }
    delete[] final;
    delete[] percentages;

    return 0;
}