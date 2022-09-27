#pragma once

#include <memory>
#include <map>
#include <vector>

#include "../error/error.hpp"

using namespace std;

struct ClassCode {
    uint8_t base_class, sub_class, interface;
};

struct Device {
    uint8_t bus, device, function, header_type;
    ClassCode class_code;
};

class PCI {
    public:
        PCI();
        uint64_t GetDevices() {return devices_.size();}

    private:
        vector<Device> devices_{};

        uint32_t MakeConfigAddress(uint8_t bus, uint8_t device, uint8_t function, uint8_t off_set);
        uint32_t ReadConfigAreaRow(uint8_t bus, uint8_t device, uint8_t function, uint8_t off_set);
        uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function);
        uint8_t  ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function);
        ClassCode ReadClassCode(uint8_t bus, uint8_t device, uint8_t function);
        Error ScanFunction(uint8_t bus, uint8_t device, uint8_t function);
        Error ScanDevice(uint8_t bus, uint8_t device);
        Error ScanBus(uint8_t bus);
};