#!/bin/bash

OCD_INTERFACE=interface/cmsis-dap.cfg
OCD_PLATFORM=target/stm32f1x.cfg

openocd -f $OCD_INTERFACE -f $OCD_PLATFORM -c init -c targets -c halt -c "program $(find ./build -name '*.hex')" -c reset -c shutdown
