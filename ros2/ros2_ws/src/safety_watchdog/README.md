# `safety_watchdog`

- **Package:** `<package_name>`
- **Executable:** `<executable_name>`
- **Status:** `Working / In progress / Experimental / Broken`
- **People:** `<name(s)>`






## How to run it

The simplest way to get this node running:

**prereqs**
* `<dependency / hardware / another node>`
* `<configuration steps>`

**Command**

```bash
<command>
```
most nodes can be run using `ros2 run <package_name> <executable_name>`







## How it works

This node is responsible for:
* Monitor the UART connection status between HLCS(Pi) and LLCS(STM32), triggering the emergency state if needed.
* Monitor the LLCS for emergency state messages and trigger emergency state on the HLCS if received.
* Monitor sensor parameters in the HLCS for out of bounds values and trigger emergency state on LLCS and HLCS if needed.

### HLCS to LLCS UART Connection Monitoring

* The LLCS (STM32) will publish its status code to: `<topic tbd>` and subscribes to: `<topic tbd>`
* The HLCS (Pi) will publish its status code to: `<topic tbd>` and subscribes to: `<topic tbd>` 

Both will publish status codes at 10Hz.

Status Codes:
* 0 - Setup
* 1 - Run
* 2 - Emergency 

In addtion the LLCS will include a error code in its message type to communicate to the HLCS the reason why it is entering emergency. This allows the HCLS to include the code in its logging.

LLCS Error Code:

`List TBD`

### Emergency State Triggering

The safety watchdog will trigger emergency mode in any of the following scenarios: 
* If the safety watchdog node has not received a status message from the LLCS after 1 second.
* If the safety watchdog receives a status code of 2 from the LLCS.
* If any HLCS sensor parameters are out of bounds.

The emergency state is a state in the finite state machine(FMS) node that will set all actuators to a state that will surface the glider as quickly as possible, trigger addtional logging, and any other measures that will increase the likely hood of the being able to recover the glider.

### HLCS Sensor Limits

The safety watchdog will monitor the following parameters, checking if they are within the listed limits.

`Table of limits TBD`

These limits should set directly in the node's .cpp file and should not be able to be overidden by any configuration file or launch parameter. This ensures that incorrect limits can not be accidentally set in user created configuration files. 




## Important parts

* Part (file / class / function) : what it does (short explanation)
* Part (file / class / function) : what it does (short explanation)
 





## What has been done

A lightweight record of meaningful progress. Only needed if node is `in progress` otherwise delete 

* thing that works 
* other thing that works

Focus on information that would help another team member understand the state of the node.






## To do

* `<next task>`
* `<bug to investigate>`
* `<cleanup / improvement>`
* `<idea that isn't currently important>`







## Known issues / gotchas

Things that might confuse the next person:

* `<weird behavior>`
* `<hardware limitation>`
* `<temporary workaround>`
* `<assumption the code currently makes>`

If something took you an hour to figure out, put it here so the next person doesn't have to.







## Testing

How have we tested this?

* [ ] Runs locally
* [ ] Runs on PI
* [ ] Integration tested
* [ ] Tested in water
* [ ] Tested on the glider






## Useful references

Anything that helped while working on this node:

* `<datasheet>`
* `<Paper>`
* `<documentation>`
* `<related node>`
* `<issue / design document>`






## Scratchpad / random stuff

This section is intentionally unstructured.


* `<random useful thing>`
* `<questions>`
* `<idea to try later>`