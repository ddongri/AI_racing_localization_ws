#include <ros/ros.h>
#include <ros/package.h>
#include <sensor_msgs/NavSatFix.h>
#include <std_msgs/Float32.h>
#include <nav_msgs/Odometry.h>

#include <robot_localization/navsat_conversions.h>

#include <tf2/LinearMath/Quaternion.h>
#include <math.h>

#define Pi 3.141592

using namespace std;

class HeadingAngle
{

public:

    HeadingAngle(ros::NodeHandle &nh):
    gps_sub(nh.subscribe("/fix",100,&HeadingAngle::gps_CB,this)),
    heading_pub(nh.advertise<std_msgs::Float32>("/heading",10)),
    displacement_pub(nh.advertise<nav_msgs::Odometry>("/displacement",10)),
    loop_rate(20)
    {
        init();
    }

    void init(void)
    {
        heading.data = 0.0;

        displacement.header.frame_id = "odom";
        displacement.child_frame_id = "displacement_footprint";
        displacement.pose.pose.position.x = 0.0;
        displacement.pose.pose.position.y = 0.0;
        displacement.pose.pose.position.z = 0.0;
        displacement.pose.pose.orientation.x = 0.0;
        displacement.pose.pose.orientation.y = 0.0;
        displacement.pose.pose.orientation.z = 0.0;
        displacement.pose.pose.orientation.w = 0.0;

        heading_quaternion.setRPY(0, 0, 0);
    }

    void spin()
    {
        while(ros::ok())
        {
            ros::spinOnce();
            heading_pub.publish(heading);
            displacement_pub.publish(displacement);
            loop_rate.sleep();
        }
    }

    void gps_CB(const sensor_msgs::NavSatFix &navsat_msg)
    {
        double utm_x = 0, utm_y = 0;
        std::string utm_zone;

        //convert lat/long to utm
        RobotLocalization::NavsatConversions::LLtoUTM(navsat_msg.latitude, navsat_msg.longitude, utm_x, utm_y, utm_zone);

        if(!navSat_msg_received){
            if(gps_origin_x_ == 0 && gps_origin_y_ == 0)
            {
            gps_origin_x_ = utm_x;
            gps_origin_y_ = utm_y;
            }
            odom_origin_x_ = utm_x;
            odom_origin_y_ = utm_y;
            navSat_msg_received = true;
        }

        position_x_ = utm_x - odom_origin_x_;
        position_y_ = -(utm_y - odom_origin_y_);

        calculate_heading();
    }

    void calculate_heading()
    {
        if (prev_position_x_ == 0 && prev_position_y_ == 0)
        {
            prev_position_x_ = position_x_;
            prev_position_y_ = position_y_;
        }
        else
        {
            displacement.pose.pose.position.x = position_x_ - prev_position_x_;
            displacement.pose.pose.position.y = position_y_ - prev_position_y_;

            heading.data = atan(displacement.pose.pose.position.y / displacement.pose.pose.position.x);

            if(displacement.pose.pose.position.x < 0 && displacement.pose.pose.position.y > 0)
                heading.data = heading.data + Pi;

            else if(displacement.pose.pose.position.x < 0 && displacement.pose.pose.position.y < 0)
                heading.data = heading.data - Pi;
            
            heading_quaternion.setRPY( 0, 0, heading.data );

            displacement.pose.pose.orientation.x = heading_quaternion.getX();
            displacement.pose.pose.orientation.y = heading_quaternion.getY();
            displacement.pose.pose.orientation.z = heading_quaternion.getZ();
            displacement.pose.pose.orientation.w = heading_quaternion.getW();

            prev_position_x_ = position_x_;
            prev_position_y_ = position_y_;
        }
    
    }

private:

    ros::Subscriber gps_sub;
    ros::Publisher heading_pub;
    ros::Publisher displacement_pub;

    ros::NodeHandle nh;
    ros::Rate loop_rate;  

    bool navSat_msg_received = false;
    double gps_origin_x_ = 0;
    double gps_origin_y_ = 0;
    double odom_origin_x_ = 0;
    double odom_origin_y_ = 0;
    float position_x_ = 0;
    float position_y_ = 0;
    float prev_position_x_ = 0;
    float prev_position_y_ = 0;
    int data_index = 0;

    std_msgs::Float32 heading;
    nav_msgs::Odometry displacement;
    tf2::Quaternion heading_quaternion;

};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "heading_angle_publisher");
    ros::NodeHandle nh;
    HeadingAngle core(nh);
    core.spin();
    return 0;
}
