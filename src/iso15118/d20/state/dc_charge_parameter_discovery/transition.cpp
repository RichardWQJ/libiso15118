// SPDX-License-Identifier: Apache-2.0
// Copyright 2023 Pionix GmbH and Contributors to EVerest
#include <iso15118/d20/state/dc_charge_parameter_discovery.hpp>

#include <iso15118/d20/state/schedule_exchange.hpp>

#include <iso15118/detail/d20/state/dc_charge_parameter_discovery.hpp>
#include <iso15118/detail/helper.hpp>

namespace iso15118::d20::state {

void DC_ChargeParameterDiscovery::enter() {
    ctx.log.enter_state("DC_ChargeParameterDiscovery");
}

FsmSimpleState::HandleEventReturnType DC_ChargeParameterDiscovery::handle_event(AllocatorType& sa, FsmEvent ev) {
    if (ev == FsmEvent::NEW_V2GTP_MESSAGE) {
        auto variant = ctx.get_request();
        if (variant->get_type() != message_20::Type::DC_ChargeParameterDiscoveryReq) {
            ctx.log("expected DC_ChargeParameterDiscovery! But code type id: %d", variant->get_type());
            return sa.PASS_ON;
        }

        const auto& req = variant->get<message_20::DC_ChargeParameterDiscoveryRequest>();

        const auto& max_current = std::get<1>(req.transfer_mode).max_charge_current;

        logf("Max charge current %de%d\n", max_current.value, max_current.exponent);

        const auto& max_discharge_current = std::get<1>(req.transfer_mode).max_discharge_current;

        logf("Max discharge current %de%d\n", max_discharge_current.value, max_discharge_current.exponent);

        const auto& res = handle_request(req, ctx.session, ctx.config);

        ctx.respond(res);

        return sa.create_simple<ScheduleExchange>(ctx);
    }

    return sa.PASS_ON;
}

} // namespace iso15118::d20::state