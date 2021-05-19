#include <memory>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "common.hpp"
using std::placeholders::_1;

using namespace std::chrono;
using namespace std::chrono_literals;

class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber()
    : Node("sub")
    {
      subscription_ = this->create_subscription<Payload>(
      "topic_end", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
    }

  private:
    void topic_callback(const Payload::SharedPtr msg) const
    {
      auto now = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch());
      auto delta = now.count() - msg->time;

      RCLCPP_INFO(this->get_logger(), "Subscribed %d", delta);
    }
    rclcpp::Subscription<Payload>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
