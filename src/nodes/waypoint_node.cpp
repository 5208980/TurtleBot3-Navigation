#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <ar_track_alvar_msgs/AlvarMarker.h>
#include <tf/tf.h>  
/*
header: 
  seq: 0
  stamp: 
    secs: 1444430928
    nsecs: 28760322
  frame_id: /head_camera
id: 3		// IMPORTANT: THIS IS THE MARKER_ID that will be uniquely used to identity ar_marker
confidence: 0
pose: 
  header: 
    seq: 0
    stamp: 
      secs: 0
      nsecs: 0
    frame_id: ''
  pose: 
    position: 
      x: 0.196624979223
      y: -0.238047436646
      z: 1.16247606451
    orientation: 
      x: 0.970435431848
      y: 0.00196992162831
      z: -0.126455066154
      w: -0.205573121457


std_msgs/Header header
uint32 id
uint32 confidence
geometry_msgs/PoseStamped pose
*/

void pose_callback(ar_track_alvar_msgs::AlvarMarkers req) {   
    if (!req.markers.empty()) {
      tf::Quaternion q(req.markers[0].pose.pose.orientation.x, req.markers[0].pose.pose.orientation.y, req.markers[0].pose.pose.orientation.z, req.markers[0].pose.pose.orientation.w);
      tf::Matrix3x3 m(q);
      double roll, pitch, yaw;
      m.getRPY(roll, pitch, yaw);
      ROS_INFO("roll, pitch, yaw=%1.2f  %1.2f  %1.2f", roll, pitch, yaw);
      // roll  --> rotate around vertical axis
      // pitch --> rotate around horizontal axis
      // yaw   --> rotate around depth axis
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "pose_subscriber");
    ros::NodeHandle nh;
    ros::Subscriber pose_sub = nh.subscribe("ar_pose_marker", 1000, pose_callback);
    ros::spin();
    return 0;

}