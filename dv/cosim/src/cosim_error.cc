/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "cosim_error.h"

#include <spdlog/fmt/fmt.h>

unexpected_event::unexpected_event(cosim_evt::event_id evt_id) noexcept : evt_id_(evt_id)
{
}

const char* unexpected_event::what() const noexcept
{
    return cosim_evt::event_name(evt_id_);
}

checker_error::checker_error(std::string err_msg) noexcept : err_msg_(err_msg)
{
}

const char* checker_error::what() const noexcept
{
    return err_msg_.c_str();
}
