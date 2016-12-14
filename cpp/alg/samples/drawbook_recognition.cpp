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

    //*********************** Test Batch classifiy************************
    
    std::cout << "Test batch interface." << std::endl;

    int image_number = 10;

    // p_image_array is a array of pointer that point to sn_image_t
    sn_image_t** p_image_array = new sn_image_t*[image_number];
    sn_drawbook_type *type_array = new sn_drawbook_type[image_number]; // 
    for (int i = 0; i < image_number; i++) {
        p_image_array[i] = new sn_image_t;
        p_image_array[i]->data = img_src.data;
        p_image_array[i]->rows = img_src.rows;
        p_image_array[i]->cols = img_src.cols;
        p_image_array[i]->stride = img_src.step;
        p_image_array[i]->pixel_format = SN_PIX_FMT_BGR888;
        type_array[i].data_feature = new float[dims];
    }

    if (sn_drawbook_batch_classify(classifier_handle, 
                p_image_array, image_number, type_array) != SN_OK) {
        std::cerr << "Failed to classify drawbook." << std::endl;
        return -1;
    }
    std::cout << "CLASS of second drawbook: " << type_array[1].prediction_page << ", score: " << type_array[1].confidence_page << std::endl;
    
    for (int i = 0; i < image_number; i++) {
        delete p_image_array[i];
        delete type_array[i].data_feature;
    }

    delete[] p_image_array;
    delete[] type_array;
    
    //*********************  Test retrieval *********************
    std::vector<std::string> files_gallery, files_test;
    std::vector<int> labels_gallery, labels_test;
    std::vector<cv::Mat> imgs_gallery;
    // 读取检索库和测试库的图像列表
    // 这个两个列表是在绘本little blue truck下photo目录随机抽取的，检索库列表每页15个，其他都测试用的
    ReadDbinfo("gallery.txt", files_gallery, labels_gallery);
    ReadDbinfo("test_imgs.txt", files_test, labels_test);

    image_number = files_gallery.size();
    p_image_array = new sn_image_t*[image_number];
    type_array = new sn_drawbook_type[image_number];

    for (int i = 0; i < image_number; i++) {
        imgs_gallery.push_back(cv::Mat());
        img_src = cv::imread("/home/app/images/littlebluetruck/photo/"+ files_gallery[i]);
        img_src.copyTo(imgs_gallery[i]);

        p_image_array[i] = new sn_image_t;
        p_image_array[i]->data = imgs_gallery[i].data;
        p_image_array[i]->rows = imgs_gallery[i].rows;
        p_image_array[i]->cols = imgs_gallery[i].cols;
        p_image_array[i]->stride = imgs_gallery[i].step;
        p_image_array[i]->pixel_format = SN_PIX_FMT_BGR888;
        type_array[i].data_feature = new float[dims];
    }
    
    // 先提取检索所有图像的特征
    if (sn_drawbook_batch_classify(classifier_handle, 
                p_image_array, image_number, type_array) != SN_OK) {
        std::cerr << "Failed to classify drawbook." << std::endl;
        return -1;
    }
    
    // 因为这本书里面都不在书库上，所以prediction_page, confidence_page是没用
    // 但是我们事前已经知道这本书所有页的ID，将其写入到以prediction_page, confidence_page即可
    for (int i = 0;i < image_number; i++) {
        type_array[i].prediction_page = labels_gallery[i];
        type_array[i].confidence_page = 1.0;
        //std::cout << labels_gallery[i] << ", " << type_array[i].prediction_cover << std::endl;
    }
    
    int count = 0;
    for (int i=0; i<files_test.size(); i++) {
        img_src = cv::imread("/home/app/images/littlebluetruck/photo/"+ files_test[i]);
        image.data = img_src.data;
        image.cols = img_src.cols;
        image.rows = img_src.rows;
        image.stride = img_src.step;
        image.pixel_format = SN_PIX_FMT_BGR888;
        classes.data_feature = new float[dims];

        //检索识别目标书页
        if(sn_drawbook_retrieve(classifier_handle, &image, files_gallery.size(), type_array, &classes) != SN_OK) {
            std::cerr << "Failed to retrieve image..." << std::endl;
            delete classes.data_feature;
            break;
        } 
        //common::Timer::Stop("Retrieving query image features");
        if (labels_test[i] == classes.prediction_page) {
            count++;
        }
        delete classes.data_feature;
    }
    
    std::cout << "Acuuracy: " << count << " / " << files_test.size() << std::endl;
    for (int i = 0; i < image_number; i++) {
        delete p_image_array[i];
        delete type_array[i].data_feature;
    }

    delete[] p_image_array;
    delete[] type_array;

    sn_drawbook_destroy_classifier(classifier_handle);

    return 0;
}
