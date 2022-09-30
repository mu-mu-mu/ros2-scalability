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
    : Node("sub"), _count(0)
    {
      subscription_ = this->create_subscription<Payload>(
      "topic_end", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
    }

  private:
    void topic_callback(const Payload::SharedPtr msg) const
    {
      int i;
      auto now = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch());
      auto delta = now.count() - msg->time;

      char buff[100];
      snprintf(buff, sizeof(buff), "Subscribed %ld || ",delta/(1000));
      std::string res = buff;

      for(i = 0; i < MAX_LEN; i++) {
	      char buff[100];
	      if(msg->tlcore[i] == -1) break;
	      snprintf(buff, sizeof(buff), "%ld ", msg->tlcore[i]);
	      std::string buffs = buff;
	      res += buffs;
      }
      
      res += " || ";

      for(i = 0; i < MAX_LEN; i++) {
	      char buff[100];
	      if(msg->tlcore[i] == -1) break;

	      auto d = now.count() - msg->time;
	      if(i == 0) d = msg->tltime[i] - msg->time;
	      else d = msg->tltime[i] - msg->tltime[i-1] ;

	      snprintf(buff, sizeof(buff), "%ld ", d);
	      std::string buffs = buff;
	      res += buffs;
      }

      snprintf(buff, sizeof(buff), "%ld", now.count() - msg->tltime[i-1]);
      res += buff;

      res += " || ";

      for(i = 0; i < MAX_LEN; i++) {
	      char buff[100];
	      if(msg->tlcore[i] == -1) break;

	      snprintf(buff, sizeof(buff), "%ld ", msg->tlherz[i]);
	      std::string buffs = buff;
	      res += buffs;
      }


      RCLCPP_INFO(this->get_logger(), "%s", res.c_str());

      if (_count >= MAX_COUNT) {
        rclcpp::shutdown();
      }
    }
    rclcpp::Subscription<Payload>::SharedPtr subscription_;
    int _count;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
