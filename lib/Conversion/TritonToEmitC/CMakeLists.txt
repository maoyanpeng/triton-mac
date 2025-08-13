add_triton_library(TritonToEmitC
    arith_to_emitc_pass.cpp
    triton_to_emitc_pass.cpp

    DEPENDS
    TritonConversionEmitCPassIncGen

    LINK_LIBS PUBLIC
    MLIRIR
    MLIRPass
    MLIRArithDialect
    MLIREmitCDialect
    MLIRArithToEmitC
    TritonIR
    EmitCIR
)
