#pragma once

//! Route constants for the HTTP microservice.

namespace HttpRoutes {
    constexpr const char* LED_OFF = "/led/off";
    constexpr const char* LED_ON = "/led/on";
    constexpr const char* LED_TOGGLE = "/led/toggle";
    constexpr const char* LED_STATUS = "/led/status";
    constexpr const char* TEMP = "/temp";
    constexpr const char* TEMP_MIN = "/temp/min";
    constexpr const char* TEMP_MAX = "/temp/max";
    constexpr const char* TEMP_RAW = "/temp/raw";
    constexpr const char* TEMP_ALERT_MIN = "/temp/alert/min";
    constexpr const char* TEMP_ALERT_MAX = "/temp/alert/max";
    constexpr const char* TEMP_ALERT_CLEAR = "/temp/alert/clear";
}

