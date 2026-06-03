cmake_minimum_required(VERSION 3.9)

target_sources(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/demo_fixed_wing_vehicle.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/demo_hovering_vehicle.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/demo_tracked_vehicle.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/h4h_vehicle.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/demo_view.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/demo_gimbal.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/demo_location.cpp"

)
