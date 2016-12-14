#include <iostream>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "drawbook.h"

int ReadDbinfo(const std::string& files_txt, std::vector<std::string>& files,
  std::vector<int>& labels) {
  char buf[256];
  std::ifstream fin;
  std::vector<std::string> elems;
  fin.open(files_txt.c_str(), std::ios::in);

  while(fin.eof() != true) {
    fin.getline(buf, 256);
    if (buf[0] == 0)
      break;

    boost::split(elems, buf,boost::is_any_of(" "));
    files.push_back(elems[0]);
    labels.push_back(boost::lexical_cast<int>(elems[1]));

  }
  fin.close();
  return 0;
}

int main(int argc, char *argv[]) {

    sn_handle_t classifier_handle = nullptr;   
    const char *model_path = "../models/sn_drawbook_classification_v2.model";
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

    cv::Mat img_src = cv::imread(argv[1]);
    if (!img_src.data) {
        std::cerr << "Failed to load image" << std::endl;
        return -1;
    }

    sn_image_t image = {img_src.data, img_src.rows, img_src.cols,
            img_src.step, SN_PIX_FMT_BGR888};

    sn_drawbook_get_feature_dims(classifier_handle, &dims);

    sn_drawbook_type classes;
    classes.data_feature = new float[dims];

    for (int i = 0; i < 1; i++) {
        if (sn_drawbook_classify(classifier_handle, &image, &classes) != SN_OK) {
        std::cerr << "Failed to recognition the image." << std::endl;
        return -1;
        }
    }
    std::cout << "CLASS: " << classes.prediction_page << ", score: " << classes.confidence_page << std::endl;
    delete classes.data_feature;

    sn_drawbook_destroy_classifier(classifier_handle);

    return 0;
}
