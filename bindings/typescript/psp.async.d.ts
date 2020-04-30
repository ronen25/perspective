export = Perspective
export as namespace Perspective;

declare namespace Perspective {

  type WasmJSMethod = 'native-wasm';

  interface ENV_LOGGING {
    PSP_LOG_TIME?: boolean,
    PSP_LOG_STORAGE_RESIZE?: boolean,
    PSP_LOG_SCHEMA_GNODE_FLATTENED?: boolean,
    PSP_LOG_DATA_POOL_SEND?: boolean,
    PSP_LOG_TIME_GNODE_PROCESS?: boolean,
    PSP_LOG_TIME_CTX_NOTIFY?: boolean,
    PSP_LOG_DATA_GNODE_FLATTENED?: boolean,
    PSP_LOG_DATA_GNODE_FLATTENED_MASK?: boolean,
    PSP_LOG_DATA_GNODE_DELTA?: boolean,
    PSP_LOG_DATA_GNODE_PREV?: boolean,
    PSP_LOG_DATA_GNODE_CURRENT?: boolean,
    PSP_LOG_DATA_GNODE_TRANSITIONS?: boolean,
    PSP_LOG_DATA_GNODE_EXISTED?: boolean,
    PSP_LOG_PROGRESS?: boolean,
    PSP_LOG_DATA_NSPARSE_STRANDS?: boolean,
    PSP_LOG_DATA_NSPARSE_STRAND_DELTAS?: boolean,
    PSP_LOG_DATA_NSPARSE_DTREE?: boolean,
    PSP_LOG_DATA_NSPARSE_DCTX?: boolean,
    PSP_LOG_DATA_NSPARSE_STREE_PREV?: boolean,
    PSP_LOG_DATA_NSPARSE_STREE_AFTER?: boolean,
  }

  interface Module {
    wasmJSMethod?: WASMJSMethod,
    LOGGING?: ENV_LOGGING,
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