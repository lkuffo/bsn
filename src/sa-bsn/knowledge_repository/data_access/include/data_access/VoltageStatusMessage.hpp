#ifndef VOLTAGE_STATUS_MESSAGE_HPP
#define VOLTAGE_STATUS_MESSAGE_HPP

#include <string>

class VoltageStatusMessage {

  	public:
		VoltageStatusMessage(const std::string &name, const int64_t &timestamp, const int64_t &logical_clock, const std::string &source, const std::string &target, const std::string &voltage) : name(name), timestamp(timestamp), logical_clock(logical_clock), source(source), target(target), voltage(voltage){};
        
        std::string getName() const { return this->name;};
        int64_t getTimestamp() const {return this->timestamp;};
        int64_t getLogicalClock() const {return this->logical_clock;};
        std::string getSource() const {return this->source;};
        std::string getTarget() const {return this->target;};
        std::string getVoltage() const {return this->voltage;};

	private:
        std::string name;
        int64_t timestamp;
        int64_t logical_clock;
        std::string source;
        std::string target;
        std::string voltage;
};

#endif 