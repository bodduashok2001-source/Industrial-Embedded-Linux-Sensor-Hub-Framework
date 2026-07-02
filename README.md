# Industrial Embedded Linux Sensor Hub Framework

## Overview

The project demonstrates production-style Linux device driver development covering the Linux Device Model, Platform Drivers, Character Device Drivers, sysfs, procfs, IOCTL interfaces, Ring Buffers, Producer–Consumer architecture, Kernel Threads, Wait Queues, poll(), Interrupt Handling (Top Half/Bottom Half), Workqueues, Synchronization primitives and event-driven driver architecture.

---

## Features Implemented

### Character Device Driver

* Dynamic device registration
* Device node creation
* File operations implementation
* User-space communication

### IOCTL Interface

* Runtime driver configuration
* Mode based data processing
* User-kernel control path

Supported Modes:

* Normal Mode
* Uppercase Mode
* Reverse Mode

### Ring Buffer Framework

* Circular buffer implementation
* FIFO event storage
* Head/Tail management
* Buffer full and buffer empty handling

### Kernel Thread Framework

* Virtual sensor event generation
* Background processing
* Periodic sensor simulation

### Producer Consumer Architecture

Producer:

* Kernel generated sensor events

Consumer:

* User-space applications

### Wait Queue Support

* Blocking read implementation
* Sleep and wakeup mechanism
* Event driven notification

### poll() Interface

* POLLIN event support
* Readiness notification
* Event driven user-space application

### Interrupt Simulation

* Software generated interrupt framework
* Simulated hardware interrupt source
* Interrupt execution path validation

### Top Half and Bottom Half Architecture

Top Half:

* Interrupt Service Routine (ISR)

Bottom Half:

* Workqueue processing

### Workqueue Framework

* Deferred event processing
* ISR workload reduction
* Bottom-half execution model

### sysfs Framework

* Runtime driver configuration through sysfs
* Device attributes for driver state monitoring
* Read/Write kernel parameters from user space
* Attribute management using DEVICE_ATTR()

### procfs Framework

* Runtime driver statistics
* Driver diagnostic interface
* Multi-parameter status reporting
* seq_file based implementation

### Platform Driver Framework

* Platform Device and Platform Driver implementation
* Device lifecycle management using probe() and remove()
* Driver initialization through Linux device model
* Simulated platform device for development environment

### Synchronization

* Mutex protected ring buffer
* Race condition prevention
* Shared resource protection

### User Space Applications

#### user_test.c

* Driver validation utility
* IOCTL testing
* Data processing verification

#### sensor_reader.c

* Blocking read based sensor consumer

#### poll_reader.c

* poll() based event driven consumer

---

## Current Architecture

Platform Device
↓
Platform Driver
↓
probe()
↓
Character Device Driver
├───────────────┬───────────────┬
│               │               │
│               │               │
/dev         /sysfs         /procfs
│               │               │
│               │               │
IOCTL        Driver Config   Driver Statistics
│
Kernel Thread
↓
Fake Interrupt Generation
↓
ISR (Top Half)
↓
schedule_work()
↓
Workqueue (Bottom Half)
↓
Ring Buffer
↓
Wait Queue
↓
poll()
↓
User Space Applications

---

## Linux Kernel Concepts Covered

* Linux Kernel Modules (LKM)
* Character Device Drivers
* IOCTL Interfaces
* Ring Buffers
* Producer Consumer Architecture
* Kernel Threads
* Wait Queues
* Blocking I/O
* poll()
* Interrupt Handling
* Top Half / Bottom Half
* Workqueues
* Platform Device
* Platform Driver
* Linux Device Model
* probe()/remove() Lifecycle
* Mutex Synchronization
* sysfs
* procfs
* seq_file Interface
* DEVICE_ATTR()
* User Space ↔ Kernel Space Communication

---

## Technologies Used

* C Programming
* Linux Kernel Modules
* GCC
* Makefile
* Ubuntu Linux
* Git
* Vagrant
* VirtualBox

---

## Planned Roadmap

### Phase 2

* Device Tree
* GPIO Driver Integration
* I2C Driver Integration
* SPI Driver Integration
* Multi-Sensor Framework

### Phase 3

* DebugFS
* Kernel Timer
* kfifo Integration
* Power Management (Suspend/Resume)
* MMIO Register Framework
* DMA Simulation Framework
* Driver Debugging Infrastructure

---

## Learning Outcomes

* Linux Kernel Development
* Linux Driver Architecture
* Event Driven Systems
* Synchronization Techniques
* Interrupt Handling
* Bottom Half Processing
* Embedded Linux Internals
* Linux Device Model
* Platform Driver Framework
* Driver Lifecycle Management
* Production Style Driver Development