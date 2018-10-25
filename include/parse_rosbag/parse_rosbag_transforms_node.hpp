//
// Created by marc on 25.10.18.
//

#ifndef PROJECT_PARSE_ROSBAG_TRANSFORMS_NODE_HPP
#define PROJECT_PARSE_ROSBAG_TRANSFORMS_NODE_HPP

#include <sys/stat.h>
#include <iostream>
#include <fstream>

#include <ros/ros.h>
#include <ros/console.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>

#include <boost/foreach.hpp>

#include <geometry_msgs/PoseStamped.h>

struct stat sb; // for system checks i.e. does directory exist

bool DEBUG = true;

const std::string rosbag_dir = "/home/marc/Workspace/Rosbag";
const std::string measurement_folder = "rccar_26_09_18";
const std::string rosbag_filename = "marc_1_full";
const std::string gt_dir = "groundtruth";
const std::string rosbag_ext = "bag";
const std::string img_ext = "png";
const std::string text_ext = "txt";

std::stringstream rosbag_file_path;
std::stringstream rosbag_file_dir;
std::stringstream rosbag_groundtruth_dir;
std::stringstream rosbag_groundtruth_file_path;

std::ofstream groundtruth_filestream;

// Image topics to load
std::vector<std::string> topics;

const std::string rccar_pose_topic = "/optitrack/rccar/pose";





#endif //PROJECT_PARSE_ROSBAG_TRANSFORMS_NODE_HPP