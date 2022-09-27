#include "pci.hpp"
#include <algorithm>
namespace {
    uint16_t CONFIG_ADDRESS_REGISTER = 0x0cf8;
    uint16_t CONFIG_DATA_REGISTER    = 0x0cfc;
}

PCI::PCI() {
    uint8_t header_type = ReadHeaderType(0, 0, 0);
    uint8_t function_num = (header_type & 0x80)? 8: 1;
    for (uint8_t function = 0; function < function_num; function++) {
        if (ReadVendorId(0, 0, function) == 0xffffu) continue;
        if (Error error = ScanBus(function)) continue;
    }
}

uint32_t PCI::MakeConfigAddress(uint8_t bus, uint8_t device, uint8_t function, uint8_t off_set) {
    return 1 << 31 | bus << 16 | device << 11 | function << 8 | (off_set & 0xfcu);
}

uint32_t PCI::ReadConfigAreaRow(uint8_t bus, uint8_t device, uint8_t function, uint8_t off_set) {
    uint32_t config_address = MakeConfigAddress(bus, device, function, off_set);
    uint32_t config_area;
    __asm__ volatile("outl %%eax, %%dx":: "a"(config_address), "d"(CONFIG_ADDRESS_REGISTER));
    __asm__ volatile("inl %%dx, %%eax": "=a"(config_area): "d"(CONFIG_ADDRESS_REGISTER));
    return config_area;
}

uint16_t PCI::ReadVendorId(uint8_t bus, uint8_t device, uint8_t function) {
    return ReadConfigAreaRow(bus, device, function, 0xc) & 0xffffu;
}

uint8_t PCI::ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
    return ReadConfigAreaRow(bus, device, function, 0xc) >> 16 & 0xffu;
}

ClassCode PCI::ReadClassCode(uint8_t bus, uint8_t device, uint8_t function) {
    uint32_t config_area_row = ReadConfigAreaRow(bus, device, function, 0x8);
    ClassCode class_code;
    class_code.base_class = (config_area_row >> 24) & 0xffu;
    class_code.sub_class = (config_area_row >> 16) & 0xffu;
    class_code.interface = (config_area_row >> 8) & 0xffu;
    return class_code;
}

Error PCI::ScanFunction(uint8_t bus, uint8_t device, uint8_t function) {
    uint8_t header_type = ReadHeaderType(bus, device, function);
    ClassCode class_code = ReadClassCode(bus, device, function);
    devices_.push_back({bus, device, function, header_type, class_code});
    return MakeError(Error::Success);
}

Error PCI::ScanDevice(uint8_t bus, uint8_t device) {
    uint8_t header_type = ReadHeaderType(bus, device, 0);
    uint8_t function_num = (header_type & 0x80)? 8: 1;
    for (uint8_t function = 0; function < function_num; function++) {
        if (ReadVendorId(0, 0, function) == 0xffffu) continue;
        if (Error error = ScanFunction(bus, device, function)) return error;
    }
    return MakeError(Error::Success);
}

Error PCI::ScanBus(uint8_t bus) {
    for (uint8_t device = 0; device < 32; device++) {
        if (ReadVendorId(bus, device, 0) == 0xffffu) continue;
        if (Error error = ScanDevice(bus, device)) return error;
    }
    return MakeError(Error::Success);
}