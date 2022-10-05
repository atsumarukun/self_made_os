#pragma once

struct {
    uint8_t CAPLENGTH;
    uint8_t Rsvd;
    uint16_t HCIVERSION;

    union HCSPARAMS1 {
        uint32_t data;
        struct {
            uint32_t MaxSlots: 8;
            uint32_t MaxIntrs: 11;
            uint32_t     Rsvd: 5;
            uint32_t MaxPorts: 8;
        } __attribute__ bits;
    } __attribute__((packed));

    union HCSPARAMS2 {
        uint32_t data;
        struct {
            uint32_t                 IST: 4;
            uint32_t             ERSTMax: 4;
            uint32_t                Rsvd: 13;
            uint32_t MaxScratchpadBufsHi: 5;
            uint32_t                 SPR: 1;
            uint32_t MaxScratchpadBufsLo: 5;
        } __attribute__ bits;
    } __attribute__((packed));

    union HCSPARAMS3 {
        uint32_t data;
        struct {
            uint32_t U1DeviceExitLatency: 8;
            uint32_t                Rsvd: 8;
            uint32_t U2DeviceExitLatency: 16;
        } __attribute__ bits;
    } __attribute__((packed));
} __attribute__((packed)) CapabilityRegisters;

struct {
    union USBCMD {
        uint32_t data;
        struct {
            uint32_t     RS: 1;
            uint32_t  HCRST: 1;
            uint32_t   INTE: 1;
            uint32_t   HSEE: 1;
            uint32_t       : 3;
            uint32_t LHCRST: 1;
            uint32_t    CSS: 1;
            uint32_t    CRS: 1;
            uint32_t    EWE: 1;
            uint32_t   EU3S: 1;
            uint32_t       : 1;
            uint32_t    CME: 1;
            uint32_t    ETE: 1;
            uint32_t TSC_EN: 1;
            uint32_t  VTIOE: 1;
            uint32_t       : 15;
        } __attribute__((packed)) bits;
    } __attribute__((packed));

    union USBSTS {
        uint32_t  data;
        struct {
            uint32_t  HCH: 1;
            uint32_t     : 1;
            uint32_t  HSE: 1;
            uint32_t EINT: 1;
            uint32_t  PCD: 1;
            uint32_t     : 3;
            uint32_t  SSS: 1;
            uint32_t  RSS: 1;
            uint32_t  SRE: 1;
            uint32_t  CNR: 1;
            uint32_t  HCE: 1;
            uint32_t     : 19;
        } __attribute__((packed)) bits;
    } __attribute__((packed));
} __attribute__((packed)) OperationalRegisters;