// wsn test pgm to command joint values to Baxter
// accept keyboard commands
#ifndef BAXTER_TRAJ_STREAMER_H_
#define BAXTER_TRAJ_STREAMER_H_

#include <ros/ros.h> //Must include this for all ROS cpp projects
#include <ros/init.h>
#include <std_msgs/Float32.h> //Including the Float32 class from std_msgs
#include <baxter_core_msgs/JointCommand.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include <sensor_msgs/JointState.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <cwru_srv/simple_bool_service_message.h> // this is a pre-defined service message, contained in shared "cwru_srv" package


typedef Eigen::Matrix<double, 6, 1> Vectorq6x1;
typedef Eigen::Matrix<double, 7, 1> Vectorq7x1;
using namespace std;
const double q0dotmax = 0.5;
const double q1dotmax = 0.5;
const double q2dotmax = 0.5;
const double q3dotmax = 0.5;
const double q4dotmax = 1;
const double q5dotmax = 1;
const double q6dotmax = 1;
const double dt_traj = 0.01; // time step for trajectory interpolation



class Baxter_traj_streamer
{
public:
    Baxter_traj_streamer(ros::NodeHandle* nodehandle); //"main" will need to instantiate a ROS nodehandle, then pass it to the constructor
    // may choose to define public methods or public variables, if desired
    void cmd_pose_right(Vectorq7x1 qvec );
    void stuff_trajectory( std::vector<Vectorq7x1> qvecs, trajectory_msgs::JointTrajectory &new_trajectory);
    void stuff_trajectory( std::vector<Eigen::VectorXd> qvecs, trajectory_msgs::JointTrajectory &new_trajectory);    

    void pub_right_arm_trajectory(trajectory_msgs::JointTrajectory &new_trajectory);
    Vectorq7x1 get_qvec_right_arm();  
    void pub_right_arm_trajectory_init();

 private:
    // put private member data here;  "private" data will only be available to member functions of this class;
    ros::NodeHandle nh_; // we will need this, to pass between "main" and constructor
    // some objects to support subscriber, service, and publisher
    ros::Subscriber joint_state_sub_; //these will be set up within the class constructor, hiding these ugly details
    //ros::ServiceServer traj_interp_stat_client_;
    ros::ServiceClient traj_interp_stat_client_;
    ros::Publisher  joint_cmd_pub_right_,joint_cmd_pub_left_;
    ros::Publisher  right_traj_pub_;
        
    double val_from_subscriber_; //example member variable: better than using globals; convenient way to pass data from a subscriber to other member functions
    double val_to_remember_; // member variables will retain their values even as callbacks come and go
    Vectorq7x1 q_vec_right_arm_; //,q_in,q_soln,q_snapshot; 
    Vectorq7x1 qdot_max_vec; // velocity constraint on each joint for interpolation
    baxter_core_msgs::JointCommand right_cmd_,left_cmd_;  // define instances of these message types, to control arms    
    cwru_srv::simple_bool_service_message traj_status_srv_;
    
    // member methods as well:
    void initializeSubscribers(); // we will define some helper methods to encapsulate the gory details of initializing subscribers, publishers and services
    void initializePublishers();
    void initializeServices();
    void jointStatesCb(const sensor_msgs::JointState& js_msg); //prototype for callback of joint-state messages
    void map_right_arm_joint_indices(vector<string> joint_names);
    
    double transition_time(Vectorq7x1 dqvec);
    double transition_time(Eigen::VectorXd dqvec);
    //prototype for callback for example service
    //bool serviceCallback(cwru_srv::simple_bool_service_messageRequest& request, cwru_srv::simple_bool_service_messageResponse& response);
}; // note: a class definition requires a semicolon at the end of the definition

#endif