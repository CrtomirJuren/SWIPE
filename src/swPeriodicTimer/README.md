# Mbed Timers Library v1.0.0

The **Mbed Timers Library** allows you to measure the time between start and stop commands. The time can measured in milli or micro seconds.

## Installation

1. Downlaod from the Release site
2. Unzip
3. Move the folder to your mbed project folder 

## How to use

First, include the Timer library to your project:

```
#include "Timers.h"
```

## Example
Complete example. Here we created two timers, you can run it and get the result in the Serial monitor.

```cpp
#include "mbed.h"
#include "Timers.h"

Timers timer1;
Timers timer2(MICROS);

int main() {
	timer1.start();
	timer2.start();
 	if(timer1.state() == RUNNING) printf("timer 1 running\n");
	if(timer2.state() == RUNNING) printf("timer 2 running\n");
 	ThisThread::sleep_for(1s);
	timer1.pause();
	timer2.pause();
	if(timer1.state() == PAUSED) printf("timer 1 pause\n");
	if(timer2.state() == PAUSED) printf("timer 2 pause\n");
	printf("timer 1 elapsed ms: %u\n", timer1.elapsed());
	printf("timer 2 elapsed us: %u\n", timer2.elapsed());
	ThisThread::sleep_for(1s);
	timer1.resume();
	timer2.resume();
	ThisThread::sleep_for(1s);
 	timer1.stop();
	timer2.stop();
 	if(timer1.state() == STOPPED) printf("timer 1 stopped\n");
	if(timer2.state() == STOPPED) printf("timer 2 stopped\n");
 	printf("timer 1 elapsed ms: %u\n", timer1.elapsed());
	printf("timer 2 elapsed us: %u\n", timer2.elapsed());
	while (true) {
		}
	}
```

## Documentation

### Resolution

```cpp
enum resolution_t {
	MICROS,
	MILLIS
	};
```

You can use milli or micro seconds for the resolution, internal the library use the us_ticker_read() function. Standard is MILLIS;

### States

```cpp
enum status_t {
	STOPPED,
	RUNNING,
	PAUSED};
```
The library uses differnet states.

### Constructor

```cpp
Timers::Timers(resolution_t resolution)
```

Creates a Timer object

- **resolution** sets the internal resolution of the Timers, it can MICROS, or MILLIS, standard is MILLIS.

**Examples**

```cpp
Timers timer1;
Timers timer2(MICROS);
```

### Destructor

```cpp
Timers::~Timers()
```

Destructor for Timers objects.

**Example**

```cpp
Timers ~timer1;
```

## Class Functions

### Timers Start

```cpp
void Timers::start() 
```

Start the Timers. If it is paused, it will restarted the Timers.

**Example**

```cpp
timer1.start();
```

### Timers Pause

```cpp
void Timers::pause() 
```

Pause the Timers.

**Example**

```cpp
timer1.pause();
```

### Timers Resume

```cpp
void Timers::resume() 
```

Resume the Timers after a pause.

**Example**

```cpp
timer1.resume();
```

### Timers Stop

```cpp
void Timers::stop() 
```

Stops the Timers.

**Example**

```cpp
timer1.stop();
```

### Timers Elapsed Time

```cpp
uint32_t Timers::elapsed() 
```

Returns the time after start, you can read the elapsed time also while running.

**Example**

```cpp
uint32_t elapse;
elapse = timer1.elapsed();
```

### Timers get State

```cpp
status_t Timers::state()
```

Get the Timers state (RUNNING, PAUSED, STOPPED).

**Example**

```cpp
status_t status;
status = timer1.state();
```
