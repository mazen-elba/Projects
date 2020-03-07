#### Features

- **Multi-modal detection:** Multiple RGB-D & 2D laser detectors in one common framework.
- **People tracking:** Efficient tracker based upon nearest-neighbor data association.
- **Social relations:** Estimate spatial relations between people via coherent motion indicators.
- **Group tracking:** Detection and tracking of groups of people based upon their social relations.
- **Extensible and reusable:** ROS message types and clearly defined interfaces.
- **Powerful visualization:** A series of reusable RViz plugins that can be configured via mouse click.
- **Evaluation tools:** Metrics for evaluation of tracking performance.
- **ROS integration:** All components are fully integrated with ROS and written in C++ or Python.

#### Architecture

The entire communication between different stages of pipeline occurs via ROS messages. The modular architecture allows for easy interchangeability of individual components in all stages of the pipeline.

#### References

[1] Linder T. and Arras K.O. _Multi-Model Hypothesis Tracking of Groups of People in RGB-D Data._ IEEE Int. Conference on Information Fusion (FUSION'14), Salamanca, Spain, 2014.

[2] Jafari O. Hosseini and Mitzel D. and Leibe B.. _Real-Time RGB-D based People Detection and Tracking for Mobile Robots and Head-Worn Cameras_. IEEE International Conference on Robotics and Automation (ICRA'14), 2014.

[3] Arras K.O. and Martinez Mozos O. and Burgard W.. _Using Boosted Features for the Detection of People in 2D Range Data_. IEEE International Conference on Robotics and Automation (ICRA'07), Rome, Italy, 2007.

[4] Munaro M. and Menegatti E. _Fast RGB-D people tracking for service robots_. In Autonomous Robots, Volume 37 Issue 3, pp. 227-242, Springer, 2014.
