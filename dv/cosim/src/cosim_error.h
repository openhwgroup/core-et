/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>

#include <spdlog/fmt/fmt.h>

#include "events/cosim_evt.h"

//! Base class for errors that CoSim may find
struct cosim_error : public std::exception {
public:
    virtual ~cosim_error() = default;
};

//! We received an unexpected DPI event
struct unexpected_event : public cosim_error {
public:
    using event_id = cosim_evt::event_id;

    unexpected_event(event_id evt) noexcept;
    virtual ~unexpected_event() = default;
    virtual const char* what() const noexcept override;

private:
    event_id evt_id_;
};

//! The checker detected an error, usually a mismatch
struct checker_error : public cosim_error {
public:
    checker_error(std::string err_msg) noexcept;
    virtual ~checker_error() = default;
    virtual const char* what() const noexcept override;

private:
    std::string err_msg_;
};

//! Error when handling messages
struct message_error : public cosim_error {
public:
    message_error() noexcept = default;
    virtual ~message_error() = default;
};
