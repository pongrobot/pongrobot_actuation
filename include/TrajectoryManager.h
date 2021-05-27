#ifndef TRAJECTORY_MANAGER_H
#define TRAJECTORY_MANAGER_H

#include "ros/ros.h"
#include <time.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int8.h>

enum StateEnum
{
    IDLE,
    HAS_TARGET,
    WAIT,
    SHOOT,
    ABORT
};

class TrajectoryManager
{
    public:
        TrajectoryManager( ros::NodeHandle nh );
        void run ();

    private:
        // ROS data
        ros::NodeHandle nh_;
        ros::Subscriber trajectory_pose_sub_;
        ros::Subscriber vesc_ready_sub_;
        ros::Subscriber yaw_ready_sub_;
        ros::Subscriber abort_sub_;
        ros::Publisher vesc_cmd_pub_;
        ros::Publisher yaw_cmd_pub_;
        ros::Publisher trigger_pub_;
        ros::Publisher shot_pub_;
        ros::Publisher state_pub_;

        // Msg data
        geometry_msgs::PoseStamped target_pose_; 
        bool vesc_ready_;
        bool yaw_ready_;
        bool abort_;

        // State data
        bool has_target_;
        StateEnum state_; 
        float target_yaw_;
        float target_velocity_;
        ros::Time trigger_time_; 
        ros::Duration cooldown_time_;
        ros::Time cmd_sent_time_;
        ros::Duration cmd_timeout_;

        // Callbacks
        void trajectoryPoseCallback(const geometry_msgs::PoseStamped::ConstPtr& msg);
        void vescReadyCallback(const std_msgs::Bool::ConstPtr& msg);
        void yawReadyCallback(const std_msgs::Bool::ConstPtr& msg);
        void abortCallback(const std_msgs::Empty::ConstPtr& msg);
        
        // Calculation utilities
        float calculateYawAngle( const geometry_msgs::PoseStamped::ConstPtr& target_pose);
        float calculateInitialVelocity( const geometry_msgs::PoseStamped::ConstPtr& target_pose);
        bool handleAbortSignal();
        bool handleNewCommand();
};

#endif
