#include <ros/ros.h>
#include <ros/console.h>
#include <nav_msgs/Odometry.h>

#include <string>
#include <iostream>

#define NODE    "Intial Pose Node ::"
ros::Publisher pose_pub;

bool has_init_odom = false;
nav_msgs::Odometry init_odom;

void odom_cb(const nav_msgs::Odometry::ConstPtr msg) {
	init_odom.header = msg->header;			// Need to Sync with curr timestamps
	if(!has_init_odom) {
		ROS_INFO("%s Initial Pose Set", NODE);
		// init_odom = msg;
		// init_odom.header = msg->header;		
		init_odom.child_frame_id = msg->child_frame_id;
		init_odom.pose = msg->pose;
		init_odom.twist = msg->twist;
		has_init_odom = true;
	}
    pose_pub.publish(init_odom);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "init_odom");
	ROS_INFO("%s Spinning", NODE);
	ros::NodeHandle n;

    ros::Subscriber odem_sub = n.subscribe("odom", 1, odom_cb);
    pose_pub = n.advertise<nav_msgs::Odometry>("initialodom", 10);
    
    ros::spin();
    return 0;
}