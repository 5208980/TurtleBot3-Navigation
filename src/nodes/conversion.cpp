#include <ros/ros.h>
#include <ros/console.h>
#include <tf/tf.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/Float64.h>
#include <string>
#include <iostream>

ros::Publisher pose_pub;
ros::Subscriber amcl_sub;

bool hasInitialPose = false;

void odem_callback(const nav_msgs::Odometry::ConstPtr msg) {
    geometry_msgs::Pose2D pose2d;

    pose2d.x = msg->pose.pose.position.x;
    pose2d.y = msg->pose.pose.position.y;
    
    tf::Quaternion q(
        msg->pose.pose.orientation.x,
        msg->pose.pose.orientation.y,
        msg->pose.pose.orientation.z,
        msg->pose.pose.orientation.w);
    tf::Matrix3x3 m(q);

    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    
    pose2d.theta = yaw;
    pose_pub.publish(pose2d);

	ros::NodeHandle nh;

	if(!hasInitialPose) {
		nh.setParam("/amcl/initial_pose_x", pose2d.x);
		nh.setParam("/amcl/initial_pose_y", pose2d.y);
		nh.setParam("/amcl/initial_pose_a", pose2d.theta);
		hasInitialPose = true;
	}
/*

	std::string str_x = std::to_string(pose2d.x);
	std::string str_y = std::to_string(pose2d.y);
	std::string str_a = std::to_string(pose2d.theta);
	std::string pose_x, pose_y, pose_a;
	nh.getParam("/amcl/initial_pose_x", pose_x);
	std::cout << (pose_x != str_x);
	ROS_INFO("pose %f", pose2d.x);
	//if(pose_x == str_x) {
		ROS_INFO("stamp %f", pose2d.x);
		nh.setParam("/amcl/initial_pose_x", pose2d.x);
	//}
	nh.getParam("/amcl/initial_pose_y", pose_y);
	//if(pose_y == str_y) {
		ROS_INFO("stamp %f", pose2d.y);
		nh.setParam("/amcl/initial_pose_y", pose2d.y);
	//}
	nh.getParam("/amcl/initial_pose_a", pose_a);
	//if(pose_a == str_a) {
		ROS_INFO("stamp %f", pose2d.theta);
		nh.setParam("/amcl/initial_pose_a", pose2d.theta);
	//}
*/
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "conversion_node");
	ros::NodeHandle n;

    ros::Subscriber odem_sub = n.subscribe("odom", 1, odem_callback);
    pose_pub = n.advertise<geometry_msgs::Pose2D>("pose2d", 1);
    
    ros::spin();
    return 0;
}