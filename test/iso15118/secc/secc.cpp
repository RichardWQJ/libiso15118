#include <cstdio>
#include <iostream>
#include <iso15118/tbd_controller.hpp>

static void log_debug(std::size_t id, const iso15118::session::logging::Event& event)
{
    std::cout << "log_debug -- id:" << id << std::endl;
}

static void signal(iso15118::session::feedback::Signal signal) {
    std::cout << "signal: " << int(signal) << std::endl;
}

static void dc_charge_target(const iso15118::session::feedback::DcChargeTarget& charge_target) {
    std::cout << "dc_charge_target: " << charge_target.voltage << "V, " << charge_target.current << "A" << std::endl;
}

static void dc_max_limits(const iso15118::session::feedback::DcMaximumLimits& max_limits) {
    std::cout << "dc_max_limits: " << max_limits.voltage << "V, " << max_limits.current << "A, " << max_limits.power << "W" << std::endl;
}


int main(int argc, char* argv[]) {
    iso15118::session::logging::set_session_log_callback(log_debug);

    iso15118::TbdConfig config;
    config.interface_name = "ens33";
    config.tls_negotiation_strategy = iso15118::config::TlsNegotiationStrategy::ENFORCE_NO_TLS;

    iso15118::session::feedback::Callbacks dc_callbacks;
    dc_callbacks.signal = signal;
    dc_callbacks.dc_charge_target = dc_charge_target;
    dc_callbacks.dc_max_limits = dc_max_limits;

    iso15118::TbdController controller(config, dc_callbacks);
    controller.setup_config();

    controller.loop();
    
    return 0;
}
