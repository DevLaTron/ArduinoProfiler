# Profiler for Arduino

This is an Arduino Library for memory and performance profiling, and the detection of memory leaks.

IMPORTANT: Depending on the Compiler optimization level, you may not necessarily notice any change when testing this
because any variables you declare may be optimized out of the code. This can lead to varying results.

Usage example:

~~~cpp
#include <Profiler.h>

Profiler profiler;

bool state = LOW;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(57600);
    Serial.print(F("Memory usage at start: "));
    Serial.print(profiler.getInitRAM());          // Maximum Free RAM available initially
    Serial.print(F(", cont. Block: "));
    Serial.println(profiler.getInitBlock());        // Initially available max Block size
    digitalWrite(LED_BUILTIN, state);
}

void loop() {   
    // We toggle the LED to show something is happening 
    state = !state;
    digitalWrite(LED_BUILTIN, state);
    Serial.print(F("Memory currently free: "));
    Serial.print(profiler.getFreeRAM());         // Currently free RAM
    Serial.print(F(", cont. Block: "));
    Serial.println(profiler.getFreeBlock());     // Currently max continous Block   

    // Add different variables here to get some idea on how the values change!
    
    // Let's create a "fake" memory leak on purpose by allocating memory and then throwing away the
    // pointer in the next loop. This consumes 102 Bytes of memory. After a while the Arduino can 
    // randomly crash on us, too, because the code is obviously broken.
    
    byte* ptr = (byte*)malloc(100);
    // Assign some stuff so the compiler optimization doesn't get smart on us and optimizes this away
    ptr[0] = 5;
    ptr[1] = 5;
       
    delay(1000);
}
~~~
