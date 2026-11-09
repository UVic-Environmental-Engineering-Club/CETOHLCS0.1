# `Mission Plan Parser`

- **Package:** `mission_plan_parser`
- **Status:** `In progress`
- **People:** `Igor`




## How to run it

The simplest way to get this node running:

**prereqs**
* `colcon build`
* `source install/setup.bash`

**Command**

```bash
ros2 run mission_plan_parser mission_plan_parser
```
*Likely called by FSM node*







## How it works

1. Subscribes to `mission_parse_request` which is called by FSM
2. Parses the file in the topic
3. Verifies mission plan request for correctness and validity
4. Responds to FSM






## Important parts

* mission_parser : manages parsing the YAML file
* mission_planner : manages planning the mission and verifying total correctness
* waypoint : manages each individual waypoint and checks for correctness
 





## What has been done

Everything






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