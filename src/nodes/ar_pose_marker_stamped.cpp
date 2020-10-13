#include <ros/ros.h>
#include <ar_track_alvar_msgs/AlvarMarkers.h>

#include <string>
#include <iostream>

ros::Publisher marker_pub;
ros::Subscriber tag_sub;

void cb(const ar_track_alvar_msgs::AlvarMarkers::ConstPtr& tag) {
	if (!tag->markers.empty()) {
		ar_track_alvar_msgs::AlvarMarkers pub;
		pub.header = tag->header;
		pub.header.stamp = tag->markers[0].header.stamp;
		pub.markers = tag->markers;
		marker_pub.publish(pub);
	}
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "stamped_ar_tag");
	ros::NodeHandle n;

    tag_sub = n.subscribe<ar_track_alvar_msgs::AlvarMarkers>("ar_pose_marker", 1, cb);
	marker_pub = n.advertise<ar_track_alvar_msgs::AlvarMarkers>("ar_pose_marker_stamped", 1);

    ros::spin();
    return 0;
}