#include <iostream>
#include <chrono>
#include <cstdio>
#include <string>
#include <cassert>
#include "xarm/wrapper/xarm_api.h"
#include "HardwareAPI.h"

using namespace std::chrono;
namespace API = Haply::HardwareAPI;

struct QuillHandle : public API::Devices::Handle {
    QuillHandle(API::IO::SerialStream *stream) : Handle(stream) {}

    uint8_t user_data[4] = {0};

    // Add a public method to get the user_data
    const uint8_t* getUserData() const {
      return user_data;
  }


  protected:
    void OnReceiveHandleInfo(uint8_t, uint16_t device_id,
                             uint8_t device_model_number,
                             uint8_t hardware_version,
                             uint8_t firmware_version) override {
        std::fprintf(
            stdout,
            "info: id=%04x, model=%u, version={hardware:%u, firmware:%u}\n",
            device_id, device_model_number, hardware_version, firmware_version);
    }

    void OnReceiveHandleStatusMessage(uint16_t, float *quaternion, uint8_t,
                                      uint8_t attached,
                                      [[maybe_unused]] uint8_t user_data_length,
                                      uint8_t *user_data) override {
        assert(user_data_length == 4);
        uint8_t battery = user_data[3];

        std::fprintf(stdout,
                     "\r"
                     "attached=%u, battery=%03u, buttons=[ %u, %u, %u ], "
                     "quaternion=[ % 0.3f % 0.3f % 0.3f % 0.3f ]",
                     attached, battery, user_data[0], user_data[1],
                     user_data[2], quaternion[0], quaternion[1], quaternion[2],
                     quaternion[3]);

        // Store the user_data in the member variable
        std::copy(user_data, user_data + 4, this->user_data);
    }
};

XArmAPI* init_xarm(const std::string &device_id) {
    XArmAPI *arm = new XArmAPI(device_id.c_str());
    sleep_milliseconds(1000);
    if (arm->error_code != 0) arm->clean_error();
    if (arm->warn_code != 0) arm->clean_warn();
    return arm;
}

int main() {

    std::string xarm_id = "192.168.1.188";
    XArmAPI *arm = init_xarm(xarm_id);

    // Detect the first available VerseGrip handle
    auto handle_list = API::Devices::DeviceDetection::DetectWiredHandles();
    if (handle_list.empty()) {
        std::fprintf(stderr, "no handles detected\n");
        return 1;
    }

    std::string handle_port = handle_list[0];
    std::fprintf(stdout, "Using handle: %s\n", handle_port.c_str());

    API::IO::SerialStream stream(handle_port.c_str());
    QuillHandle device{&stream};

    device.SendDeviceWakeup();
    (void)device.Receive();

    auto current = high_resolution_clock::now();
    const auto print_delay = milliseconds(100);
    int last_val = 0;
    while (true) {
        device.RequestStatus();
        (void)device.Receive();

        const auto now = high_resolution_clock::now();
        if (now > current + print_delay) {
            current = now;
        }

        if (device.getUserData()[0] == last_val){
          continue;
        }

        else if (device.getUserData()[0] == 1) {
            last_val = 1;
        }
        else if (device.getUserData()[0] == 0){
          last_val = 0;
        }

        if (last_val == 1){
          arm->set_vacuum_gripper(true);
        }
        else if (last_val == 0){
          arm->set_vacuum_gripper(false);
        }

        std::this_thread::sleep_for(milliseconds(1));
    }
//
//delete arm;
    return 0;
}
