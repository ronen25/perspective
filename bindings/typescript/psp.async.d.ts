export = Perspective
export as namespace Perspective;

declare namespace Perspective {

  type WasmJSMethod = 'native-wasm';

  interface Module {
    wasmJSMethod?: WASMJSMethod,
    PSP_LOG_PROGRESS?: boolean,
    printErr?: Function,
    print?: Function,
    onRuntimeInitialized?: Function;
  }

  interface PerspectiveModule extends Module{
    t_pool: any;
    t_dtype: any;
    t_filter_op: any;
    t_header: any;
    t_aggtype: any;
    t_ctx_type: any;
    make_table: Function;
    make_gnode: Function;
    make_context_zero: Function;
    make_context_one: Function;
    make_context_two: Function;
    scalar_vec_to_val: Function;
    scalar_to_val: Function;
    table_add_computed_column: Function;
    sort: Function;
    fill: Function;
  }

  function loadPerspective(module: Module): PerspectiveModule;
};