#include <stdio.h>
#include <string.h>
#include "intrin.h"

#ifndef __cplusplus
typedef int bool;
enum true_or_false{false,true};
#endif // __cplusplus

const char* szFeatures[] =
{
    "x87 FPU On Chip",
    "Virtual-8086 Mode Enhancement",
    "Debugging Extensions",
    "Page Size Extensions",
    "Time Stamp Counter",
    "RDMSR and WRMSR Support",
    "Physical Address Extensions",
    "Machine Check Exception",
    "CMPXCHG8B Instruction",
    "APIC On Chip",
    "Unknown1",
    "SYSENTER and SYSEXIT",
    "Memory Type Range Registers",
    "PTE Global Bit",
    "Machine Check Architecture",
    "Conditional Move/Compare Instruction",
    "Page Attribute Table",
    "Page Size Extension",
    "Processor Serial Number",
    "CFLUSH Extension",
    "Unknown2",
    "Debug Store",
    "Thermal Monitor and Clock Ctrl",
    "MMX Technology",
    "FXSAVE/FXRSTOR",
    "SSE Extensions",
    "SSE2 Extensions",
    "Self Snoop",
    "Hyper-threading Technology",
    "Thermal Monitor",
    "Unknown4",
    "Pend. Brk. EN."
};

int main(int argc, char* argv[])
{
    char CPUString[0x20];
    char CPUBrandString[0x40];
    int CPUInfo[4] = {-1};
    int nSteppingID = 0;
    int nModel = 0;
    int nFamily = 0;
    int nProcessorType = 0;
    int nExtendedmodel = 0;
    int nExtendedfamily = 0;
    int nBrandIndex = 0;
    int nCLFLUSHcachelinesize = 0;
    int nAPICPhysicalID = 0;
    int nFeatureInfo = 0;
    int nCacheLineSize = 0;
    int nL2Associativity = 0;
    int nCacheSizeK = 0;
    int nRet = 0;
    unsigned    nIds, nExIds, i;
    bool    bSSE3NewInstructions = false;
    bool    bMONITOR_MWAIT = false;
    bool    bCPLQualifiedDebugStore = false;
    bool    bThermalMonitor2 = false;


    // __cpuid with an InfoType argument of 0 returns the number of
    // valid Ids in CPUInfo[0] and the CPU identification string in
    // the other three array elements. The CPU identification string is
    // not in linear order. The code below arranges the information
    // in a human readable form.
    __cpuid(CPUInfo, 0);
    nIds = CPUInfo[0];
    memset(CPUString, 0, sizeof(CPUString));
    *((int*)CPUString) = CPUInfo[1];
    *((int*)(CPUString+4)) = CPUInfo[3];
    *((int*)(CPUString+8)) = CPUInfo[2];

    // Get the information associated with each valid Id
    for (i=0; i<=nIds; i++)
    {
        __cpuid(CPUInfo, i);


        //printf("\nFor InfoType %d\n", i);
        //printf("CPUInfo[0] = 0x%x\n", CPUInfo[0]);
        //printf("CPUInfo[1] = 0x%x\n", CPUInfo[1]);
        //printf("CPUInfo[2] = 0x%x\n", CPUInfo[2]);
        //printf("CPUInfo[3] = 0x%x\n", CPUInfo[3]);


        printf("\nFor InfoType %d\n", i);
        printf("CPUInfo[0] = 0x%x\n", CPUInfo[0]);
        printf("CPUInfo[1] = 0x%x\n", CPUInfo[1]);
        printf("CPUInfo[2] = 0x%x\n", CPUInfo[2]);
        printf("CPUInfo[3] = 0x%x\n", CPUInfo[3]);


        // Interpret CPU feature information.
        if  (i == 1)
        {
            nSteppingID = CPUInfo[0] & 0xf;
            nModel = (CPUInfo[0] >> 4) & 0xf;
            nFamily = (CPUInfo[0] >> 8) & 0xf;
            nProcessorType = (CPUInfo[0] >> 12) & 0x3;
            nExtendedmodel = (CPUInfo[0] >> 16) & 0xf;
            nExtendedfamily = (CPUInfo[0] >> 20) & 0xff;
            nBrandIndex = CPUInfo[1] & 0xff;
            nCLFLUSHcachelinesize = ((CPUInfo[1] >> 8) & 0xff) * 8;
            nAPICPhysicalID = (CPUInfo[1] >> 24) & 0xff;
            bSSE3NewInstructions = (CPUInfo[2] & 0x1) || false;
            bMONITOR_MWAIT = (CPUInfo[2] & 0x8) || false;
            bCPLQualifiedDebugStore = (CPUInfo[2] & 0x10) || false;
            bThermalMonitor2 = (CPUInfo[2] & 0x100) || false;
            nFeatureInfo = CPUInfo[3];
        }
    }

    // Calling __cpuid with 0x80000000 as the InfoType argument
    // gets the number of valid extended IDs.
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    memset(CPUBrandString, 0, sizeof(CPUBrandString));

    // Get the information associated with each extended ID.
    for (i=0x80000000; i<=nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        //printf("\nFor InfoType %x\n", i);
        //printf("CPUInfo[0] = 0x%x\n", CPUInfo[0]);
        //printf("CPUInfo[1] = 0x%x\n", CPUInfo[1]);
        //printf("CPUInfo[2] = 0x%x\n", CPUInfo[2]);
        //printf("CPUInfo[3] = 0x%x\n", CPUInfo[3]);


        printf("\nFor InfoType %x\n", i);
        printf("CPUInfo[0] = 0x%x\n", CPUInfo[0]);
        printf("CPUInfo[1] = 0x%x\n", CPUInfo[1]);
        printf("CPUInfo[2] = 0x%x\n", CPUInfo[2]);
        printf("CPUInfo[3] = 0x%x\n", CPUInfo[3]);

        // Interpret CPU brand string and cache information.
        if  (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
        else if  (i == 0x80000006)
        {
            nCacheLineSize = CPUInfo[2] & 0xff;
            nL2Associativity = (CPUInfo[2] >> 12) & 0xf;
            nCacheSizeK = (CPUInfo[2] >> 16) & 0xffff;
        }
    }

    // Display all the information in user-friendly format.

    //printf("\n\nCPU String: %s\n", CPUString);
    printf("\n\nCPU String: %s\n", CPUString);

    if  (nIds >= 1)
    {
        if  (nSteppingID)
            printf("Stepping ID = %d\n", nSteppingID);
        if  (nModel)
            printf("Model = %d\n", nModel);
        if  (nFamily)
            printf("Family = %d\n", nFamily);
        if  (nProcessorType)
            printf("Processor Type = %d\n", nProcessorType);
        if  (nExtendedmodel)
            printf("Extended model = %d\n", nExtendedmodel);
        if  (nExtendedfamily)
            printf("Extended family = %d\n", nExtendedfamily);
        if  (nBrandIndex)
            printf("Brand Index = %d\n", nBrandIndex);
        if  (nCLFLUSHcachelinesize)
            printf("CLFLUSH cache line size = %d\n",
                   nCLFLUSHcachelinesize);
        if  (nAPICPhysicalID)
            printf("APIC Physical ID = %d\n", nAPICPhysicalID);

        if  (nFeatureInfo || bSSE3NewInstructions ||
                bMONITOR_MWAIT || bCPLQualifiedDebugStore ||
                bThermalMonitor2)
        {
            printf("\nThe following features are supported:\n");

            if  (bSSE3NewInstructions)
                printf("\tSSE3 New Instructions\n");
            if  (bMONITOR_MWAIT)
                printf("\tMONITOR/MWAIT\n");
            if  (bCPLQualifiedDebugStore)
                printf("\tCPL Qualified Debug Store\n");
            if  (bThermalMonitor2)
                printf("\tThermal Monitor 2\n");

            i = 0;
            nIds = 1;
            while (i < (sizeof(szFeatures)/sizeof(const char*)))
            {
                if  (nFeatureInfo & nIds)
                {
                    printf("\t");
                    printf(szFeatures[i]);
                    printf("\n");
                }

                nIds <<= 1;
                ++i;
            }
        }
    }

    if  (nExIds >= 0x80000004)
        printf("\nCPU Brand String: %s\n", CPUBrandString);

    if  (nExIds >= 0x80000006)
    {
        printf("Cache Line Size = %d\n", nCacheLineSize);
        printf("L2 Associativity = %d\n", nL2Associativity);
        printf("Cache Size = %dK\n", nCacheSizeK);
    }
    getchar();
    return  nRet;
}
