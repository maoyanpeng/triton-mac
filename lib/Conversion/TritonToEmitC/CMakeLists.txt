add_triton_library(TritonToEmitC
    arith_to_emitc_pass.cpp

    LINK_LIBS PUBLIC
    MLIRIR
    MLIRPass
    MLIRArithDialect
    MLIREmitCDialect
    MLIRArithToEmitC
    TritonIR
    EmitCIR
)
