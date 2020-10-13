#include <ros/ros.h>

#include <cstdio>
#include <cstdlib>
#include <ar_track_alvar_msgs/AlvarMarkers.h>

ros::Publisher ars_pub;

void ar_tags_callback(const ar_track_alvar_msgs::AlvarMarkers::ConstPtr &msg) {
	// ar_pose::ARMarker ar_marker;
	if (!msg->markers.empty()) {
		if(msg->markers[0].id >= 0 and msg->markers[0].id <= 11) {
			ROS_INFO("Valid AR Tag");
		} else {			
			ROS_WARN_ONCE("Valid AR Tag");
		}

		// ar_marker = msg->markers[0];
		// geometry_msgs::Twist motor_command;  
		//ROS_INFO("X : [%f] ", msg->markers[0].pose.pose.position.x);
		//ROS_INFO("Y : [%f] ", msg->markers[0].pose.pose.position.y);
		//ROS_INFO("Z : [%f] ", msg->markers[0].pose.pose.position.z); 
	}
}

int main(int argc, char** argv) {
	ros::init(argc, argv, "ar_tags");
	ros::NodeHandle n;

    ros::Subscriber ar_sub = n.subscribe("ar_pose_marker", 1, ar_tags_callback);
    ars_pub = n.advertise<ar_track_alvar_msgs::AlvarMarkers>("ar_pose_marker_valid", 1);
    
    ROS_INFO("AR Tags Detection Spinning");
    ros::spin();
    
    return 0;
}	
