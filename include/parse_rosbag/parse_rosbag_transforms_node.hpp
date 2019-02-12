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
#include <tuw_nav_msgs/JointsIWS.h>

struct stat sb; // for system checks i.e. does directory exist

bool DEBUG = false;

const std::string rosbag_dir = "/home/marc/Workspace/Rosbag";
const std::string measurement_folder = "rccar_15_11_18";
const std::string rosbag_filename = "marc_run_5_full";
const std::string gt_file = "groundtruth";
const std::string gt_opti_file = "groundtruth_opti";
const std::string encoder_file = "encoder";
const std::string rosbag_ext = "bag";
const std::string img_ext = "png";
const std::string text_ext = "txt";

std::stringstream rosbag_file_path;
std::stringstream rosbag_file_dir;
std::stringstream rosbag_groundtruth_dir;
std::stringstream rosbag_groundtruth_opti_dir;
std::stringstream rosbag_encoder_dir;
std::stringstream rosbag_groundtruth_file_path;
std::stringstream rosbag_groundtruth_opti_file_path;
std::stringstream rosbag_encoder_file_path;

std::ofstream groundtruth_filestream;
std::ofstream groundtruth_opti_filestream;
std::ofstream encoder_filestream;

// Image topics to load
std::vector<std::string> topics;

//const std::string rccar_pose_topic = "/optitrack/rccar/pose";
const std::string rccar_pose_topic = "/master/true_pose";
const std::string rccar_pose_topic_optitrack = "/vrpn_client_node/rccar/pose";
const std::string encoder_topic = "/r1/joint_cmds";

const int nano_sec_digits = 9;



#endif //PROJECT_PARSE_ROSBAG_TRANSFORMS_NODE_HPP
