#include "parse_rosbag/parse_rosbag_node.hpp"

using namespace std;
using namespace cv;


// Callback for synchronized messages
void callback(const sensor_msgs::Image::ConstPtr &c_img,
               const sensor_msgs::Image::ConstPtr &d_img)
 {

   cv_bridge::CvImageConstPtr cv_depth_ptr = cv_bridge::toCvShare(d_img,sensor_msgs::image_encodings::MONO16);

 }


int main(int argc, char** argv)
{
  ros::init(argc, argv, "parse_rosbag");

  rosbag_file_dir << rosbag_dir << "/" << measurement_folder << "/" << rosbag_filename;

  string rosbag_file_dir_str = rosbag_file_dir.str();
  const char * rosbag_file_dir_c_str = rosbag_file_dir_str.c_str();

  rosbag_file_path << rosbag_file_dir_str << '.' << rosbag_ext;

  string rosbag_file_path_str = rosbag_file_path.str();

  rosbag_depth_dir << rosbag_file_dir_str << "/" << depth_dir;
  auto rosbag_depth_dir_str = rosbag_depth_dir.str();
  auto rosbag_depth_dir_c_str = rosbag_depth_dir_str.c_str();

  rosbag_depth_rect_dir << rosbag_file_dir_str << "/" << depth_rect_dir;
  auto rosbag_depth_rect_dir_str = rosbag_depth_rect_dir.str();
  auto rosbag_depth_rect_dir_c_str = rosbag_depth_rect_dir_str.c_str();

  rosbag_color_dir << rosbag_file_dir_str << "/" << color_dir;
  auto rosbag_color_dir_str = rosbag_color_dir.str();
  auto rosbag_color_dir_c_str = rosbag_color_dir_str.c_str();

  rosbag_color_rect_dir << rosbag_file_dir_str << "/" << color_rect_dir;
  auto rosbag_color_rect_dir_str = rosbag_color_rect_dir.str();
  auto rosbag_color_rect_dir_c_str = rosbag_color_rect_dir_str.c_str();



  // check if directory exists already
  if (!(stat(rosbag_file_dir_c_str, &sb) == 0 && S_ISDIR(sb.st_mode))){
    const int dir_err = mkdir(rosbag_file_dir_c_str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err){
      printf("Error creating directory!n");
      return 1;
    }
  }

  if (!(stat(rosbag_depth_dir_c_str, &sb) == 0 && S_ISDIR(sb.st_mode))){
    const int dir_err = mkdir(rosbag_depth_dir_c_str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err){
      printf("Error creating directory!n");
      return 1;
    }
  }


  if (!(stat(rosbag_depth_rect_dir_c_str, &sb) == 0 && S_ISDIR(sb.st_mode))){
    const int dir_err = mkdir(rosbag_depth_rect_dir_c_str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err){
      printf("Error creating directory!n");
      return 1;
    }
  }

  if (!(stat(rosbag_color_dir_c_str, &sb) == 0 && S_ISDIR(sb.st_mode))){
    const int dir_err = mkdir(rosbag_color_dir_c_str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err){
      printf("Error creating directory!n");
      return 1;
    }
  }

  if (!(stat(rosbag_color_rect_dir_c_str, &sb) == 0 && S_ISDIR(sb.st_mode))){
    const int dir_err = mkdir(rosbag_color_rect_dir_c_str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err){
      printf("Error creating directory!n");
      return 1;
    }
  }


  //ImageSaver * imageSaver = new ImageSaver();
  rosbag::Bag bag;
  bag.open(rosbag_file_path_str, rosbag::bagmode::Read);

  topics.push_back(color_image_topic);
  topics.push_back(color_image_rectified_topic);
  topics.push_back(depth_image_topic);
  topics.push_back(depth_image_rectified_topic);

  rosbag::View view(bag, rosbag::TopicQuery(topics));

  // Use time synchronizer to make sure we get properly synchronized images
  message_filters::Synchronizer<MySyncPolicy> sync_raw(MySyncPolicy(10000000),img_color_sub, im_depth_sub);
  message_filters::Synchronizer<MySyncPolicy> sync_rect(MySyncPolicy(10000000), im_color_rect_sub, im_depth_rect_sub);
  sync_raw.registerCallback(boost::bind(&callback,_1, _2));
  sync_rect.registerCallback(boost::bind(&callback,_1, _2));

  vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(9);


  // Load all messages into our stereo dataset
  for(rosbag::MessageInstance const m: view)
  {
    string topic = m.getTopic();

    if (topic == color_image_topic || ("/" + topic == color_image_topic))
    {
      sensor_msgs::CompressedImageConstPtr img = m.instantiate<sensor_msgs::CompressedImage>();
      if(img == NULL)
        continue;
      auto ts = img->header.stamp;

      auto img_ptr = cv_bridge::toCvCopy(img,sensor_msgs::image_encodings::BGR8);
      cv::Mat view = img_ptr->image;
      if(!view.empty()) {
        if(DEBUG){
          const cv::Mat &image_ref = view;
          cv::imshow("color", image_ref);
          cv::waitKey(1); // super important otherwise image wont be displayed
        }
        stringstream image_name;
        image_name << rosbag_color_dir_str << "/" << ts.sec << "." << ts.nsec << "." << img_ext;
        auto image_name_str = image_name.str();

        cv::imwrite(image_name_str,view,compression_params);
      }

    }

    if (topic == color_image_rectified_topic || ("/" + topic == color_image_rectified_topic))
    {
      sensor_msgs::CompressedImageConstPtr img = m.instantiate<sensor_msgs::CompressedImage>();
      if(img == NULL)
        continue;
      auto ts = img->header.stamp;
      auto img_ptr = cv_bridge::toCvCopy(img,sensor_msgs::image_encodings::BGR8);
      cv::Mat view = img_ptr->image;
      if(!view.empty()) {
        if(DEBUG){
          const cv::Mat &image_ref = view;
          cv::imshow("color rect", image_ref);
          cv::waitKey(1); // super important otherwise image wont be displayed
        }
        stringstream image_name;
        image_name << rosbag_color_rect_dir_str << "/" << ts.sec << "." << ts.nsec << "." << img_ext;
        auto image_name_str = image_name.str();

        cv::imwrite(image_name_str,view,compression_params);

      }
    }

    if (topic == depth_image_topic || ("/" + topic == depth_image_topic))
    {
      sensor_msgs::ImageConstPtr img = m.instantiate<sensor_msgs::Image>();
      if(img == NULL)
        continue;
      auto ts = img->header.stamp;
      auto img_ptr = cv_bridge::toCvCopy(img,sensor_msgs::image_encodings::TYPE_16UC1);
      cv::Mat view = img_ptr->image;
      if(!view.empty()){
        if(DEBUG){
          cv::Mat view_norm;
          cv::normalize( view , view_norm,0,255, NORM_MINMAX,CV_8UC1);
          cv::imshow("depth", view_norm);
          cv::waitKey(1); // super important otherwise image wont be displayed
        }

        stringstream image_name;
        image_name << rosbag_depth_dir_str << "/" << ts.sec << "." << ts.nsec << "." << img_ext;
        auto image_name_str = image_name.str();

        cv::imwrite(image_name_str,view,compression_params);

      }
    }


    if (topic == depth_image_rectified_topic || ("/" + topic == depth_image_rectified_topic))
    {
      sensor_msgs::ImageConstPtr img = m.instantiate<sensor_msgs::Image>();
      if(img == NULL)
        continue;
      auto ts = img->header.stamp;
      auto img_ptr = cv_bridge::toCvCopy(img,sensor_msgs::image_encodings::TYPE_16UC1);
      cv::Mat view = img_ptr->image;
      if(!view.empty()){
        if(DEBUG){
          cv::Mat view_norm;
          cv::normalize( view , view_norm,0,255, NORM_MINMAX,CV_8UC1);
          cv::imshow("depth rect", view_norm);
          cv::waitKey(1); // super important otherwise image wont be displayed
        }

        stringstream image_name;
        image_name << rosbag_depth_rect_dir_str << "/" << ts.sec << "." << ts.nsec << "." << img_ext;
        auto image_name_str = image_name.str();

        cv::imwrite(image_name_str,view,compression_params);

      }

    }
  }



  bag.close();
  return 0;
}