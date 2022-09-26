#include "pci.hpp"

namespace {
    uint16_t CONFIG_ADDRESS_REGISTER = 0x0cf8;
    uint16_t CONFIG_DATA_REGISTER    = 0x0cfc;

    uint32_t MakeConfigAddress(uint8_t bus, uint8_t device, uint8_t function, uint8_t off_set) {
        return 1 << 31 | bus << 16 | device << 11 | function << 8 | (off_set & 0xfcu);
    }

    uint32_t ReadConfigAreaRow(uint8_t bus, uint8_t device, uint8_t function, uint8_t off_set) {
        uint32_t config_address = MakeConfigAddress(bus, device, function, off_set);
        uint32_t config_area;
        __asm__ volatile("outl %%eax, %%dx":: "a"(config_address), "d"(CONFIG_ADDRESS_REGISTER));
        __asm__ volatile("inl %%dx, %%eax": "=a"(config_area): "d"(CONFIG_ADDRESS_REGISTER));
        return config_area;
    }

    uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function) {
        return ReadConfigAreaRow(bus, device, function, 0xc) & 0xffffu;
    }

    uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
        return ReadConfigAreaRow(bus, device, function, 0xc) >> 16 & 0xffu;
    }
}

Error ScanAllBus() {
    uint8_t header_type = ReadHeaderType(0, 0, 0);
    uint8_t function_num = (header_type & 0x80)? 8: 1;
    for (uint8_t function = 0; function < function_num; function++) {
        if (ReadVendorId(0, 0, function) == 0xffffu) continue;
        // if (Error err = ScanBus(function)) return err;
    }
    return MakeError(Error::Success);
}