// SPDX-License-Identifier: Apache-2.0
// Copyright 2023 Pionix GmbH and Contributors to EVerest
#include <iso15118/d20/state/dc_charge_loop.hpp>

#include <iso15118/detail/d20/state/dc_charge_loop.hpp>

#include <iso15118/detail/d20/state/power_delivery.hpp>

#include <iso15118/detail/helper.hpp>

namespace iso15118::d20::state {

void DC_ChargeLoop::enter() {
    ctx.log.enter_state("DC_ChargeLoop");
}

FsmSimpleState::HandleEventReturnType DC_ChargeLoop::handle_event(AllocatorType& sa, FsmEvent ev) {

    if (ev != FsmEvent::V2GTP_MESSAGE) {
        return sa.PASS_ON;
    }

    // TODO(SL): ugly
    static bool first_entry_in_charge_loop = true;

    auto variant = ctx.get_request();

    if (variant->get_type() == message_20::Type::PowerDeliveryReq) {

        const auto& req = variant->get<message_20::PowerDeliveryRequest>();

        const auto& res = handle_request(req, ctx.session);

        ctx.respond(res);

        // Reset
        first_entry_in_charge_loop = true;

        // Todo(sl): React properly to Start, Stop, Standby and ScheduleRenegotiation
        return sa.HANDLED_INTERNALLY;

    } else if (variant->get_type() == message_20::Type::DC_ChargeLoopReq) {

        if (first_entry_in_charge_loop) {
            signal_dc_charge_loop_started();
            first_entry_in_charge_loop = false;
        }
        
        const auto& req = variant->get<message_20::DC_ChargeLoopRequest>();

        const auto& res = handle_request(req, ctx.session);

        ctx.respond(res);

        return sa.HANDLED_INTERNALLY;

    } else {
        ctx.log("Expected PowerDeliveryReq or DC_ChargeLoopReq! But code type id: %d", variant->get_type());
        return sa.PASS_ON;
    }
}

} // namespace iso15118::d20::state
