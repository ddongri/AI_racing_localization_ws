// 내가 만들다 만 것

// #include <ros/ros.h>
// #include <ublox_msgs/ublox_msgs.h>

// using namespace std;

// class GpsMode
// {
// public:
//     GpsMode(ros::NodeHandle &nh):
//     flagSub_(nh.subscribe("/ublox_gps/navrelposned", 10, &GpsMode::rtk_flag, this)),
//     loop_rate(20)
//     {}

//     void spin()
//     {
//         while(ros::ok())
//         {
//             ros::spinOnce();
//             loop_rate.sleep();
//         }
//     }

//     void rtk_flag(const ublox_msgs::NavRELPOSNED &flag_msg)
//     {
//         if((flag_msg.flags % 32) > 15) // 2^4 자리 존재
//         GpsMode::fix_or_not_ = true;
        
//         else
//         GpsMode::fix_or_not_ = false;

//         printf("fix_or_not_: %d", fix_or_not_);
//     }

//     void fix_CB(const sensor_msgs::NavSatFix &navsat_msg)
//     {
//         double utm_x = 0, utm_y = 0;
//         std::string utm_zone;
//         //convert lat/long to utm
//         RobotLocalization::NavsatConversions::LLtoUTM(navsat_msg.latitude, navsat_msg.longitude, utm_x, utm_y, utm_zone);

//         if(!navSat_msg_received){
//             if(gps_origin_x_ == 0 && gps_origin_y_ == 0)
//             {
//             gps_origin_x_ = utm_x;
//             gps_origin_y_ = utm_y;
//             }
//             odom_origin_x_ = utm_x;
//             odom_origin_y_ = utm_y;
//             navSat_msg_received = true;
//         }

//         map2gps.header.stamp = ros::Time::now();
//         map2gps.pose.pose.position.x = utm_x - odom_origin_x_;
//         map2gps.pose.pose.position.y = -(utm_y - odom_origin_y_);
//         map2gps.pose.pose.position.z = 0;
//     }

// private:
//     /*Subscriber */
//     ros::Subscriber flagSub_;
    
//     /*Publisher */
//     //ros::Publisher odom_pub;


//     /*Transform Broadcaster */

//     /*Parameter*/

//     /*ROS Variable*/
//     ros::NodeHandle nh;
//     ros::Rate loop_rate;  

//     bool fix_or_not_ = false;
//     string initial_estimate_covariance_ = "initial_estimate_covariance_float";
//     string process_noise_covariance_ = "process_noise_covariance_float";

//     /*Message Types*/
		
// };

// int main(int argc, char **argv)
// {
//     ros::init(argc, argv, "gps_mode_node");
//     ros::NodeHandle nh;
//     GpsMode core(nh);
//     core.spin();
//     return 0;
// }
