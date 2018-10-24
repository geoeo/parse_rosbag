#include "parse_rosbag/parse_rosbag_node.hpp"


using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "parse_rosbag");

  rosbag_file_dir << rosbag_dir << "/" << measurement_folder << "/" << rosbag_filename;

  string rosbag_file_dir_str = rosbag_file_dir.str();
  const char * rosbag_file_dir_c_str = rosbag_file_dir_str.c_str();

  rosbag_file_path << rosbag_file_dir_str << '.' << rosbag_ext;

  string rosbag_file_path_str = rosbag_file_path.str();



  // check if directory exists already
  if (!(stat(rosbag_file_dir_c_str, &sb) == 0 && S_ISDIR(sb.st_mode))){
    const int dir_err = mkdir(rosbag_file_dir_c_str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err){
      printf("Error creating directory!n");
      return 1;
    }
  }


  rosbag::Bag bag;
  bag.open(rosbag_file_path_str, rosbag::bagmode::Read);


  bag.close();
  return 0;
}