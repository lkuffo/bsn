#include "Injector.hpp"


Injector::Injector(int  &argc, char **argv, const std::string &name) : ROSComponent(argc, argv, name), cycles(0), duration(), frequency(), amplitude(), noise_factor(), begin(), end(), type() {}
Injector::~Injector() {}

void Injector::setUp() {
    srand(time(NULL));
    log_uncertainty = handle.advertise<archlib::Uncertainty>("log_uncertainty", 10);

    ros::NodeHandle config;
    
    double freq;
    config.getParam("frequency", freq);
    rosComponentDescriptor.setFreq(freq);

    std::string comps;
    config.getParam("components", comps);
    components = bsn::utils::split(comps, ',');

    for (std::vector<std::string>::iterator component = components.begin(); component != components.end(); ++component){
        uncertainty_pub[*component] = handle.advertise<archlib::Uncertainty>("uncertainty_/"+(*component), 1000);
        config.getParam((*component) + "/type", type[*component]);
        config.getParam((*component) + "/offset", noise_factor[*component]);
        config.getParam((*component) + "/amplitude", amplitude[*component]);
        config.getParam((*component) + "/frequency", frequency[*component]);
        config.getParam((*component) + "/duration", duration[*component]);
        int beg;
        config.getParam((*component) + "/begin", beg);
        begin[*component] = seconds_in_cycles(beg);
        end[*component] = begin[*component] + seconds_in_cycles(duration[*component]);

        config.getParam((*component) + "/volt_duration", volt_duration[*component]);
        config.getParam((*component) + "/volt_frequency", volt_frequency[*component]);
        config.getParam((*component) + "/volt_frequency", volt_factor[*component]); // dummy
        config.getParam((*component) + "/volt_min", volt_min[*component]);
        config.getParam((*component) + "/volt_max", volt_max[*component]);
        int volt_beg;
        config.getParam((*component) + "/volt_begin", volt_beg);
        volt_begin[*component] = seconds_in_cycles(volt_beg);
        volt_end[*component] = volt_begin[*component] + seconds_in_cycles(volt_duration[*component]);
    }
}

void Injector::tearDown() {}

int64_t Injector::seconds_in_cycles(const double &seconds){
    return seconds*rosComponentDescriptor.getFreq();
}

int64_t Injector::cycles_in_seconds(const double &cycles) {
    return cycles/rosComponentDescriptor.getFreq();
}

double Injector::gen_noise(const std::string &component, double &noise, int &duration, double &amplitude, std::string &type){

    bool is_last_cycle = (cycles == end[component])?true:false;

    if (type=="step" && !is_last_cycle){
        return amplitude;
    } else if (type=="ramp" && !is_last_cycle) {
        return noise + amplitude/seconds_in_cycles(duration);
    } else if (type=="random") {
        return ((double) rand() / (RAND_MAX)) * amplitude;
    } 
    
    return 0.0;
}

double Injector::gen_volt_noise(const std::string &component, double &volt_min_amp, double &volt_max_amp){

    bool is_last_cycle = (cycles == volt_end[component])?true:false;

    if (!is_last_cycle){
        return 1; // no noise during period before change
    }

    return ((double)rand()) / ((double)RAND_MAX) * (volt_max_amp - volt_min_amp) + volt_min_amp;
    
}

void Injector::body() {
    ++cycles;

    for (std::vector<std::string>::iterator component = components.begin(); component != components.end(); ++component){
        if (begin[*component] <= cycles && cycles <= end[*component]) {

            noise_factor[*component] = gen_noise(*component, noise_factor[*component], duration[*component], amplitude[*component], type[*component]);
            inject(*component, "noise_factor=" + std::to_string(noise_factor[*component]));

            //update begin and end tags in last cycle
            if (cycles == end[*component]){
                begin[*component] += seconds_in_cycles(1.0/frequency[*component]);
                end[*component]   += seconds_in_cycles(1.0/frequency[*component]);
            }
        }

        if (volt_begin[*component] <= cycles && cycles <= volt_end[*component]) {

            volt_factor[*component] = gen_volt_noise(*component, volt_min[*component], volt_max[*component]);
            inject(*component, "voltage_factor=" + std::to_string(volt_factor[*component]));

            //update begin and end tags in last cycle
            if (cycles == volt_end[*component]){
                volt_begin[*component] += seconds_in_cycles(1.0/volt_frequency[*component]);
                volt_end[*component]   += seconds_in_cycles(1.0/volt_frequency[*component]);
            }
        }

    }
}

void Injector::inject(const std::string &component, const std::string &content){
    archlib::Uncertainty msg;

    msg.source = ros::this_node::getName();
    msg.target = "/"+component;
    msg.content = content;

    uncertainty_pub[component].publish(msg);
    log_uncertainty.publish(msg);
    ROS_INFO("Inject [%s] at [%s].", content.c_str(), component.c_str());
}
