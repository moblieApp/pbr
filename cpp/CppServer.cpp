/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>  
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <stdio.h>  
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <chrono>
#include <queue> 

#include "../gen-cpp/Calculator.h"
#include "./utils/base64.h"
#include "drawbook.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace tutorial;
using namespace shared;

queue<sn_handle_t> handle_queue;
//sn_handle_t classifier_handle = nullptr;
const char *model_path = "/home/app/sn_drawbook_sdk_v0.0.2_linux_x64/models/sn_drawbook_classification_v6.model";
//const char *model_path = "/home/app/sn_drawbook_sdk_v0.0.2_linux_x64/models/sn_drawbook_classification_v4.model";
const char *license_path = "/home/app/sn_drawbook_sdk_v0.0.2_linux_x64/resource/license.dat";
int device_id = 0;
int batch_size = 1;
int dims;

template <class Type>
Type stringToNum(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}
int ReadLines(const std::string& files_txt, std::vector<std::string>& files, std::vector<int>& labels, std::string tokens) {
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
            boost::split(elems, strLine, boost::is_any_of(tokens));
            files.push_back(elems[1]);
            labels.push_back(boost::lexical_cast<int>(elems[0]));
    }
}

class CalculatorHandler : public CalculatorIf {
public:
  CalculatorHandler() {
  }

  void ping() { cout << "ping()" << endl; }

  int32_t add(const int32_t n1, const int32_t n2) {
    cout << "add(" << n1 << ", " << n2 << ")" << endl;
    return n1 + n2;
  }
  
  void addpicture(std::string& _return, const std::string& imagefile, const int32_t cover) {
    sn_handle_t classifier_handle = nullptr;
    std::cout << "start to create handle " << " device_id:" << device_id << std::endl;
    //if(classifier_handle == nullptr){
    if (sn_drawbook_create_classifier(
                &classifier_handle,
                model_path, license_path, device_id, batch_size) != SN_OK) {
        std::cerr << "Failed to create handle" << std::endl;
    }
    //}
    //sn_handle_t classifier_handle = nullptr; 
    //cout << "queue.lenth:" << handle_queue.size() << " device_id:" << device_id << endl;
    //if(!handle_queue.empty()){
    //	classifier_handle = handle_queue.front();
    //    handle_queue.pop();
    //}else{
    //    cout << "no handle in queue!" << endl;
    //    _return = "ERROR:no handle in queue!";
    //}
    cout << "cover:" << cover << endl;
    string decoded = base64_decode(imagefile);
    
    time_t t;  //秒时间  
    tm* local; //本地时间   
    char now[128]= {0};
    t = time(NULL);  
    local = localtime(&t);  
    strftime(now, 64, "%Y-%m-%d-%H:%M:%S", local);  
    cout << "now:" << now << endl;
    char day[128]= {0};
    strftime(day, 64, "%Y-%m-%d", local);  

    //保存图片================================================
    boost::uuids::uuid aUuid = boost::uuids::random_generator()();
    string imagePath = "/home/app/userimage/image/" + boost::lexical_cast<string>(day) + "/" + boost::lexical_cast<string>(now) + boost::lexical_cast<string>(aUuid) + ".jpg";
    cout << "imagePath:" << imagePath << endl;
    ofstream ouF;  
    ouF.open(imagePath.c_str(), std::ofstream::binary);  
    ouF.write(decoded.c_str(), sizeof(char)*(decoded.size()));  
    ouF.close();

    //记录日志
    string resultPath = "/home/app/userimage/result/" + boost::lexical_cast<string>(day) + ".txt";
    string result = boost::lexical_cast<string>(now) + boost::lexical_cast<string>(aUuid) + ".jpg ";
    
    //start 处理识别=============================================================
    cv::Mat img_src = cv::imread(imagePath);
    if (!img_src.data) {
        std::cerr << "Failed to load image" << std::endl;
        _return = "ERROR:Failed to load image!";
        
        //handle_queue.push(classifier_handle);
        //cout << "queue.lenth0:" << handle_queue.size() << endl;
    	sn_drawbook_destroy_classifier(classifier_handle);
	return;
    }
    img_src = img_src.t();

    sn_image_t image = {img_src.data, img_src.rows, img_src.cols,
            img_src.step, SN_PIX_FMT_BGR888};
    sn_drawbook_get_feature_dims(classifier_handle, &dims);
    sn_drawbook_type classes;
    classes.data_feature = new float[dims];
 
    char coveridentifystart[128]= {0};
    t = time(NULL);  
    local = localtime(&t);  
    strftime(coveridentifystart, 64, "%Y-%m-%d-%H:%M:%S", local);  
    cout << "******coveridentifystart:" << coveridentifystart << endl;
   
    clock_t t1 = clock(); 
    //for (int i = 0; i < 1; i++) {
    if (sn_drawbook_classify(classifier_handle, &image, &classes) != SN_OK) {
            std::cerr << "Failed to recognition the image." << std::endl;
    	_return = "ERROR:Failed to recognition the image!";
    
    	//handle_queue.push(classifier_handle);
    	//cout << "queue.lenth1:" << handle_queue.size() << endl;
    	sn_drawbook_destroy_classifier(classifier_handle);
    	return;
    }
    //}
    std::cout << "isCover:" << classes.prediction_cover << ", score: " << classes.confidence_cover << std::endl;
    delete classes.data_feature;
    
    std::cout << "cover-cost " << 1.0 * (clock() - t1) / CLOCKS_PER_SEC << " sec" << std::endl;
    char coveridentifyend[128]= {0};
    t = time(NULL);  
    local = localtime(&t);  
    strftime(coveridentifyend, 64, "%Y-%m-%d-%H:%M:%S", local);  
    cout << "******coveridentifyend:" << coveridentifyend << endl;
 
    int isCover = classes.prediction_cover;
    int prediction = classes.prediction_page;
    int confidence = classes.confidence_page;
    std::cout << "****************isCover:" << isCover <<" prediction::" << classes.prediction_page <<" score:"<< classes.confidence_page << std::endl;

    string cover_path = "/home/app/cover/cover.txt";
    std::vector<std::string> files_vec;
    std::vector<int> labels_vec;
    ReadLines(cover_path, files_vec, labels_vec, " ");
    cout << "#####################:" << labels_vec.size()  << endl;
    vector<int>::iterator it;
    it = find(labels_vec.begin(), labels_vec.end(), classes.prediction_page); 
    if (it != labels_vec.end()) {
        isCover = 1;
	cout << "#####################:" << *it  << endl;
    } else {
	isCover = -100;
    }


    if(isCover == 1){
	//记录返回结果日志=================================================== 
	result += " is_cover:1  predict_id:" +  boost::lexical_cast<string>(classes.prediction_page) + " score: " +  boost::lexical_cast<string>(classes.confidence_page);
	ofstream f1(resultPath.c_str(), ios::app);
	if(f1){
	        f1 << result << std::endl;
	        f1.close();
	}
    	if(classes.confidence_page > 0.9){
        	_return = "{\"iscover\":1, \"prediction\":" + boost::lexical_cast<string>(classes.prediction_page) + ",\"confidence\":" + boost::lexical_cast<string>(classes.confidence_page) + "}";
		//_return = "CLASS: 1CLASS: " + boost::lexical_cast<string>(classes.prediction_page) + "CLASS: " + boost::lexical_cast<string>(classes.confidence_page);
        	
        	//handle_queue.push(classifier_handle);
        	//cout << "queue.lenth2:" << handle_queue.size() << endl;
    		sn_drawbook_destroy_classifier(classifier_handle);
        	return;
    	}
    } else {
	//记录返回结果日志=================================================== 
	result += " is_cover:0  predict_id:" +  boost::lexical_cast<string>(classes.prediction_page) + " score: " +  boost::lexical_cast<string>(classes.confidence_page);
	ofstream f1(resultPath.c_str(), ios::app);
	if(f1){
	        f1 << result << std::endl;
	        f1.close();
	}
    	//if(classes.confidence_page > 0.99){
        //	_return = "{\"iscover\":0, \"prediction\":" + boost::lexical_cast<string>(classes.prediction_page) + ",\"confidence\":" + boost::lexical_cast<string>(classes.confidence_page) + "}";
    	//	sn_drawbook_destroy_classifier(classifier_handle);
        //	return;
    	//}

    }
    result = boost::lexical_cast<string>(now) + boost::lexical_cast<string>(aUuid) + ".jpg ";
    
    if(cover < -1){
        _return = "{\"iscover\":-1,\"prediction\": -1,\"confidence\":-1}";
	//记录返回结果日志=================================================== 
	result += " 不是首页，也没有输入首页的cover值";
	ofstream f1(resultPath.c_str(), ios::app);
	if(f1){
	        f1 << result << endl;
	        f1.close();
	}
        //handle_queue.push(classifier_handle);
        //cout << "queue.lenth3:" << handle_queue.size() << endl;
    	sn_drawbook_destroy_classifier(classifier_handle);
        return;
    }

    //识别内容页===========================================================
    char contentreadstart[128]= {0};
    t = time(NULL);  
    local = localtime(&t);  
    strftime(contentreadstart, 64, "%Y-%m-%d-%H:%M:%S", local);  
    cout << "******contentreadstart:" << contentreadstart << endl;
    
    int image_number = 10;
    sn_drawbook_type *type_array = new sn_drawbook_type[image_number];
    std::vector<std::string> files_gallery;
    std::vector<int> labels_gallery;
    std::vector<cv::Mat> imgs_gallery;
    string feaPath = "/home/app/feas/" + boost::lexical_cast<string>(cover) +".txt";
    std::cout << "feaPath:" << feaPath << std::endl;
    fstream _file;
    _file.open(feaPath, ios::in);
    if(!_file){
        _return = "{\"iscover\":-2,\"prediction\": -1,\"confidence\":-1}",
	//记录返回结果日志=================================================== 
	result += " 没有对应绘本的特征文件" + feaPath;
	ofstream f1(resultPath.c_str(), ios::app);
	if(f1){
	        f1 << result << endl;
	        f1.close();
	}
        
        //handle_queue.push(classifier_handle);
        //cout << "queue.lenth4:" << handle_queue.size() << endl;
    	sn_drawbook_destroy_classifier(classifier_handle);
        return;
    }

    ReadLines(feaPath, files_gallery, labels_gallery, ":");

    image_number = files_gallery.size();
    type_array = new sn_drawbook_type[image_number];

    std::cout << "image_number:" << image_number << std::endl;

    for (int i = 0; i < image_number; i++) {
        type_array[i].data_feature = new float[dims];

        vector<string> tokens;
        boost::split(tokens, files_gallery[i], boost::is_any_of(";"));
        for (size_t j = 0; j < tokens.size(); ++j){
              type_array[i].data_feature[j] = stringToNum<float>(tokens[j]);
        }

        type_array[i].prediction_page = labels_gallery[i];
        type_array[i].confidence_page = 1.0;
    }
    char contentidentifystart[128]= {0};
    t = time(NULL);  
    local = localtime(&t);  
    strftime(contentidentifystart, 64, "%Y-%m-%d-%H:%M:%S", local);  
    cout << "******contentidentifystart:" << contentidentifystart << endl;
    
    image.data = img_src.data;
    image.cols = img_src.cols;
    image.rows = img_src.rows;
    image.stride = img_src.step;
    image.pixel_format = SN_PIX_FMT_BGR888;
    classes.data_feature = new float[dims];

    clock_t t2 = clock(); 
    if(sn_drawbook_retrieve(classifier_handle, &image, files_gallery.size(), type_array, &classes) != SN_OK) {
        std::cerr << "Failed to retrieve image..." << std::endl;
        delete classes.data_feature;
        _return = "ERROR:Failed to retrieve image...";
        
        //handle_queue.push(classifier_handle);
        //cout << "queue.lenth5:" << handle_queue.size() << endl;
    	sn_drawbook_destroy_classifier(classifier_handle);
	return;
    }
    std::cout << "CLASS: 0"  << "CLASS: " << classes.prediction_page << "CLASS: " << classes.confidence_page << std::endl;
    std::cout << "content-cost " << 1.0 * (clock() - t2) / CLOCKS_PER_SEC << " sec" << std::endl;
    
    char contentidentifyend[128]= {0};
    t = time(NULL);  
    local = localtime(&t);  
    strftime(contentidentifyend, 64, "%Y-%m-%d-%H:%M:%S", local);  
    cout << "******contentidentifyend:" << contentidentifyend << endl;
    
    //记录返回结果日志=================================================== 
    result += "input_cover: " + boost::lexical_cast<string>(cover) + " is_cover:0  predict_id:" +  boost::lexical_cast<string>(classes.prediction_page) + " score: " +  boost::lexical_cast<string>(classes.confidence_page);
    ofstream f1(resultPath.c_str(), ios::app);
    if(f1){
            f1 << result << endl;
            f1.close();
    }
    if(classes.confidence_page > 0.50){
    	if(cover == classes.prediction_page){
    		_return = "{\"iscover\": 1,\"prediction\":" + boost::lexical_cast<string>(classes.prediction_page) + ",\"confidence\":" + boost::lexical_cast<string>(classes.confidence_page) + "}";
    	}else{
    		_return = "{\"iscover\": 0,\"prediction\":" + boost::lexical_cast<string>(classes.prediction_page) + ",\"confidence\":" + boost::lexical_cast<string>(classes.confidence_page) + "}";
    	}
     }else{
        _return = "{\"iscover\":-3,\"prediction\": -1,\"confidence\":-1}";
    }

    delete classes.data_feature;

    for (int i = 0; i < image_number; i++) {
        delete type_array[i].data_feature;
    }

    delete[] type_array;

    //handle_queue.push(classifier_handle);
    //cout << "queue.lenth6:" << handle_queue.size() << endl;
    sn_drawbook_destroy_classifier(classifier_handle);

    return;
 
    //FILE *fp = popen("cd /home/app/sn_drawbook_sdk_v0.0.2_linux_x64/bin && ./test_dbr_recoall_server.bin ~/images/zh20161209/106zhizhuodeaiyinsitan/photo/0/IMG_19700103_040226.jpg -1 && cd -", "r");
    /*string cmd = "cd /home/app/sn_drawbook_sdk_v0.0.2_linux_x64/bin && ./test_dbr_recoall_server.bin " + imagePath + " " + boost::lexical_cast<string>(cover) +  " && cd -";
    cout << "cmd:" << cmd << endl;
    FILE *fp = popen(cmd.c_str(), "r");
    if(fp == NULL){
	return;
    }

    string retAll = "";
    char line[10240];
    while(fgets(line, 10240, fp) != NULL) {
	cout << line;
	retAll += boost::lexical_cast<string>(line) + "\n";
        string::size_type idx = boost::lexical_cast<string>(line).find("CLASS");
	if ( idx != string::npos ) {
		cout << "字符串含有" << line << "\n";
		ret = line;

		string resultPath = "/home/app/userimage/result/" + boost::lexical_cast<string>(day) + ".txt";
		ofstream f1(resultPath.c_str());	
		if(f1){
        		string result = boost::lexical_cast<string>(now) + boost::lexical_cast<string>(aUuid) + ".jpg ";
			result += " is_cover : predict_id: score: ";
			result += line;
			f1 << result << endl;
			f1.close();
		}
	}
   }
   pclose(fp); 

   if(ret == "") {
	ret = "ERROR:" + retAll;
   }
   _return = ret;*/
   cout << "return:" << _return << endl;
  }

  int32_t calculate(const int32_t logid, const Work& work) {
    cout << "calculate(" << logid << ", " << work << ")" << endl;
    int32_t val;

    switch (work.op) {
    case Operation::ADD:
      val = work.num1 + work.num2;
      break;
    case Operation::SUBTRACT:
      val = work.num1 - work.num2;
      break;
    case Operation::MULTIPLY:
      val = work.num1 * work.num2;
      break;
    case Operation::DIVIDE:
      if (work.num2 == 0) {
        InvalidOperation io;
        io.whatOp = work.op;
        io.why = "Cannot divide by 0";
        throw io;
      }
      val = work.num1 / work.num2;
      break;
    default:
      InvalidOperation io;
      io.whatOp = work.op;
      io.why = "Invalid Operation";
      throw io;
    }

    SharedStruct ss;
    ss.key = logid;
    ss.value = to_string(val);

    log[logid] = ss;

    return val;
  }

  void getStruct(SharedStruct& ret, const int32_t logid) {
    cout << "getStruct(" << logid << ")" << endl;
    ret = log[logid];
  }

  void zip() { cout << "zip()" << endl; }

protected:
  map<int32_t, SharedStruct> log;
};

/*
  CalculatorIfFactory is code generated.
  CalculatorCloneFactory is useful for getting access to the server side of the
  transport.  It is also useful for making per-connection state.  Without this
  CloneFactory, all connections will end up sharing the same handler instance.
*/
class CalculatorCloneFactory : virtual public CalculatorIfFactory {
 public:
  virtual ~CalculatorCloneFactory() {}
  virtual CalculatorIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
  {
    boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
    cout << "Incoming connection\n";
    cout << "\tSocketInfo: "  << sock->getSocketInfo() << "\n";
    cout << "\tPeerHost: "    << sock->getPeerHost() << "\n";
    cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
    cout << "\tPeerPort: "    << sock->getPeerPort() << "\n";
    return new CalculatorHandler;
  }
  virtual void releaseHandler( ::shared::SharedServiceIf* handler) {
    delete handler;
  }
};

int main() {
  TThreadedServer server(
    boost::make_shared<CalculatorProcessorFactory>(boost::make_shared<CalculatorCloneFactory>()),
    boost::make_shared<TServerSocket>(9090), //port
    boost::make_shared<TBufferedTransportFactory>(),
    boost::make_shared<TBinaryProtocolFactory>());

  /*
  // if you don't need per-connection state, do the following instead
  TThreadedServer server(
    boost::make_shared<CalculatorProcessor>(boost::make_shared<CalculatorHandler>()),
    boost::make_shared<TServerSocket>(9090), //port
    boost::make_shared<TBufferedTransportFactory>(),
    boost::make_shared<TBinaryProtocolFactory>());
  */

  /**
   * Here are some alternate server types...

  // This server only allows one connection at a time, but spawns no threads
  TSimpleServer server(
    boost::make_shared<CalculatorProcessor>(boost::make_shared<CalculatorHandler>()),
    boost::make_shared<TServerSocket>(9090),
    boost::make_shared<TBufferedTransportFactory>(),
    boost::make_shared<TBinaryProtocolFactory>());

  const int workerCount = 4;

  boost::shared_ptr<ThreadManager> threadManager =
    ThreadManager::newSimpleThreadManager(workerCount);
  threadManager->threadFactory(
    boost::make_shared<PlatformThreadFactory>());
  threadManager->start();

  // This server allows "workerCount" connection at a time, and reuses threads
  TThreadPoolServer server(
    boost::make_shared<CalculatorProcessorFactory>(boost::make_shared<CalculatorCloneFactory>()),
    boost::make_shared<TServerSocket>(9090),
    boost::make_shared<TBufferedTransportFactory>(),
    boost::make_shared<TBinaryProtocolFactory>(),
    threadManager);
  */
  
  //for(int i = 0; i < 3; i++) {
  //        sn_handle_t classifier_handle = nullptr; 
  //        std::cout << "start to create handle " << i << " device_id:" << device_id << std::endl;
  //        if (sn_drawbook_create_classifier(
  //      	      &classifier_handle,
  //      	      model_path, license_path, device_id, batch_size) != SN_OK) {
  //            std::cerr << "Failed to create handle" << i << std::endl;
  //        }
  //        std::cout << "Successed to create handle." << i << std::endl;
  //        handle_queue.push(classifier_handle);
  //}
  
  cout << "Starting the server..." << endl;
  server.serve();
  cout << "Done." << endl;
  return 0;
}
