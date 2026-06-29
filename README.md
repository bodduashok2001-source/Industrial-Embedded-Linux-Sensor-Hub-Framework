# Industrial Embedded Linux Sensor Hub Framework

## Overview

Industrial Embedded Linux Sensor Hub Framework is a production-oriented Linux kernel development project designed to simulate a real embedded sensor subsystem commonly found in Qualcomm, AMD, Nvidia, Intel and Embedded Linux platforms.

The project demonstrates end-to-end Linux device driver development concepts including Platform Drivers, Character Device Drivers, IOCTL interfaces, Ring Buffers, Producer–Consumer architecture, Kernel Threads, Wait Queues, poll(), Interrupt Handling (Top Half/Bottom Half), Workqueues, Synchronization primitives and event-driven driver architectures.

The objective is to move beyond basic Linux character driver examples and build a scalable sensor framework that closely resembles real-world embedded Linux driver architectures.

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
↓
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

* Device Tree Integration
* sysfs Interface
* procfs Interface
* DebugFS
* Kernel Timer
* kfifo Integration

### Phase 3

* GPIO Driver Integration
* I2C Driver Integration
* SPI Driver Integration
* Multi-Sensor Framework

### Phase 4

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