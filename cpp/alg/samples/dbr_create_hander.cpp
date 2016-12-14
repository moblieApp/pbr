#include <iostream>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <fstream>

#include "drawbook.h"

using namespace std;

int main(int argc, char *argv[]) {
    std::cout << "create handler"  << std::endl;

    sn_handle_t classifier_handle = nullptr;   
    const char *model_path = "/home/app/sn_drawbook_sdk_v0.0.2_linux_x64/models/sn_drawbook_classification_v2.model";
    const char *license_path = "/home/app/sn_drawbook_sdk_v0.0.2_linux_x64/resource/license.dat";
    int device_id = -1; 
    int batch_size = 1;
    int dims;

    if (sn_drawbook_create_classifier(
                &classifier_handle,
                model_path, license_path, device_id, batch_size) != SN_OK) {
        std::cerr << "Failed to create handle" << std::endl;
        return -1;
    }
    std::cout << "Successed to create handle." << std::endl;


    sn_drawbook_destroy_classifier(classifier_handle);

    return 0;
}
