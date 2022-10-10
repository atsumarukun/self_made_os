#pragma once

#include <stdint.h>

struct CapabilityRegisters {
    uint8_t CAPLENGTH;
    uint8_t Rsvd;
    uint16_t HCIVERSION;

    union HCSPARAMS1 {
        uint32_t data;
        struct {
            uint32_t MaxSlots: 8;
            uint32_t MaxIntrs: 11;
            uint32_t         : 5;
            uint32_t MaxPorts: 8;
        } __attribute__((packed)) bits;
    } __attribute__((packed));

    union HCSPARAMS2 {
        uint32_t data;
        struct {
            uint32_t                 IST: 4;
            uint32_t             ERSTMax: 4;
            uint32_t                    : 13;
            uint32_t MaxScratchpadBufsHi: 5;
            uint32_t                 SPR: 1;
            uint32_t MaxScratchpadBufsLo: 5;
        } __attribute__((packed)) bits;
    } __attribute__((packed));

    union HCSPARAMS3 {
        uint32_t data;
        struct {
            uint32_t U1DeviceExitLatency: 8;
            uint32_t                    : 8;
            uint32_t U2DeviceExitLatency: 16;
        } __attribute__((packed)) bits;
    } __attribute__((packed));

    union HCCPARAMS1 {
        uint32_t data;
        struct {
            uint32_t       AC64: 1;
            uint32_t        BNC: 1;
            uint32_t        CSZ: 1;
            uint32_t        PPC: 1;
            uint32_t       PIND: 1;
            uint32_t       LHRC: 1;
            uint32_t        LTC: 1;
            uint32_t        NSS: 1;
            uint32_t        PAE: 1;
            uint32_t        SPC: 1;
            uint32_t        SEC: 1;
            uint32_t        CFC: 1;
            uint32_t MaxPSASize: 4;
            uint32_t       xECP: 16;
        } __attribute__((packed)) bits;
    } __attribute__((packed));

    uint32_t DBOFF;
    uint32_t RTSOFF;

    union HCCPARAMS2 {
        uint32_t data;
        struct {
            uint32_t     U3C: 1;
            uint32_t     CMC: 1;
            uint32_t     FSC: 1;
            uint32_t     CTC: 1;
            uint32_t     LEC: 1;
            uint32_t     CIC: 1;
            uint32_t     ETC: 1;
            uint32_t ETC_TSC: 1;
            uint32_t     GSC: 1;
            uint32_t     VTD: 1;
            uint32_t        : 22;
        } __attribute__((packed)) bits;
    } __attribute__((packed));
} __attribute__((packed));

union USBCMDMap {
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

union USBSTSMap {
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

union CRCRMap {
    uint64_t data;
    struct {
        uint64_t RCS: 1;
        uint64_t  CS: 1;
        uint64_t  CA: 1;
        uint64_t CRR: 1;
        uint64_t    : 2;
        uint64_t CRP: 58;
    } __attribute__((packed)) bits;
} __attribute__((packed));

union CONFIGMap {
    uint32_t data;
    struct {
        uint32_t MaxSlotsEn: 8;
        uint32_t        U3E: 1;
        uint32_t        CIE: 1;
        uint32_t           : 22;
    } __attribute__((packed)) bits;
} __attribute__((packed));

struct OperationalRegisters {
    USBCMDMap USBCMD;
    USBSTSMap USBSTS;
    uint32_t PAGESIZE;
    uint64_t Rsvd1;
    uint32_t DNCTRL;
    CRCRMap CRCR;
    uint64_t Rsvd2[2];
    uint64_t DCBAAP;
    CONFIGMap CONFIG;
} __attribute__((packed));