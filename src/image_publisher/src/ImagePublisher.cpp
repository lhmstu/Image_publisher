#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "image_publisher/ParameterReader.h"


int main(int argc, char** argv)
{
  //init ros package , init ros node , init parameterReader 
  cout << "Initializing" << endl;
  ros::init(argc, argv, "ImagePublisher");
  ros::NodeHandle nh;
  ParameterReader pd;

  // set the multi picture number -40 ~ 40
  int startIndex = atoi(pd.getData( "start_index" ).c_str() );
  int endIndex = atoi(pd.getData( "end_index" ).c_str() );
  cout << "setting image data " << endl;
  cout << "startIndex = " << startIndex << endl;
  cout << "endIndex = " << endIndex << endl;

  int currIndex = startIndex;
  // init image_transport object : rgb_transport
  image_transport::ImageTransport rgb_transport(nh);
  image_transport::Publisher pub_rgb = rgb_transport.advertise("camera/rgb/image", 10); // topic : camera/rgb/image
  cv::Mat image_rgb = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
  cv::waitKey(30);
  sensor_msgs::ImagePtr msg_rgb = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image_rgb).toImageMsg();

  ros::Rate loop_rate(5);
  while (nh.ok()) {
    pub_rgb.publish(msg_rgb);
    ros::spinOnce();
    loop_rate.sleep();
  }
}