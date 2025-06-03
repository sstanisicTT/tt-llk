# SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
# SPDX-License-Identifier: Apache-2.0

from helpers.device import (
    run_elf_files,
    wait_for_tensix_operations_finished,
)
from helpers.format_config import DataFormat
from helpers.param_config import InputOutputFormat
from helpers.profiler import ProfilerData, build_perf_test


def test_profiler_overhead():
   
    test_config = {
        "formats" : InputOutputFormat(DataFormat.Float16, DataFormat.Float16),
        "testname": "profiler_overhead_test",
    }

    profiler_meta = build_perf_test(test_config)
    if profiler_meta is not None:
        print(f"Profiler metadata: {profiler_meta}")
    
    run_elf_files("profiler_overhead_test")
    wait_for_tensix_operations_finished()

    runtime = ProfilerData.get(profiler_meta)

    print(runtime)

    print("DURATIONS:")
    for idx, x in enumerate(runtime["UNPACK"], 0):
        if x.full_marker["marker"] == "OVERHEAD":
            print(idx, x.duration)

    
    
