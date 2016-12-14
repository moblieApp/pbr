//
// File name: common.h
// Created by ronny on 16-8-4.
// Copyright (c) 2016 SenseNets. All rights reserved.
//

#ifndef SN_COMMON_COMMON_H
#define SN_COMMON_COMMON_H

#ifdef __cplusplus
    #define SN_API extern "C"
#else
    #define SN_API
#endif

typedef void *sn_handle_t;

typedef int sn_result_t;

#define SN_OK (0)
#define SN_E_INVALIDARG (-1)
#define SN_E_HANDLE (-2)
#define SN_E_OUTOFMEMORY (-3)
#define SN_E_FAIL (-4)
#define SN_E_DELNOTFOUND (-5)

#define SN_E_MODEL_FAIL_LOAD (-10)
#define SN_E_LICENSE_EXPIRE (-11)
#define SN_E_LICENSE_NOTEXIST (-12)
#define SN_E_LICENSE_INVAILD (-13)

#define SN_E_FEATURE_VERSION (-16)
#define SN_E_FEATURE_BAD (-17)
#define SN_E_BATCHSIZE (-20)
#define SN_E_UNSUPPORTED (-1000)

typedef struct _sn_rect_t {
    int x;
    int y;
    int width;
    int height;
} sn_rect_t;

typedef struct _sn_point2i_t {
    int x;
    int y;
} sn_point2i_t;

typedef struct _sn_point2f_t {
    float x;
    float y;
} sn_point2f_t;

typedef enum {
    SN_PIX_FMT_GRAY8,
    SN_PIX_FMT_YUV420P,
    SN_PIX_FMT_BGR888,
} sn_pixel_format;

/// 图像数据类型
typedef struct _sn_image_t {
    unsigned char *data;                 /// 数据指针
    int rows;                           /// 图像的行数
    int cols;                           /// 图像的列数
    int stride;                         /// 一行数据的字节数
    sn_pixel_format pixel_format;       /// 像素类型
} sn_image_t;

/// @brief 为图像分配内存
///
/// @param[out] image 存放图像指针的指针
/// @param[in]  image_width 图像的宽度
/// @param[in]  image_height 图像的高度
/// @param[in]  pixel_format 像素格式
SN_API sn_result_t sn_image_allocate(
    sn_image_t **image,
    int image_width,
    int image_height,
    sn_pixel_format pixel_format
);

/// @brief 释放图像内存
/// @param[in] image 存放图像的指针
SN_API void sn_image_release(sn_image_t *image);

#endif //SN_COMMON_COMMON_H
