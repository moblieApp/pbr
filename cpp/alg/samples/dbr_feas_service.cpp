#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include "drawbook.h"

using namespace std; 

#define PATHMAX 1024

template <class T>
int getArrayLen(T& array){
	return (sizeof(array) / sizeof(array[0]));
}

bool IsDir(std::string path) {  
    struct stat sb;  
    if (stat(path.c_str(), &sb) == -1) return false;  
    return S_ISDIR(sb.st_mode);  
}

bool IsFile(const string &path)
{
   struct stat st; 
   int ret = stat(path.c_str(), &st);
   return ret>=0 && S_ISREG(st.st_mode);
}

vector<string> getFiles(const string &path, const string &postfix)
{
   vector<string> files;
   struct dirent* ent = NULL;
   DIR* pDir;
   pDir = opendir(path.c_str());
   while(NULL != (ent = readdir(pDir)))
   {
	string fullpath = path + "/" + ent->d_name;
	if(IsFile(fullpath))
	{
		if(postfix == "" || strstr(ent->d_name, postfix.c_str())!=NULL)
		{
			files.push_back(ent->d_name);
		}
	}
    }
    closedir(pDir);
    std::cerr << files.size() << std::endl;
    return files;
}

vector<string> searchdir(const char *path)
{
    string feas;
    vector<string> feasv;

    DIR *dp;
    struct dirent *dmsg;
    int i=0;
    char addpath[PATHMAX] = {'\0'}, *tmpstr;
    if ((dp = opendir(path)) != NULL)
    {
      while ((dmsg = readdir(dp)) != NULL)
      {
        if (!strcmp(dmsg->d_name, ".") || !strcmp(dmsg->d_name, ".."))
            continue;
        strcpy(addpath, path);
        strcat(addpath, "/");
        strcat(addpath, dmsg->d_name);

        if (IsDir(addpath))
        {
            char *temp;
            temp=dmsg->d_name;
            if(strchr(dmsg->d_name, '.'))
            {
               if((strcmp(strchr(dmsg->d_name, '.'), dmsg->d_name)==0))
               {
                 continue;
               }
            }
            searchdir(addpath);
        }else{
	    feas.insert(i, addpath);
            feasv.push_back(addpath);
            i++;
        }
      }
    }
    closedir(dp);
    return feasv;
}

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
    time_t t;  //秒时间
    tm* local; //本地时间
    char now[128]= {0};
    
    sn_handle_t classifier_handle = nullptr;   
    const char *model_path = "/home/app/sn_drawbook_sdk_v0.0.2_linux_x64/models/sn_drawbook_classification_v2.model";
    const char *license_path = "/home/app/sn_drawbook_sdk_v0.0.2_linux_x64/resource/license.dat";
    int device_id = -1; 
    int batch_size = 1;
    int dims;
    
    t = time(NULL);
    local = localtime(&t);
    strftime(now, 64, "%Y-%m-%d-%H:%M:%S", local);
    cout << "start create:" << now << endl;
    if (sn_drawbook_create_classifier(
                &classifier_handle,
                model_path, license_path, device_id, batch_size) != SN_OK) {
        std::cerr << "Failed to create handle" << std::endl;
        return -1;
    }
    std::cout << "Successed to create handle." << std::endl;
    t = time(NULL);
    local = localtime(&t);
    strftime(now, 64, "%Y-%m-%d-%H:%M:%S", local);
    cout << "end create:" << now << endl;

    cv::Mat img_src;
    //cv::Mat img_src = cv::imread(argv[1]);
    //if (!img_src.data) {
    //    std::cerr << "Failed to load image" << std::endl;
    //    return -1;
    //}

    //sn_image_t image = {img_src.data, img_src.rows, img_src.cols,
    //        img_src.step, SN_PIX_FMT_BGR888};

    sn_drawbook_get_feature_dims(classifier_handle, &dims);

    sn_drawbook_type classes;
    classes.data_feature = new float[dims];

    //delete classes.data_feature;

    //*********************** Test Batch classifiy************************
    
    std::cout << "Test batch interface." << std::endl;

    int image_number = 10;
    double start, end, cost;

    // p_image_array is a array of pointer that point to sn_image_t
    sn_image_t** p_image_array = new sn_image_t*[image_number];
    sn_drawbook_type *type_array = new sn_drawbook_type[image_number]; // 
    
    //*********************  Test retrieval *********************
    std::vector<std::string> files_gallery;
    std::vector<int> labels_gallery;
    std::vector<cv::Mat> imgs_gallery;
    // 读取检索库和测试库的图像列表
    // 这个两个列表是在绘本little blue truck下photo目录随机抽取的，检索库列表每页15个，其他都测试用的
    vector<string> files = searchdir("/home/app/relation");
    size_t len = files.size();
    std::cerr << "len:" << len << std::endl;
    for (size_t j =0; j < len; j ++) {
        string file = files[j];
        std::cerr << file << std::endl;
	files_gallery.clear();
	labels_gallery.clear();
	imgs_gallery.clear();
	
        t = time(NULL);
        local = localtime(&t);
        strftime(now, 64, "%Y-%m-%d-%H:%M:%S", local);
	cout << "readDb & this time start:" << now << endl;
    	ReadDbinfo(file, files_gallery, labels_gallery);
        t = time(NULL);
        local = localtime(&t);
        strftime(now, 64, "%Y-%m-%d-%H:%M:%S", local);
	cout << "readDb end:" << now << endl;
        
	image_number = files_gallery.size();
        p_image_array = new sn_image_t*[image_number];
	type_array = new sn_drawbook_type[image_number];
    	for (int i = 0; i < image_number; i++) {
		imgs_gallery.push_back(cv::Mat());
		img_src = cv::imread(files_gallery[i]);
		img_src.copyTo(imgs_gallery[i]);

		p_image_array[i] = new sn_image_t;
		p_image_array[i]->data = imgs_gallery[i].data;
		p_image_array[i]->rows = imgs_gallery[i].rows;
		p_image_array[i]->cols = imgs_gallery[i].cols;
		p_image_array[i]->stride = imgs_gallery[i].step;
		p_image_array[i]->pixel_format = SN_PIX_FMT_BGR888;
		type_array[i].data_feature = new float[dims];
    	}

	
        t = time(NULL);
        local = localtime(&t);
        strftime(now, 64, "%Y-%m-%d-%H:%M:%S", local);
	cout << "agl start:" << now << endl;
	start = clock();
    	// 先提取检索所有图像的特征
    	if (sn_drawbook_batch_classify(classifier_handle, 
        	        p_image_array, image_number, type_array) != SN_OK) {
        	std::cerr << "Failed to classify drawbook." << std::endl;
        	return -1;
    	}

	end = clock();
	cost = (double)(end - start) / CLOCKS_PER_SEC;
        t = time(NULL);
        local = localtime(&t);
        strftime(now, 64, "%Y-%m-%d-%H:%M:%S", local);
	cout << "agl end:" << now << endl;
    	for (int i = 0;i < image_number; i++) {
        	type_array[i].prediction_page = labels_gallery[i];
        	type_array[i].confidence_page = 1.0;
    	}
	cout << "cost:" << cost << endl;
	cout << "image_number:" << image_number << endl;
	cout << "prediction_page:" << type_array[0].prediction_page << endl;
	cout << "dims:" << dims << endl;
	
	string feasPath = "/home/app/feas/" + file.substr(file.rfind('/')+1); 
	cout << feasPath << endl;  

	int lenFeature = 0;	
	ofstream outfile;  
	outfile.open("/home/app/feas/" + file.substr(file.rfind('/')+1), ios::ate);
	if(outfile.is_open()) {
    		for (int i = 0; i < image_number; i++) {
			string message = boost::lexical_cast<string>(labels_gallery[i]) + ":";
			
			for(int k = 0; k < dims; k++){
				message += boost::lexical_cast<string>(type_array[i].data_feature[k]) + ";";
			}
			message=message.substr(0, message.length() - 1);
			//cout << message << endl;  
			outfile<<message<<endl;
		}
	}else{  
		cout << "can not open!" << endl;  
	}
	outfile.close();   
        
	t = time(NULL);
        local = localtime(&t);
        strftime(now, 64, "%Y-%m-%d-%H:%M:%S", local);
	cout << "this time end:" << now << endl;
    }

    for (int i = 0; i < image_number; i++) {
        delete p_image_array[i];
        delete type_array[i].data_feature;
    }

    delete[] p_image_array;
    delete[] type_array;

    sn_drawbook_destroy_classifier(classifier_handle);

    return 0;
}
