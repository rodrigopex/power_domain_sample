# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(EXTRA_ZEPHYR_MODULES $ENV{ZEPHYR_BASE}/../sample_device)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

message(WARNING " === $ENV{ZEPHYR_BASE}/../sample_device")


target_sources(app PRIVATE
        src/main.c
        src/pm_notifier.c
)
