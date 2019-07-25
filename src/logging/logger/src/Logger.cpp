#include "Logger.hpp"

Logger::Logger(int  &argc, char **argv, std::string name) : fp(), filepath(), logical_clock(0), task_logger_manager_pub(), context_logger_manager_pub() {}
Logger::~Logger() {}

std::string Logger::now() const{
    return std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

void Logger::setUp() {
    std::string path = ros::package::getPath("logger");
    std::string now = this->now();

    filepath = path + "/resource/logs/" + now + ".log";
    std::cout << filepath;

    fp.open(filepath, std::fstream::in | std::fstream::out | std::fstream::trunc);
    fp << "\n";
    fp.close();

    ros::NodeHandle handler;
    task_logger_manager_pub = handler.advertise<messages::TaskInfo>("logger_manager", 10);
    context_logger_manager_pub = handler.advertise<messages::ContextInfo>("logger_manager", 10);

}

void Logger::receiveTaskInfo(const messages::TaskInfo::ConstPtr& msg) {
    //ROS_INFO("I heard: [%s]", msg->task_id.c_str());
    ++logical_clock;

    // persist
    fp.open(filepath, std::fstream::in | std::fstream::out | std::fstream::app);
    fp << logical_clock << ",";
    fp << this->now() << ",";
    fp << msg->task_id << ",";
    fp << msg->cost << ",";
    fp << msg->reliability << ",";
    fp << msg->frequency << "\n";

    fp.close();

    //forward
    task_logger_manager_pub.publish(msg);
}

void Logger::receiveContextInfo(const messages::ContextInfo::ConstPtr& msg) {
    //ROS_INFO("I heard: [%s]", msg->context_id.c_str());
    ++logical_clock;

    // persist
    fp.open(filepath, std::fstream::in | std::fstream::out | std::fstream::app);
    fp << logical_clock << ",";
    fp << this->now() << ",";
    fp << msg->context_id << ",";
    fp << msg->status << "\n";

    fp.close();

    //forward
    context_logger_manager_pub.publish(msg);
}

void Logger::run(){
    ros::NodeHandle n;
    ros::Subscriber t_sub = n.subscribe("probe_logger", 1000, &Logger::receiveTaskInfo, this);
    ros::Subscriber c_sub = n.subscribe("probe_logger", 1000, &Logger::receiveContextInfo, this);
    ros::spin();
}
