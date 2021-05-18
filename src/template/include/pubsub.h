
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;
using namespace std::chrono_literals;
using namespace std;
class PubSub : public rclcpp::Node
{
  public:
    PubSub(string node_name, string sub_topic, string pub_topic)
    : Node(node_name), count_(0)
    {
      publisher_ = this->create_publisher<std_msgs::msg::String>(pub_topic, 100);
      subscription_ = this->create_subscription<std_msgs::msg::String>(
      sub_topic, 10, std::bind(&PubSub::topic_callback, this, _1));
    }

  private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
    {
      auto message = std_msgs::msg::String();
      message.data = msg->data.c_str();
      publisher_->publish(message);
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};
