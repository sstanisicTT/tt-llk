# SPDX-FileCopyrightText: © 2025 Tenstorrent AI ULC
# SPDX-License-Identifier: Apache-2.0

import math

import pytest
import torch
from helpers.chip_architecture import ChipArchitecture, get_chip_architecture
from helpers.device import (
    collect_results,
    run_elf_files,
    wait_for_tensix_operations_finished,
    write_stimuli_to_l1,
)
from helpers.format_arg_mapping import (
    ApproximationMode,
    DestAccumulation,
    MathOperation,
    format_dict,
)
from helpers.format_config import DataFormat
from helpers.param_config import (
    clean_params,
    generate_param_ids,
    generate_params,
    input_output_formats,
)
from helpers.stimuli_generator import generate_stimuli
from helpers.test_config import generate_make_command
from helpers.utils import compare_pcc, run_shell_command
from helpers.profiler import ProfilerData, build_perf_test

# SUPPORTED FORMATS FOR TEST
supported_formats = [DataFormat.Float16]

#   INPUT-OUTPUT FORMAT SWEEP
#   input_output_formats(supported_formats)

#   FULL FORMAT SWEEP
#   format_combination_sweep(formats=supported_formats, all_same=False, same_src_reg_format=True)

#   SPECIFIC FORMAT COMBINATION
#   generate_combination(
#       [(DataFormat.Float16_b,  # index 0 is for unpack_A_src
#         DataFormat.Float16_b,  # index 1 is for unpack_A_dst
#         DataFormat.Float16_b,  # index 2 is for pack_src (if src registers have same formats)
#         DataFormat.Bfp8_b,  # index 3 is for pack_dst
#         DataFormat.Float16_b,  # index 4 is for math format)])

#   SPECIFIC INPUT-OUTPUT COMBINATION
#   [InputOutputFormat(DataFormat.Float16, DataFormat.Float32)]

test_formats = input_output_formats(supported_formats)
all_params = generate_params(
    ["eltwise_binary_fpu_perf"],
    test_formats
)
param_ids = generate_param_ids(all_params)


@pytest.mark.parametrize(
    "testname, formats",
    clean_params(all_params),
    ids=param_ids,
)
def test_eltwise_unary_sfpu(testname, formats):
   
    test_config = {
        "formats": formats,
        "testname": testname,
    }

    profiler_meta = build_perf_test(test_config)
    if profiler_meta is not None:
        print(f"Profiler metadata: {profiler_meta}")
    
    run_elf_files(testname)
    wait_for_tensix_operations_finished()

    profiler_runtime_data = ProfilerData.get(profiler_meta)
    print(f"Profiler runtime data: {profiler_runtime_data}")
    ProfilerData.dump_csv(profiler_runtime_data)
    
