/* Basic usage example with good practices for Profiling an Arduino Sketch
   It blinks the Onboard LED, but also has a fake memory leak so you can see how available memory
   is leaking.
*/

// Include and initialize Profiler
#include <Profiler.h>
Profiler profiler;

// Toggle this to use profiler or keep it out of compile for production
#define DEBUG_PROFILER true

bool state = LOW;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, state);
    
    Serial.begin(57600);

    // We only want to compile this in conditionally. No reason the keep it when in production!
    #if DEBUG_PROFILER
        Serial.print(F("Memory usage at start: "));
        Serial.print(profiler.getInitRAM());          // Maximum Free RAM available initially
        Serial.print(F(", cont. Block: "));
        Serial.println(profiler.getInitBlock());        // Initially available max Block size
    #endif

    digitalWrite(LED_BUILTIN, state);
}

void loop() {   
    // We toggle the LED to show something is happening 
    state = !state;
    digitalWrite(LED_BUILTIN, state);

    #if DEBUG_PROFILER
        Serial.print(F("Memory currently free: "));
        Serial.print(profiler.getFreeRAM());         // Currently free RAM
        Serial.print(F(", cont. Block: "));
        Serial.println(profiler.getFreeBlock());     // Currently max continous Block   
    #endif
    
    // Let's create a "fake" memory leak on purpose by allocating memory and then throwing away the
    // pointer in the next loop. This consumes 102 Bytes of memory. After a while the Arduino can 
    // randomly crash on us, too, because the code is obviously broken.
    byte* ptr = (byte*)malloc(100);
    
    // Assign some stuff so the compiler optimization doesn't get smart on us and optimizes this away.
    ptr[0] = 5;
    ptr[1] = 5;
    
    Serial.println(F("Going to sleep for a second!"));  
    delay(1000);
}
