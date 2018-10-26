#include "parse_rosbag/parse_rosbag_transforms_node.hpp"

using namespace std;


int main(int argc, char** argv)
{
  ros::init(argc, argv, "parse_rosbag_transforms");

  rosbag_file_dir << rosbag_dir << "/" << measurement_folder << "/" << rosbag_filename;

  string rosbag_file_dir_str = rosbag_file_dir.str();
  const char * rosbag_file_dir_c_str = rosbag_file_dir_str.c_str();

  rosbag_file_path << rosbag_file_dir_str << '.' << rosbag_ext;
  string rosbag_file_path_str = rosbag_file_path.str();

  rosbag_groundtruth_dir << rosbag_file_dir_str;
  auto rosbag_groundtruth_dir_str = rosbag_groundtruth_dir.str();

  rosbag_groundtruth_file_path << rosbag_groundtruth_dir_str << "/" << gt_file << "." << text_ext;
  auto rosbag_groundtruth_file_path_str = rosbag_groundtruth_file_path.str();

  rosbag_encoder_dir << rosbag_file_dir_str;
  auto rosbag_encoder_dir_str = rosbag_encoder_dir.str();

  rosbag_encoder_file_path << rosbag_encoder_dir_str << "/" << encoder_file << "." << text_ext;
  auto rosbag_encoder_file_path_str = rosbag_encoder_file_path.str();


  // check if directory exists already
  if (!(stat(rosbag_file_dir_c_str, &sb) == 0 && S_ISDIR(sb.st_mode))){
    const int dir_err = mkdir(rosbag_file_dir_c_str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err){
      printf("Error creating directory!n");
      return 1;
    }
  }


  groundtruth_filestream.open(rosbag_groundtruth_file_path_str);
  encoder_filestream.open(rosbag_encoder_file_path_str);



  //ImageSaver * imageSaver = new ImageSaver();
  rosbag::Bag bag;
  bag.open(rosbag_file_path_str, rosbag::bagmode::Read);

  topics.push_back(rccar_pose_topic);
  topics.push_back(encoder_topic);

  rosbag::View view(bag, rosbag::TopicQuery(topics));


  // Load all messages into our stereo dataset
  for(rosbag::MessageInstance const m: view)
  {
    string topic = m.getTopic();

    if (topic == rccar_pose_topic || ("/" + topic == rccar_pose_topic))
    {
      geometry_msgs::PoseStampedConstPtr posePtr = m.instantiate<geometry_msgs::PoseStamped>();
      if(posePtr == NULL)
        continue;
      auto ts = posePtr->header.stamp;
      auto pose = posePtr->pose;
      auto position = pose.position;
      auto quat = pose.orientation;
      stringstream timeSS;
      stringstream positionSS;
      stringstream quaternionSS;
      stringstream measurementSS;
      timeSS << ts.sec << "." << ts.nsec;
      positionSS << position.x << " " << position.y << " " << position.z;
      quaternionSS << quat.x << " " << quat.y << " " << quat.z << " " << quat.w;
      auto timeString = timeSS.str();
      auto positionString = positionSS.str();
      auto quaternionString = quaternionSS.str();

      measurementSS << timeString << " " << positionString << " " << quaternionString;
      auto measurementString = measurementSS.str();

      if(DEBUG){
        cout << measurementString << "\n";
      }

      groundtruth_filestream << measurementString << "\n";

    }

    if (topic == encoder_topic || ("/" + topic == encoder_topic))
    {
      tuw_nav_msgs::JointsIWS::ConstPtr iwsPtr = m.instantiate<tuw_nav_msgs::JointsIWS>();
      if(iwsPtr == NULL)
        continue;
      auto ts = iwsPtr->header.stamp;
      auto rev = iwsPtr->revolute[1];
      auto steering = iwsPtr->steering[0];

      stringstream timeSS;
      timeSS << ts.sec << "." << ts.nsec;
      auto timeString = timeSS.str();

      stringstream encoderSS;
      encoderSS << timeString << " " << rev << " " << " " << steering;
      auto encoderString = encoderSS.str();

      if(DEBUG){
        cout << encoderString << "\n";
      }

      encoder_filestream << encoderString << "\n";

    }

  }


  encoder_filestream.close();
  groundtruth_filestream.close();
  bag.close();
  return 0;
}