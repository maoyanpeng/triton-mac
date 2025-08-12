#ifndef EMITC_OPS
#define EMITC_OPS

include "mlir/Dialect/EmitC/IR/EmitCAttributes.td"
include "mlir/Dialect/EmitC/IR/EmitCTypes.td"

include "mlir/Interfaces/CallInterfaces.td"
include "mlir/Interfaces/CastInterfaces.td"
include "mlir/Interfaces/ControlFlowInterfaces.td"
include "mlir/Interfaces/FunctionInterfaces.td"
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/IR/RegionKindInterface.td"


//===----------------------------------------------------------------------===//
// EmitC op definitions
//===----------------------------------------------------------------------===//

// Base class for EmitC dialect ops.
class EmitC_Op<string mnemonic, list<Trait> traits = []>
    : Op<EmitC_Dialect, mnemonic, traits>;

// Base class for unary operations.
class EmitC_UnaryOp<string mnemonic, list<Trait> traits = []> :
    EmitC_Op<mnemonic, traits> {
  let arguments = (ins EmitCType);
  let results = (outs EmitCType);
  let assemblyFormat = "operands attr-dict `:` functional-type(operands, results)";
}

// Base class for binary operations.
class EmitC_BinaryOp<string mnemonic, list<Trait> traits = []> :
    EmitC_Op<mnemonic, traits> {
  let arguments = (ins EmitCType:$lhs, EmitCType:$rhs);
  let results = (outs EmitCType);
  let assemblyFormat = "operands attr-dict `:` functional-type(operands, results)";
}

// EmitC OpTrait
def CExpression : NativeOpTrait<"emitc::CExpression">;

// Types only used in binary arithmetic operations.
def IntegerIndexOrOpaqueType : AnyTypeOf<[EmitCIntegerType, Index, EmitC_OpaqueType]>;
def FloatIntegerIndexOrOpaqueType : AnyTypeOf<[EmitCFloatType, IntegerIndexOrOpaqueType]>;


def EmitC_MinSIOp : EmitC_BinaryOp<"minsi", [CExpression, SameOperandsAndResultShape]> {
  let summary = "Min operation";
  let description = [{
    Example:

    ```mlir
    // Custom form of the addition operation.
    %0 = emitc.minsi %arg0, %arg1 : (i32, i32) -> i32
    %1 = emitc.minsi %arg2, %arg3 : (!emitc.ptr<f32>, i32) -> !emitc.ptr<f32>
    ```
    ```c++
    // Code emitted for the operations above.
    int32_t v5 = min(v1, v2);
    float* v6 = min(v3, v4);
    ```
  }];
}


#endif // EMITC_OPS