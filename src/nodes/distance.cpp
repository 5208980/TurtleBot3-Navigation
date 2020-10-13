#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/Odometry.h>
#include <ar_track_alvar_msgs/AlvarMarkers.h>
#include <message_filters/subscriber.h>
#include <geometry_msgs/PointStamped.h>
// #include <message_filters/time_synchronizer.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

#include <string>
#include <iostream>

ros::Subscriber scanSub;

// void distance_cb(const nav_msgs::Odometry::ConstPtr msg) {
void distance_cb(const nav_msgs::Odometry::ConstPtr& msg, const ar_track_alvar_msgs::AlvarMarkers::ConstPtr& tag) {
	// ROS_INFO("%f", (double)scan->ranges[360]);
	ROS_INFO("SYNC");
	if (!tag->markers.empty()) {
		tf::TransformListener listener;
		tf::StampedTransform transform;

		try{
			std::string marker_id = "ar_marker_" + std::to_string(tag->markers[0].id);
			listener.lookupTransform("camera_rgb_optical_frame", marker_id, ros::Time(0), transform);
		}
		catch (tf::TransformException ex){
			ROS_ERROR("Transform couldn't be made");
		}

	/*
		double x = transform.getOrigin().x();
		double y = transform.getOrigin().y();
		double dist = sqrt(x*x + y*y);
		ROS_INFO("%f", dist);	
	*/
		
		tf::Quaternion q(
		    msg->pose.pose.orientation.x,
		    msg->pose.pose.orientation.y,
		    msg->pose.pose.orientation.z,
		    msg->pose.pose.orientation.w);
		tf::Matrix3x3 m(q);

		double roll, pitch, yaw;
		m.getRPY(roll, pitch, yaw);
	#include <ar_track_alvar_msgs/AlvarMarkers.h>
		tf::Transform tf_bot_ar_tag = transform.inverse();
		tf::Vector3 robot(msg->pose.pose.position.x, msg->pose.pose.position.y, yaw);
		tf::Vector3 pt = tf_bot_ar_tag * robot;

		tf::Quaternion q1(
		    tag->markers[0].pose.pose.orientation.x,
		    tag->markers[0].pose.pose.orientation.y,
		    tag->markers[0].pose.pose.orientation.z,
		    tag->markers[0].pose.pose.orientation.w);
		tf::Matrix3x3 m1(q1);
		double roll1, pitch1, yaw1;
		m.getRPY(roll1, pitch1, yaw1);
		tf::Vector3 door1_pt(tag->markers[0].pose.pose.position.x, tag->markers[0].pose.pose.position.y, yaw1);

		float distance = pt.distance(door1_pt);
		ROS_INFO("%f", distance);
	}
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "tf_ar_robot");
	ros::NodeHandle n;

    // scanSub = n.subscribe<sensor_msgs::LaserScan>("scan", 1, distance_cb);

    // scanSub = n.subscribe<nav_msgs::Odometry>("odom", 1, distance_cb);

	message_filters::Subscriber<nav_msgs::Odometry> odom_sub(n, "odom", 1);
	message_filters::Subscriber<ar_track_alvar_msgs::AlvarMarkers> ar_tag(n, "ar_pose_marker_stamped", 1);
	marker_pub = n.advertise<geometry_msgs/PointStamped>("ar_pose_marker_stamped/distance", 1);
	typedef message_filters::sync_policies::ApproximateTime<nav_msgs::Odometry, ar_track_alvar_msgs::AlvarMarkers> MySyncPolicy;	
	message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), odom_sub, ar_tag);
	ROS_INFO("HERE");
	sync.registerCallback(boost::bind(&distance_cb, _1, _2));

    ros::spin();
    return 0;
}