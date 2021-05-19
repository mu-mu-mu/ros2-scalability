#include <chrono>
#include <functional>
#include <memory>
#include <string>

using std::placeholders::_1;
using namespace std::chrono_literals;

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "pubsub.hpp"


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  //rclcpp::spin(std::make_shared<PubSub>("pubsub", "topic1", "topic2"));
  // HERE
  rclcpp::shutdown();
  return 0;
}
