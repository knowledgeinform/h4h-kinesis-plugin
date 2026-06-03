# h4h-kinesis-plugin

## Name
h4h-kinesis-plugin 
## Description
APL developed plugin that deploys H4H-enabled vehicles in Kinesis for the RAD program.
## Installation
Current Workflow: 
1. This repo contains the kinesis-sdk needed to build the plugin. Open this repo and build in Android Studio.  
2. The resulting .apk file is manually copied and installed on the UCC grip.  
## Usage
Main files to make edits in: 
* On the plugin side (UI of plugin, including data fields going to Kinesis): 
  * `KinesisH4HActivity.java` - path: app/src/main/java/com/tomahawkrobotics/kinesis/plugins/salesdemo/KinesisH4HActivity.java 
  * `KinesisDemoService.kt` - path: app/src/main/java/com/tomahawkrobotics/kinesis/plugins/salesdemo/KinesisDemoService.kt
  * `h4h_activity.xml` - path: app/src/main/res/layout-land/h4h_activity.xml
  * `strings.xml` - path: app/src/main/res/layout-land/h4h_activity.xml 
* On the Kinesis side (control and status message processing): 
  * `h4h_nehicle.cpp` - path: app/src/main/cpp/src/agent/h4h_vehicle.cpp 
  * `h4h_vehicle.h` - path: app/src/main/cpp/include/agent/h4h_vehicle.h
* To add additional fields to send into Kinesis: 
  * `sdk_user_to_c.cpp` - path: app/src/main/cpp/jni/delegate/sdk_user_to_c.cpp 
  * `kinesis_demo_sdk_user.cpp` - path: app/src/main/cpp/src/sdk_user/kinesis_demo_sdk_user.cpp