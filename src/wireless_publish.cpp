
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/msgs/wireless_node.pb.h>
#include <gazebo/msgs/wireless_nodes.pb.h>
#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/propagation_grid.pb.h>
#include <iostream>
#include <sensor_msgs/ChannelFloat32.h>
#include <gazebo_wireless/Rss_level.h>
//#include <gazebo/gazebo_client.hh>
//gazebo::msgs::WirelessNodes NodesMsg;

  int nthSubstr(int n, const std::string& s,
              const std::string& p) {
              size_t i = s.find(p);     // Find the first occurrence

   int j;
   for (j = 1; j < n && i != std::string::npos; ++j)
      i = s.find(p, i+1); // Find the next occurrence

   if (j == n)
     return(i);
   else
     return(-1);
}



float sig_level1;
float sig_level2;
float sig_level3;
void receiver_subs(ConstWirelessNodesPtr &msg)
{
    std::string raw_msg = msg->DebugString();
    size_t occur1 = raw_msg.find("signal_level:");
    size_t occur2 = raw_msg.find("signal_level:", occur1+1);
    size_t occur3 = raw_msg.find("signal_level:", occur2+1);
    //size_t occur1 = 43; size_t occur2 = 121; size_t occur3 = 201;  


    std::string sig_lev_str1 = raw_msg.substr(occur1+13, 15);
    std::string sig_lev_str2 = raw_msg.substr(occur2+13, 15);
    std::string sig_lev_str3 = raw_msg.substr(occur3+13, 15);
    
    sig_level1 = std::stof(sig_lev_str1);
    sig_level2 = std::stof(sig_lev_str2);
    if (occur3 <1000){
      sig_level3 = std::stof(sig_lev_str3);
    } else{
       sig_level3 = 0.0; 
    }
    
    std::cout << sig_lev_str1 <<"\n"<<sig_lev_str2 <<"\n"<<sig_lev_str3 <<"\n-----------------\n" <<std::endl;

}

int main(int _argc, char **_argv)
{

  gazebo::client::setup(_argc, _argv);
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  
  node->Init ();
  // start transportation
  //gazebo::transport::run();

  // Subscribe to a topic
  gazebo::transport::SubscriberPtr sub_2 = node->Subscribe("/gazebo/default/iris_opt_flow/wirelessReceiver/link/wirelessReceiver/transceiver", receiver_subs);
  //publish on Gazebo
  //gazebo::transport::PublisherPtr sensor_pub = node->Advertise <gazebo::msgs::WirelessNodes>("/sensor/receiver", 1000, (1.0));
  //sensor_pub -> Publish(NodesMsg);
  //sensor_msgs::ChannelFloat32 Rec_msgs ;



  //publish on Ros



  ros::init(_argc, _argv, "talker");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<gazebo_wireless::Rss_level>("/receiver_rss", 300);
  gazebo_wireless::Rss_level NodeMsg;
  NodeMsg.signal_level1 = sig_level1;
  NodeMsg.signal_level2 = sig_level2;
  NodeMsg.signal_level3 = sig_level3;
  

  ros::Rate loop_rate(10);

  int count = 0;
    while (ros::ok())
   {
  chatter_pub.publish(NodeMsg);
  ros::spinOnce(); 
  loop_rate.sleep();
  ++count;
   }
 
  // shutting down
  while (true)
    gazebo::common::Time::MSleep(10);

  // Make sure to shut everything down.
   gazebo::client::shutdown();
}