// MemoryFree library based on code posted here:
// https://forum.arduino.cc/index.php?topic=27536.msg204024#msg204024
// Extended by Matthew Murdoch to include walking of the free list.

#ifndef ARDUINO_PROFILER_H
#define ARDUINO_PROFILER_H

#ifdef __cplusplus
extern "C" {
#endif

class Profiler
{
    public:
        Profiler();
        int getFreeRAM();
        int getFreeBlock();
        int getRAMUsage();
        int getBlockUsage();

        int getInitRAM();
        int getInitBlock();
    private:
        int freeListSize();
        int _getBlockSize(uint16_t, uint16_t);

        int _initFreeRAM;
        int _initFreeBlock;
};

#ifdef  __cplusplus
}
#endif

#endif
