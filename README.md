# SetUp Proj-1

### Remote
```bash
# new terminal
roscore

# new terminal
roslaunch turtlebot3_slam turtlebot3_slam.launch slam_methods:=gmapping

# new terminal
roslaunch turtlebot3_bringup turtlebot3_remote.launch

# new terminal
roslaunch comp3431_starter ass1.launch
```
**Note**: When working in `Gazebo`, need to change camera topics.

### Turtlebot
```bash
ssh turtlebot
roslaunch turtlebot3_bringup turtlebot3_robot.launch

# new terminal
roslaunch turtlebot3_bringup turtlebot3_rpicamera.launch
```
To ensure that `Markers` on Rviz are in fixed frame. Set `Fixed Frame` as `map`.


### Controls
```bash
roscd comp3431_start/scripts
python cmd_controller.py
```
