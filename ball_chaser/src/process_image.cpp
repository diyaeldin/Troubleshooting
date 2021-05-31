#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <ros/console.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    //geometry_msgs::Twist motor_command;

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    // Call the drive_robot service and pass the requested joint angles
    if (!client.call(srv))
        ROS_ERROR("Failed to call service drive_bot");
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    	int white_pixel = 255;
    	int image_res = img.height * img.step;
	float direction = 0.0;
	enum direction{left=0,right=0,mid=0};
	bool white_ball = false;
    
	

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
	for (int i = 0; i < img.height * img.step; i+=3) {
		
		if (img.data[i] == white_pixel && img.data[i+1] == white_pixel && img.data[i+2] == white_pixel){
			white_ball = true;
			auto column = i % img.step;
			if (column < img.step*0.4){
			direction = left;	
			}
		 	else if (column > img.step*0.6){
			direction = right;
			}
			else {
			direction = mid;
			}
			break;
		}   
	}		
		
		if (direction = left){
		drive_robot(0.2,0.5);}
		else if (direction = right){
		drive_robot(0.2,-0.5);}
		else if (direction = mid){
		drive_robot(0.2,0.0);}
		else {drive_robot(0.0,0.0);}

		//ROS_INFO_STREAM("i - " << i);
		//ROS_INFO_STREAM("Image Data - " << (float)img.data[i]);
		//ROS_INFO_STREAM("Image height -" << img.height);
		//ROS_INFO_STREAM("Image width -" << img.step);
		
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);
	

    // Handle ROS communication events
    ros::spin();

    return 0;
}
