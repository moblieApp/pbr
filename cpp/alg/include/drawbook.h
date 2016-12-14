//
// File name: drawbook.h
// Created by yangyansheng@sensenets.com on 16-9-20.
// Copyright (c) 2016 SenseNets. All rights reserved.
//

#ifndef SN_DRAWBOOK_DRAWBOOK_H
#define SN_DRAWBOOK_DRAWBOOK_H

#include "common.h"

/// 绘本的类别结果
/// prediction_page  预测页ID
/// prediction_cover 预测是否为封面(1为封面，0为内页)
/// confidence_page  页识别置信度
/// confidence_cover 封面识别置信度
/// feature          识别图像特征
typedef struct _sn_drawbook_type {
    int prediction_page;
    int prediction_cover;
    float confidence_page;
    float confidence_cover;
    float * data_feature;
} sn_drawbook_type;

/// @brief 创建绘本分类句柄
/// @param[out] handle 函数成功时，将被设置为有效的绘本分类句柄
/// @param[in] model_path 深度模型的路径
/// @param[in] device_id 用于计算的GPU设备序号，如果为负数，则使用CPU进行计算
/// @param[in] batch_size 用于指定每批图像中的图像数
/// @return 成功返回SN_OK，否则返回错误类型
SN_API sn_result_t sn_drawbook_create_classifier(
    sn_handle_t *handle,
    const char *model_path,
    const char *license_path,
    int device_id,
    unsigned int batch_size
);

/// @breif 用于获取最大支持的batch size的值
/// @return 返回最大支持的batch size的值
SN_API unsigned int sn_drawbook_get_max_batch_size();

/// @brief 销毁绘本分类句柄
/// @param[in] classifier_handle 分类的句柄
SN_API void sn_drawbook_destroy_classifier(
    sn_handle_t classifier_handle
);

/// @breif 用于获取模型特征的维度，以便于用户分配特征的存储空间
/// @param[in] classifier_handle 分类的句柄
/// @param[out] dim 特征的维度
/// @return 返回当前使用模型特征的维度
SN_API sn_result_t sn_drawbook_get_feature_dims(
    sn_handle_t classifier_handle, int * dim);

/// @brief 绘本分类
/// @param[in] classifier_handle 绘本分类的句柄
/// @param[in] image 用于分类的图像指针
/// @param[in] drawbook_type 类别结果
/// @return 成功返回SN_OK，否则返回错误类型
SN_API sn_result_t sn_drawbook_classify(
    sn_handle_t classifier_handle,
    const sn_image_t *image,
    sn_drawbook_type* drawbook_type
);

/// @brief 绘本分类批量处理接口
/// @param[in] classifier_handle 绘本分类的句柄
/// @param[in] image 用于分类的图像指针数组
/// @param[in] image_number 图像指针数组的长度
/// @param[in] drawbook_type_array 分类得到的结果数组,存储空间由外部分配
/// @return 成功返回SN_OK，否则返回错误类型
SN_API sn_result_t sn_drawbook_batch_classify(
    sn_handle_t classifier_handle,
    const sn_image_t * const *image,
    const int image_number,
    sn_drawbook_type * drawbook_type_array
);

/// @brief 绘本检索识别 通过检索的方式识别来目标书页
/// @param[in] classifier_handle 绘本分类的句柄
/// @param[in] image 用于分类的图像指针数组
/// @param[in] gallery_size 检索库的图像数量
/// @param[in] drawbook_type_array 分类得到的结果数组
/// @param[out] drawbook_type 通过检索得到识别结果, 存储空间由外部分配
/// @return 成功返回SN_OK，否则返回错误类型
SN_API sn_result_t sn_drawbook_retrieve(
    sn_handle_t classifier_handle,
    const sn_image_t * image,
    const int gallery_size,
    const sn_drawbook_type * drawbook_type_array,
    sn_drawbook_type * drawbook_type
);

#endif // SN_DRAWBOOK_DRAWBOOK_H
