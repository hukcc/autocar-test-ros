/**
 * Robomaster Vision program of Autocar
 * Copyright (c) 2019, Xidian University Robotics Vision group.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
 * documentation files(the "Software"), to deal in the Software without restriction.
 */

#ifndef _ARMOR_DETECT_NODE_H_
#define _ARMOR_DETECT_NODE_H_

#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <geometry_msgs/Point32.h>

#include <armor_info.h>
#include <armor_detect.h>

#include <ros/ros.h>
#include <serial/car_info.h>
#include <detect/armor_goal.h>

namespace detect_mul
{
class armor_detect_node
{
public:
    armor_detect_node()     //构造函数
    {
        ros::NodeHandle n;          //消息句柄  我记得好象是这样
        image_transport::ImageTransport it(n);      //ros的图片发布流       这边的句柄和这个图片流的操作都是固定的
        sub_img_   = it.subscribe("camera_info", 5,    //这边5不知道是啥意思                                 //这里subscribe是订阅消息   订阅的是camera_info节点发布的消息
                        boost::bind(&armor_detect_node::armor_callback, this, _1));     //回调函数      

        sub_yaw_   = n.subscribe<serial::car_info>("car_info",5,                        //订阅了两个节点的消息  车和摄像头回传的消息
                        boost::bind(&armor_detect_node::car_callback, this, _1));
                                                                                        //上面的两个订阅 触发回调函数应该也是按照订阅消息的顺序
        pub_armor_ = n.advertise<detect::armor_goal>("armor_info", 5);                  //发布关于装甲的消息
    }

    void armor_callback(const sensor_msgs::ImageConstPtr& msg);     //由摄像头节点回传的消息触发该回调函数 这边基本上就是原来旧版的装甲识别 还有很多没有搬过来的
    void car_callback(const serial::car_info::ConstPtr &gimbal_info);      //由车回传的云台消息触发该回传函数 

private:
    image_transport::Subscriber sub_img_;
    ros::Subscriber sub_yaw_;
    ros::Publisher  pub_armor_;

    detect::armor_goal armor_info;
    int detected;
    
    //ArmorDetector armor_detect_;
    //std::vector<armor_info> multi_armors_;
};

} // namespace detect_mul

#endif
