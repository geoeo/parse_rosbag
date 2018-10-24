//
// Created by marc on 24.10.18.
//

#ifndef PROJECT_PARSE_ROSBAG_NODE_HPP
#define PROJECT_PARSE_ROSBAG_NODE_HPP

#include <sys/stat.h>

#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>

#include <boost/foreach.hpp>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>

#include <opencv2/opencv.hpp>

struct stat sb; // for system checks i.e. does directory exist

const std::string rosbag_dir = "/home/marc/Workspace/Rosbag";
const std::string measurement_folder = "rccar_26_09_18";
const std::string rosbag_filename = "marc_1";
const std::string rosbag_ext = "bag";

std::stringstream rosbag_file_path;
std::stringstream rosbag_file_dir;



#endif //PROJECT_PARSE_ROSBAG_NODE_HPP
