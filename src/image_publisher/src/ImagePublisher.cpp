/*
 * @file_name: ImagePublisher.cpp 
 * @Author: che-yu,yang 
 * @Date: 2017-09-05 10:50:43 
 * @Last Modified by:   che-yu,yang 
 * @Last Modified time: 2017-09-05 10:50:43 
 */
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "image_publisher/ParameterReader.h"

//give index , to load frame data
FRAME readFrame( int index, ParameterReader& pd );

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

  int currIndex = startIndex; // current index is currIndex
  //FRAME lastFrame = readFrame( currIndex, pd); //compute currFrame to lastFrame
  // init image_transport object : rgb_transport
  image_transport::ImageTransport rgb_transport(nh);
  image_transport::Publisher pub_rgb = rgb_transport.advertise("camera/rgb/image", 1); // topic : camera/rgb/image

  image_transport::ImageTransport depth_transport(nh);
  image_transport::Publisher pub_depth = depth_transport.advertise("camera/depth/image", 1); // topic : camera/depth/image
    for( currIndex = startIndex; currIndex <= endIndex; currIndex++)
    {
      cout << "Reading files : " << currIndex << endl;
      FRAME currFrame = readFrame( currIndex, pd ); // load currFrame (rgb.png & depth.png)
      cv::Mat image_rgb = cv::imread(currFrame.rgb, CV_LOAD_IMAGE_COLOR);
      cv::Mat image_depth = cv::imread(currFrame.depth, CV_LOAD_IMAGE_COLOR);
      cv::waitKey(100);
      sensor_msgs::ImagePtr msg_rgb = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image_rgb).toImageMsg();
      sensor_msgs::ImagePtr msg_depth = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image_depth).toImageMsg();
      ros::Rate loop_rate(5);
      if(nh.ok())
      {
        pub_rgb.publish(msg_rgb);
        pub_depth.publish(msg_depth);
        ros::spinOnce();
        loop_rate.sleep();
      }
  }
}

//give index , to load frame data (rgb.png & depth.png)
FRAME readFrame(int index, ParameterReader& pd)
{
	FRAME f ;
	string rgbDir = pd.getData("rgb_dir");
	string depthDir = pd.getData("depth_dir");

	string rgbExt = pd.getData("rgb_extension");
	string depthExt = pd.getData("depth_extension");

	stringstream ss;
	ss<<rgbDir<<index<<rgbExt;
	string filename;
	ss>>filename;
	f.rgb = filename;

	ss.clear();
	filename.clear();
	ss<<depthDir<<"DeepImage_"<<index<<depthExt;
	ss>>filename;
	f.depth = filename;
	
	return f ; 
}