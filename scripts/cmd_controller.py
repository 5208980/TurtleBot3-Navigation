#! /usr/bin/python
"""
A basic ros node written in python that publishes commands typed into
/cmd. Other nodes interested in this command should listen to this topic,
parse the string in the callback and do something. 

Type of the message is a ros std_msgs String

You can expand this to parse and send/publish other things too such as goals,
or twist(drive) commands. 

# Contains Node to detect the robots start position and cmds it to stop there.
"""

import rospy
import message_filters
import sys
import threading
import time

# Note - for debugging you can also do something similar here with goal. 
# Send a PoseStamped into the goal topic. 
# from geometry_msgs.msg import Pose, PoseStamped
from std_msgs.msg import String
# from geometry_msgs.msg import Pose
from nav_msgs.msg import Odometry


# GLOBAL
cmd_pub = rospy.Publisher('/cmd', String, queue_size=10)
threshold = 0.20

# Used to handle whenever the robots starts. To ensure that the pose check doesn't affect the start of robot for 5 seconds.
starting_up = False
def start_up_handler():
	global starting_up
	starting_up = not starting_up
	time.sleep(45)
	print("Swapped")
	starting_up = not starting_up
	sys.exit()
	
# Compares the initial pose to current pose. If true then stop
def pose_check(init_odom, odom):
	global starting_up
	low_init_x = init_odom.pose.pose.position.x - threshold
	up_init_x = init_odom.pose.pose.position.x + threshold
	low_init_y = init_odom.pose.pose.position.y - threshold
	up_init_y = init_odom.pose.pose.position.y + threshold
	
	curr_x = odom.pose.pose.position.x
	curr_y = odom.pose.pose.position.y

	#print("X: lower: {} upper: {:.2f} curr: {:.2f}".format(round(up_init_x, 2) >= round(curr_x, 2), up_init_x, curr_x))
	#print("Y: lower: {:.2f} upper: {:.2f} curr: {:.2f}".format(low_init_y, up_init_y, curr_y))
	#print("=======")
	if (round(low_init_x, 2) <= round(curr_x, 2) and round(up_init_x, 2) >= round(curr_x, 2)) and (round(low_init_y, 2) <= round(curr_y, 2) and round(up_init_y, 2) >= round(curr_y, 2)):
		# print(starting_up)
		if not starting_up:
			cmd_pub.publish("stop")

def send_goal_loop():
	while True:
		# Wait for a user to print something. 
		command = raw_input()

		# Clean-ish exit
		if command == '\n':
			break
		elif command == '':
			break
		else: 
			print "Sending command: " + command
			# Publishes the command. 
			cmd_pub.publish(command)
			t1 = threading.Thread(target=start_up_handler);
			t1.start(); 

		# publish message here
		rospy.Rate(10).sleep()

if __name__ == "__main__":
	rospy.init_node('command_controller_node')
	init_odom_sub = message_filters.Subscriber("initialodom", Odometry)
	odom_sub = message_filters.Subscriber("odom", Odometry)
	# cmd_sub = message_filters.Subscriber("/cmd", String)
	ts = message_filters.ApproximateTimeSynchronizer([init_odom_sub, odom_sub], 10, 10)
	# ts = message_filters.TimeSynchronizer([init_odom_sub, odom_sub], 1)
  	ts.registerCallback(pose_check)
	send_goal_loop()
