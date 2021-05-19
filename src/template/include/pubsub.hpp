
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "common.hpp"
#include "rclcpp/rclcpp.hpp"

using std::placeholders::_1;
using namespace std::chrono_literals;
using namespace std;
class PubSub : public rclcpp::Node
{
  public:
    PubSub(string node_name, string sub_topic, string pub_topic)
    : Node(node_name), count_(0)
    {
      publisher_ = this->create_publisher<Payload>(pub_topic, 100);
      subscription_ = this->create_subscription<Payload>(
      sub_topic, 10, std::bind(&PubSub::topic_callback, this, _1));
    }

  private:
    void topic_callback(const Payload::SharedPtr msg) const
    {
      auto message = Payload();
      message.time = msg->time;
      publisher_->publish(message);
    }

    rclcpp::Subscription<Payload>::SharedPtr subscription_;
    rclcpp::Publisher<Payload>::SharedPtr publisher_;
    size_t count_;
};
