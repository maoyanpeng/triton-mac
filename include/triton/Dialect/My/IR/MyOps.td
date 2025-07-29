#ifndef My_OPS
#define My_OPS

include "triton/Dialect/My/IR/MyDialect.td"
include "triton/Dialect/Triton/IR/TritonTypes.td"
include "mlir/IR/OpBase.td"
include "mlir/IR/SymbolInterfaces.td" // SymbolUserOpInterface
include "mlir/IR/OpAsmInterface.td" // OpAsmOpInterface
include "mlir/Interfaces/CallInterfaces.td" // CallOpInterface
include "mlir/Interfaces/CastInterfaces.td" // CastOpInterface
include "mlir/Interfaces/FunctionInterfaces.td" // FunctionOpInterface
include "mlir/Interfaces/SideEffectInterfaces.td" // Pure
include "mlir/Interfaces/ControlFlowInterfaces.td" // BranchOpInterface
include "mlir/Interfaces/InferTypeOpInterface.td" // SameOperandsAndResultType
include "mlir/Interfaces/SideEffectInterfaces.td" // Pure
include "mlir/Interfaces/CastInterfaces.td" // CastOpInterface
include "mlir/Interfaces/CallInterfaces.td" // CallOpInterface
include "mlir/IR/EnumAttr.td"

// 定义 Operation
def MyOp : Op<My_Dialect, "my_op"> {
  let summary = "My custom operation";
  
  // 参数：输入操作数(operands)和属性(attributes)
  let arguments = (ins
    F32Tensor:$input,       // 输入张量
    I32Attr:$threshold      // 整数属性
  );

  // 结果类型
  let results = (outs
    AnyTensor:$output       // 输出张量
  );

  // 自定义汇编格式(Assembly Format)
  let assemblyFormat = [{
    $input `,` $threshold attr-dict `:` type($input) `->` type($output)
  }];

  let extraClassDeclaration = [{
    void custom_func();
  }];
}


//===----------------------------------------------------------------------===//
// AddFOp
//===----------------------------------------------------------------------===//

def My_AddFOp : Op<My_Dialect, "addf"> {
  let summary = "Custom floating-point addition with custom printing";
  
  let arguments = (ins
    TT_FloatLike:$lhs,
    TT_FloatLike:$rhs
  );
  
  let results = (outs 
    TT_FloatLike:$result
  );

  // 类型约束：输入输出类型相同
  let extraClassDeclaration = [{
    static bool areCastCompatible(TypeRange inputs, TypeRange outputs) {
      return inputs.size() == 2 && outputs.size() == 1 &&
             inputs[0] == inputs[1] && inputs[0] == outputs[0];
    }
  }];

  // 自定义汇编格式
  let assemblyFormat = [{
    $lhs `,` $rhs attr-dict `:` `(` type($lhs) `,` type($rhs) `)` `->` type($result)
  }];

  // 构建方法
  // let builders = [
  //   OpBuilder<(ins "Value":$lhs, "Value":$rhs)>
  // ];
  
}


#endif // My_OPS