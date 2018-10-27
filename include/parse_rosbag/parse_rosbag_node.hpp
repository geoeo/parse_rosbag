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
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/simple_filter.h>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>

#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>

struct stat sb; // for system checks i.e. does directory exist

bool DEBUG = false;

const std::string rosbag_dir = "/home/marc/Workspace/Rosbag";
const std::string measurement_folder = "rccar_26_09_18";
const std::string rosbag_filename = "marc_3_full";
const std::string color_dir = "color";
const std::string color_rect_dir = "color_rect";
const std::string depth_dir = "depth";
const std::string depth_rect_dir = "depth_rect";
const std::string rosbag_ext = "bag";
const std::string img_ext = "png";

std::stringstream rosbag_file_path;
std::stringstream rosbag_file_dir;
std::stringstream rosbag_depth_dir;
std::stringstream rosbag_depth_rect_dir;
std::stringstream rosbag_color_dir;
std::stringstream rosbag_color_rect_dir;

// Image topics to load
std::vector<std::string> topics;

const std::string color_image_topic = "/camera/rgb/image_raw/compressed";
const std::string color_image_rectified_topic = "/camera/rgb/image_rect_color/compressed";

const std::string depth_image_topic = "/camera/depth/image_raw";
const std::string depth_image_rectified_topic = "/camera/depth/image_rect";

const int nano_sec_digits = 9;

// http://wiki.ros.org/message_filters#Example_.28C.2B-.2B-.29-1
typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> MySyncPolicy;

/**
 * Inherits from message_filters::SimpleFilter<M>
 * to use protected signalMessage function
 */
template <class M>
class BagSubscriber : public message_filters::SimpleFilter<M>
{
public:
  void newMessage(const boost::shared_ptr<M const> &msg)
  {
    this->signalMessage(msg);
  }
};

// Set up fake subscribers to capture images
BagSubscriber<sensor_msgs::Image> img_color_sub, im_color_rect_sub, im_depth_sub, im_depth_rect_sub;

class ImageSaver
{
public:
  void callback(const sensor_msgs::Image::ConstPtr &c_img,
                const sensor_msgs::Image::ConstPtr &d_img);


};


#endif //PROJECT_PARSE_ROSBAG_NODE_HPP
