#pragma once

#include <vector>

using namespace std;

struct ClassCode {
    uint8_t base_class, sub_class, interface;
};

struct Device {
    uint8_t bus, device, function, header_type;
    uint16_t vendor_id;
    ClassCode class_code;
};

class PCI {
    public:
        vector<Device> devices{};

        PCI();

    private:

        uint32_t MakeConfigAddress(uint8_t bus, uint8_t device, uint8_t function, uint8_t off_set);
        uint32_t ReadConfigAreaRow(uint8_t bus, uint8_t device, uint8_t function, uint8_t off_set);
        uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function);
        uint8_t  ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function);
        ClassCode ReadClassCode(uint8_t bus, uint8_t device, uint8_t function);
        uint8_t ReadSecondaryBus(uint8_t bus, uint8_t device, uint8_t function);
        void ScanFunction(uint8_t bus, uint8_t device, uint8_t function);
        void ScanDevice(uint8_t bus, uint8_t device);
        void ScanBus(uint8_t bus);
};