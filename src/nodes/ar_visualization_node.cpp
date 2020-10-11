#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

// #include <visualization_msgs/MarkerArray.h>

#include <ar_track_alvar_msgs/AlvarMarkers.h>

ros::Publisher marker_pub;
ros::Subscriber visual_marker_sub;

uint32_t shape = visualization_msgs::Marker::SPHERE;

// visualization_msgs::MarkerArray marker_array;

void visualise_marker_cb(ar_track_alvar_msgs::AlvarMarkers req) {
	if (!req.markers.empty()) {
		ROS_INFO("Marker FOUND");
		visualization_msgs::Marker marker;
		marker.header.frame_id = "base_footprint";
		// marker.header.frame_id = "camera_rgb_optical_frame";
		marker.header.stamp = ros::Time::now();

		marker.ns = "basic_shapes";
		marker.id = req.markers[0].id;

		marker.type = shape;

		marker.action = visualization_msgs::Marker::ADD;

		marker.pose.position.x = req.markers[0].pose.pose.position.x;
		marker.pose.position.y = req.markers[0].pose.pose.position.y;
		marker.pose.position.z = req.markers[0].pose.pose.position.z;
		marker.pose.orientation.x = req.markers[0].pose.pose.orientation.x;
		marker.pose.orientation.y = req.markers[0].pose.pose.orientation.y;
		marker.pose.orientation.z = req.markers[0].pose.pose.orientation.z;
		marker.pose.orientation.w = req.markers[0].pose.pose.orientation.w;

		marker.scale.x = 0.2;
		marker.scale.y = 0.2;
		marker.scale.z = 0.4;

		marker.color.r = 0.0f;
		marker.color.g = 0.0f;
		marker.color.b = 1.0f;
		marker.color.a = 1.0;

		// marker.frame_locked = true;

		marker.lifetime = ros::Duration();
	
		while (marker_pub.getNumSubscribers() == 0) {
			ROS_WARN_ONCE("Please create a subscriber to the marker");
			ros::Duration(1.0).sleep();
		}
		marker_pub.publish(marker);
		ROS_INFO("visual marker on rviz");
	} else {
		ROS_WARN_ONCE("No Markers Detected");
	}
}

int main( int argc, char** argv ) {
	ros::init(argc, argv, "basic_shapes");
	ROS_INFO("Visualization Markers Spinnig");
	ros::NodeHandle n;
	
// ros::Rate r(1);
	marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
	visual_marker_sub = n.subscribe("ar_pose_marker", 1, visualise_marker_cb);
	
 	ros::spin();
	return 0;
}


