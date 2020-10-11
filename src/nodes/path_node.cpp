#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>

ros::Publisher path_pub;
nav_msgs::Path path;

void path_trace_callback(const nav_msgs::Odometry::ConstPtr &odom) {
	geometry_msgs::PoseStamped pose;

	pose.header.stamp = odom->header.stamp;
	pose.header.frame_id = odom->header.frame_id;
	pose.pose.position = odom->pose.pose.position;
	// pose.pose.position.x = ...;

	path.header.stamp = odom->header.stamp;
	path.header.frame_id = odom->header.frame_id;
	path.poses.push_back(pose);

	path_pub.publish(path);
}

int main(int argc, char** argv) {
	ros::init(argc, argv, "path_node");
	ros::NodeHandle n;

    ros::Subscriber odom_sub = n.subscribe("odom", 1, path_trace_callback);
    path_pub = n.advertise<nav_msgs::Path>("/path", 1);
    
    ros::spin();
    return 0;
}	
