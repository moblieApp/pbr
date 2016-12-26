#include <iostream>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "drawbook.h"

using namespace std;

template <class Type>  
Type stringToNum(const string& str)  
{  
    istringstream iss(str);  
    Type num;  
    iss >> num;  
    return num;      
}

int ReadDbinfo(const std::string& files_txt, std::vector<std::string>& files, std::vector<int>& labels) {
  char buf[256];
  std::ifstream fin;
  std::vector<std::string> elems;
  fin.open(files_txt.c_str(), std::ios::in);

  while(fin.eof() != true) {
    fin.getline(buf, 256);
    if (buf[0] == 0)
      break;
    
    std::cout << buf << std::endl;
    boost::split(elems, buf,boost::is_any_of(":"));
    files.push_back(elems[1]);
    labels.push_back(boost::lexical_cast<int>(elems[0]));

  }
  fin.close();
  return 0;
}
int ReadLines(const std::string& files_txt, std::vector<std::string>& files, std::vector<int>& labels) {
   char *filePath = (char*)files_txt.c_str();  
   ifstream file;  
   std::vector<std::string> elems;
   file.open(filePath,ios::in);  
  
   if(!file.is_open())  
 	return 0;  
      
   std::string strLine;  
   while(getline(file,strLine)) {  
            if(strLine.empty())  
                continue;  
	    boost::split(elems, strLine, boost::is_any_of(":"));
	    files.push_back(elems[1]);
	    labels.push_back(boost::lexical_cast<int>(elems[0]));
    }  
}
bool isNum(string str)  
{  
    stringstream sin(str);  
    double d;  
    char c;  
    if(!(sin >> d))  
        return false;  
    if (sin >> c)  
        return false;  
    return true;  
}

int main(int argc, char *argv[]) {
    std::cout << "argc:" << argc  << std::endl;
    //获取参数  
    if (argc < 3) {  
    	std::cout << "please input imagePath and cover number"  << std::endl;
    	return 0;
    }
    if(isNum(argv[2])){
    	//std::cout << "please input cover number is digit"  << std::endl;
    }

    sn_handle_t classifier_handle = nullptr;   
    const char *model_path = "/home/app/sn_drawbook_sdk_v0.0.2_linux_x64/models/sn_drawbook_classification_v4.model";
    const char *license_path = "/home/app/sn_drawbook_sdk_v0.0.2_linux_x64/resource/license.dat";
    int device_id = 0; 
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

    clock_t t = clock();
    for (int i = 0; i < 1; i++) {
        if (sn_drawbook_classify(classifier_handle, &image, &classes) != SN_OK) {
        	std::cerr << "Failed to recognition the image." << std::endl;
        return -1;
        }
    }
    std::cout << "step1-cost " << 1.0 * (clock() - t) / CLOCKS_PER_SEC << " sec" << std::endl;    
    std::cout << "isCover:" << classes.prediction_cover  << ", score: " << classes.confidence_cover << std::endl;
    delete classes.data_feature;

    int isCover = classes.prediction_cover;
    
    std::cout << "prediction_page: " << classes.prediction_page << " confidence_page: " << classes.confidence_page << std::endl;

    std::cout << "isCover:" << isCover <<":" << (isCover == 1) <<":"<< (isCover == '1') << std::endl;
    if(isCover == 1){
    	std::cout << "CLASS: 1"  << "CLASS: " << classes.prediction_page << "CLASS: " << classes.confidence_page << std::endl;
    	return 0;
    }
    
    
    int cover = atoi(argv[2]);
    if(cover < 0){
        std::cout << "CLASS: -1CLASS: -1CLASS: -1" << std::endl;
        return 0;
    }

    //*********************** Test Batch classifiy************************
    int image_number = 10;

    sn_drawbook_type *type_array = new sn_drawbook_type[image_number]; // 
    
    //*********************  Test retrieval *********************
    std::vector<std::string> files_gallery;
    std::vector<int> labels_gallery;
    std::vector<cv::Mat> imgs_gallery;
    // 读取检索库和测试库的图像列表
    // 这个两个列表是在绘本little blue truck下photo目录随机抽取的，检索库列表每页15个，其他都测试用的
    string feaPath = "/home/app/feas/" + boost::lexical_cast<string>(cover) +".txt";
    std::cout << "fenPath:" << feaPath << std::endl;
    fstream _file;
    _file.open(feaPath, ios::in);
    if(!_file){
        std::cout << "CLASS: -2CLASS: -1CLASS: -1" << std::endl;
        return 0;
    }

    ReadLines(feaPath, files_gallery, labels_gallery);

    image_number = files_gallery.size();
    type_array = new sn_drawbook_type[image_number];
   
    std::cout << "image_number:" << image_number << std::endl;

    for (int i = 0; i < image_number; i++) {
        type_array[i].data_feature = new float[dims];
	
	vector<string> tokens;
        boost::split(tokens, files_gallery[i], boost::is_any_of(";"));
        for (size_t j = 0; j < tokens.size(); ++j){
	      type_array[i].data_feature[j] = stringToNum<float>(tokens[j]);
              //cout << tokens[j] << endl;
        }
	
        type_array[i].prediction_page = labels_gallery[i];
        type_array[i].confidence_page = 1.0;
    }
    
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
	return -1;
    } 
    //common::Timer::Stop("Retrieving query image features");
    std::cout << "CLASS: 0"  << "CLASS: " << classes.prediction_page << "CLASS: " << classes.confidence_page << std::endl;
    
    delete classes.data_feature;
    
    for (int i = 0; i < image_number; i++) {
        delete type_array[i].data_feature;
    }

    delete[] type_array;

    sn_drawbook_destroy_classifier(classifier_handle);

    return 0;
}
