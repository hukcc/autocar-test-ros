/**
 * Robomaster Vision program of Autocar
 * Copyright (c) 2019, Xidian University Robotics Vision group.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
 * documentation files(the "Software"), to deal in the Software without restriction.
 */

#include <armor_detect_node.h>

namespace detect_mul
{

void armor_detect_node::car_callback(const serial::car_info::ConstPtr &info)        //这个回调函数基本上只是用来从car_info节点获取云台的信息
{                                                                                   //这边应该是用来在回传数据的时候做一个处理 另外用来做预测
    double gimbal_yaw   = info->yaw;                                                //问题在于 这边从车接收到的角度数据到底要怎么用
    double gimbal_pitch = info->pitch;
}

void armor_detect_node::armor_callback(const sensor_msgs::ImageConstPtr& msg) //这边注释掉的部分基本上就是以前那一版代码 看完框架之后移植过来      //处理摄像头节点的消息 这里传过来的是一帧一帧的图像
{
    try{        //做一个异常捕获

        cv::Mat frame = cv_bridge::toCvShare(msg, "bgr8")->image;   //从消息中将图像读取出来
        
        cv::imshow("img", frame);   
        cv::waitKey(1);
        
        //multi_armors_.clear();
        //armor_detect_.detect(frame, multi_armors_);

        //if(multi_armors_.size())
        //{
            // pnp
            // detect::armor_goal armor_pos = what?
            // pub_armor_.publish(armor_pos);
        //}

        // armor detect

        std::vector<geometry_msgs::Point32> multi_goal;  //这边的容器装的是每一个装甲板对应的云台姿态   //ros自带的一种数据结构 Point32 含有x、y、z三个成员函数 这边是用来记录yaw pitch 和 距离信息 取代原来的armor结构
        geometry_msgs::Point32 v1,v2;
        multi_goal.push_back(v1);
        multi_goal.push_back(v2);       //保证一开始至少是个有两个元素的动态数组

        // pnp solver

        
        // DO NOT EDIT!                                  //消息的成员可以看msg文件夹 的.msg文件   这边这个类应该是基于.msg文件 在编译的时候自己生成的 所以他的成员只需要看msg文件就ok
        armor_info.stamp     = ros::Time::now();        //对发布的消息操作 盖时间戳  
        armor_info.detected  = detected;                //标志位  是否检测到装甲
        armor_info.multigoal = multi_goal;              //将装甲对应的动态数组放到消息里面
        pub_armor_.publish(armor_info);                 //发布消息
    }
    catch (cv_bridge::Exception& e)         //如果抛出了这个不知道啥异常
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());     //就在日志中记录一下无法从消息中读取到图片
    }
}

} // namespace detect_mul

int main(int argc, char **argv)         //装甲识别节点的路口
{
    ros::init(argc, argv, "armor_detect");      //ros初始化

    detect_mul::armor_detect_node node;     //声明节点对象（应该和官方一样是在构造函数里面操作）

    ros::MultiThreadedSpinner spinner(2);
    ros::spin();

    return 0;
}