#pragma once

enum Routes {
    LED_OFF,
    LED_ON,
    LED_TOGGLE,
    LED_STATUS
};

namespace HttpRoutes {
    constexpr const char* LED_OFF = "/led/off";
    constexpr const char* LED_ON = "/led/on";
    constexpr const char* LED_TOGGLE = "/led/toggle";
    constexpr const char* LED_STATUS = "/led/status";
}

