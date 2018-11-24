
import {
  Table, Vector, visitor, type, enum_
} from "apache-arrow";

import {
  loadPerspective
} from "./psp.async";

const Module = loadPerspective({
  wasmJSMethod: "native-wasm",
  PSP_LOG_PROGRESS: false,
  printErr: (x: any) => console.error(x),
  print: (x: any) => console.log(x)
});


/**
 * A lightweight wrapper around the perspective web assembly.
 *
 */
class WorkerHost {
  /**
   * Construct a new worker.
   *
   */
  constructor(scope: DedicatedWorkerGlobalScope) {
    this._scope = scope;

    let queued_listener: EventListener = (ev: MessageEvent) => {
      this._queued_messages.push(ev.data);
    };

    this._scope.addEventListener("message", queued_listener, false);

    Module.onRuntimeInitialized = () => {
      this._pool = new Module.t_pool({
        _update_callback: () => this._poolUpdated()
      });

      // Start updating pool
      this._intervalID = setInterval(() => this._pool.process(), this._timeout);

      // Process currently cached messages
      while (this._queued_messages.length > 0) {
        let msg = this._queued_messages.shift();
        this._processMessage(msg);
      }

      this._scope.removeEventListener("message", queued_listener);
      this._scope.addEventListener("message", (ev: MessageEvent) => { this._processMessage(ev.data); }, false);
    }
  }

  shutdown() {
    // Worker has been asked to shutdown
    clearInterval(this._intervalID);
    this.postMessage({ cmd: "shutting_down" });
    this._scope.close();
  }

  postMessage(msg: any) {
    this._scope.postMessage(msg);
  }

  private _processMessage(msg: any) {
    switch (msg.cmd) {
      case 'create-table': {
        let cfg = msg.data as any;
        let { names, types, index } = cfg;
        let name = cfg.name as Private.TableName;

        let tbl;
        try {
          tbl = Module.make_table(0, names, Private.mapTypes(types), [], 0, 0, index, false, false);

          cfg.gnode_id = this._pool.register_gnode(Module.make_gnode(tbl));

          this._table_map.set(name, cfg);
        } catch (e) {
          // Signal error to client
          console.error(e);
        } finally {
          if (tbl) tbl.delete();
        }
        break;
      }
      case 'delete-table': {
        let config = msg.data as any;
        let name = config.name as Private.TableName;
        let table = this._table_map.get(name) as any;
        this._table_map.delete(name);

        let gnode = this._pool.get_gnode(table.gnode_id);
        this._pool.unregister_gnode(gnode.get_id());
        gnode.delete();
        break;
      }
      case 'update': {
        let { name, records } = msg.data as any;
        let cfg = this._table_map.get(name) as any;
        let port = 0;

        if (cfg === undefined) {
          console.error("Table doesn't exist");
          break;
        }

        let nrecords = 0;
        let isArrow = false;
        let names = cfg.names;
        let cdata: Array<any>;
        let types = Private.mapTypes(cfg.types);

        if (records instanceof ArrayBuffer) {
          let pdata = Arrow.loadArrowBuffer(records);
          nrecords = pdata.nrecords;
          names = pdata.names;
          types = pdata.types;
          cdata = pdata.cdata;
          isArrow = true;
        } else {
          // Convert records to arrays of arrays
          cdata = [];
          for (let i = 0; i < names.length; ++i) {
            cdata.push([]);
          }
          for (let i = 0; i < records.length; ++i) {
            let record = records[i] as { [key: string]: any};
            for (let j = 0; j < names.length; ++j) {
              let name = names[j] as string;
              cdata[j].push(record[name]);
            }
          }
          nrecords = records.length;
        }

        let offset = 0;
        let limit = 1e9;
        let is_delete = false;
        if (cfg.index === '') {
          let gnode = this._pool.get_gnode(cfg.gnode_id);
          offset = gnode.get_table().size(); // + gnode.get_input_table(port).size();
        }

        let tbl;
        try {
          tbl = Module.make_table(nrecords, names, types,
            cdata, offset, limit, cfg.index, isArrow, is_delete);

          this._pool.send(cfg.gnode_id, port, tbl);
        } catch (e) {
          // Signal error to client
          console.error(e);
        } finally {
          if (tbl) tbl.delete();
        }
        break;
      }
      case 'create-view': {
        let config = msg.data as any;
        let name = config.name as Private.ViewName;
        let table = this._table_map.get(config.table_name) as any;
        let gnode = this._pool.get_gnode(table.gnode_id);

        let schema = gnode.get_tblschema();
        let { context, context_type } = Private.generateContext(config, schema);
        schema.delete();

        this._pool.register_context(gnode.get_id(), name, context_type, context.$$.ptr);
        this._context_map.set(name, context);
        break;
      }
      case 'delete-view': {
        let config = msg.data as any;
        let name = config.name as Private.ViewName;
        let table = this._table_map.get(config.table_name) as any;
        let gnode = this._pool.get_gnode(table.gnode_id);

        this._pool.unregister_context(gnode.get_id(), name);
        let context = this._context_map.get(name);
        this._context_map.delete(name);
        context.delete();
        break;
      }
      case 'view-snapshot': {
        let { name } = msg.data as any;
        let context = this._context_map.get(name as Private.ViewName);
        this._sendSnapshot(name, context);
        break;
      }
      case 'set-view-depth': {
        let { name, depth, isColumn } = msg.data as any;
        let context = this._context_map.get(name as Private.ViewName);
        if (context.sidedness() === 1) {
          context.set_depth(depth);
        } else {
          if (isColumn) {
            context.set_depth(Module.t_header.HEADER_COLUMN, depth);
          } else {
            context.set_depth(Module.t_header.HEADER_ROW, depth);
          }
        }
        this._sendSnapshot(name, context);
        break;
      }
      case "shutdown": {
        this.shutdown();
        break;
      }
    }
  }

  private _sendSnapshot(name: Private.ViewName, context: Private.PerspectiveContext) {
    this.postMessage({
      cmd: 'snapshot',
      name: name,
      snapshot: Private.generateFlatSnapshot(context)
    })
  }

  private _poolUpdated() {
    let updates = this._pool.get_contexts_last_updated();

    for (let i = 0; i < updates.size(); ++i) {
      let upd = updates.get(i);
      // Pull out the data and send to the client
      let context = this._context_map.get(upd.ctx_name);

      let rc = context.get_row_count();
      let delta = context.get_step_delta(0, rc);

      let count = 0; //delta.cells.size();
      if (!(delta.row_changed || delta.columns_changed) && count > 0) {
        let cells = [];
        let ccount = delta.cells.size();
        for (let j = 0; j < ccount; ++j) {
          let cell = delta.cells.get(j);
          cell.old_value = Module.scalar_to_val(cell.old_value);
          cell.new_value = Module.scalar_to_val(cell.new_value);
          cells.push(cell);
        }

        this.postMessage({
          cmd: "update",
          name: upd.ctx_name,
          updates: cells
        });
      } else {
        this._sendSnapshot(upd.ctx_name, context);
      }

      delta.cells.delete();
    }
    updates.delete();
  }

  private _scope: DedicatedWorkerGlobalScope;

  private _queued_messages: Array<any> = [];

  private _pool: any;
  private _timeout: number = 500;
  private _intervalID: any;

  private _table_map: Map<Private.TableName, object> = new Map();
  private _context_map: Map<Private.ViewName, Private.PerspectiveContext> = new Map();
}


const scope: DedicatedWorkerGlobalScope = self as any;
if (typeof self !== "undefined" && self.addEventListener) {
  new WorkerHost(scope);
}

namespace Private {

  export type TableName = string;
  export type ViewName = string;

  export type PerspectiveContext = any;

  export
    function mapTypes(types: string[]): any[] {
    let dtypes = types.map((type: string): any => {
      switch (type) {
        case "integer":
          return Module.t_dtype.DTYPE_INT64;
        case "float":
          return Module.t_dtype.DTYPE_FLOAT64;
        case "boolean":
          return Module.t_dtype.DTYPE_BOOL;
        case "date":
          return Module.t_dtype.DTYPE_DATE;
        case "datetime":
          return Module.t_dtype.DTYPE_TIME;
        case "string":
        default:
          return Module.t_dtype.DTYPE_STR;
      }
    });
    return dtypes;
  }

  const sortOrders = ["asc", "desc", "none", "asc abs", "desc abs"];

  function mapFilterOps(op: string): any {
    switch (op) {
      case "&": return Module.t_filter_op.FILTER_OP_AND;
      case "|": return Module.t_filter_op.FILTER_OP_OR;
      case "<": return Module.t_filter_op.FILTER_OP_LT;
      case ">": return Module.t_filter_op.FILTER_OP_GT;
      case "==": return Module.t_filter_op.FILTER_OP_EQ;
      case "contains": return Module.t_filter_op.FILTER_OP_CONTAINS;
      case "<=": return Module.t_filter_op.FILTER_OP_LTEQ;
      case ">=": return Module.t_filter_op.FILTER_OP_GTEQ;
      case "!=": return Module.t_filter_op.FILTER_OP_NE;
      case "begins with": return Module.t_filter_op.FILTER_OP_BEGINS_WITH;
      case "ends with": return Module.t_filter_op.FILTER_OP_ENDS_WITH;
      case "or": return Module.t_filter_op.FILTER_OP_OR;
      case "in": return Module.t_filter_op.FILTER_OP_IN;
      case "not in": return Module.t_filter_op.FILTER_OP_NOT_IN;
      case "and": return Module.t_filter_op.FILTER_OP_AND;
      case "is nan": return Module.t_filter_op.FILTER_OP_IS_NAN;
      case "is not nan": return Module.t_filter_op.FILTER_OP_IS_NOT_NA;
    }
  }

  function mapAggName(agg: string): any {
    switch (agg) {
      case "distinct": return Module.t_aggtype.AGGTYPE_DISTINCT_COUNT;
      case "sum": return Module.t_aggtype.AGGTYPE_SUM;
      case "mul": return Module.t_aggtype.AGGTYPE_MUL;
      case "avg": return Module.t_aggtype.AGGTYPE_MEAN;
      case "mean": return Module.t_aggtype.AGGTYPE_MEAN;
      case "count": return Module.t_aggtype.AGGTYPE_COUNT;
      case "weighted mean": return Module.t_aggtype.AGGTYPE_WEIGHTED_MEAN;
      case "unique": return Module.t_aggtype.AGGTYPE_UNIQUE;
      case "any": return Module.t_aggtype.AGGTYPE_ANY;
      case "median": return Module.t_aggtype.AGGTYPE_MEDIAN;
      case "join": return Module.t_aggtype.AGGTYPE_JOIN;
      case "div": return Module.t_aggtype.AGGTYPE_SCALED_DIV;
      case "add": return Module.t_aggtype.AGGTYPE_SCALED_ADD;
      case "dominant": return Module.t_aggtype.AGGTYPE_DOMINANT;
      case "first by index": return Module.t_aggtype.AGGTYPE_FIRST;
      case "last by index": return Module.t_aggtype.AGGTYPE_LAST;
      case "and": return Module.t_aggtype.AGGTYPE_AND;
      case "or": return Module.t_aggtype.AGGTYPE_OR;
      case "last": return Module.t_aggtype.AGGTYPE_LAST_VALUE;
      case "high": return Module.t_aggtype.AGGTYPE_HIGH_WATER_MARK;
      case "low": return Module.t_aggtype.AGGTYPE_LOW_WATER_MARK;
      case "sum abs": return Module.t_aggtype.AGGTYPE_SUM_ABS;
      case "sum not null": return Module.t_aggtype.AGGTYPE_SUM_NOT_NULL;
      case "mean by count": return Module.t_aggtype.AGGTYPE_MEAN_BY_COUNT;
      case "identity": return Module.t_aggtype.AGGTYPE_IDENTITY;
      case "distinct leaf": return Module.t_aggtype.AGGTYPE_DISTINCT_LEAF;
      case "pct sum parent": return Module.t_aggtype.AGGTYPE_PCT_SUM_PARENT;
      case "pct sum grand total": return Module.t_aggtype.AGGTYPE_PCT_SUM_GRAND_TOTAL;
    }
  }

  export
    function generateContext(config: any, schema: any)
    : { context: PerspectiveContext, context_type: any } {
    config = { ...config };

    config.row_pivot = config.row_pivot || [];
    config.column_pivot = config.column_pivot || [];

    // Filters
    let filters = [];
    let filter_op = Module.t_filter_op.FILTER_OP_AND;
    if (config.filter) {
      filters = config.filter.map((filter: [string, string, any]) => {
        return [filter[0], mapFilterOps(filter[1]), filter[2]];
      });
    }

    // Sort
    let sort_spec = [];
    if (config.sort) {
      sort_spec = config.sort.map((sort: [number, string]) => {
        return [sort[0], sortOrders.indexOf(sort[1])];
      });
    }

    // Aggregates
    let aggregates = [];
    if (config.aggregate) {
      for (let aidx = 0; aidx < config.aggregate.length; aidx++) {
        let agg = config.aggregate[aidx];
        let agg_op = mapAggName(agg.op);
        if (typeof agg.column === "string") {
          agg.column = [agg.column];
        } else {
          let dep_length = agg.column.length;
          if ((agg.op === "weighted mean" && dep_length != 2) || (agg.op !== "weighted mean" && dep_length != 1)) {
            throw `'${agg.op}' has incorrect arity ('${dep_length}') for column dependencies.`;
          }
        }
        let name = agg.name || (`${agg.op}(${agg.column.join('|')})`);
        aggregates.push([name, agg_op, agg.column]);
      }
    } else {
      let columns = schema.columns();
      for (let i = 0; i < columns.size(); ++i) {
        let column = columns.get(i);
        aggregates.push([column, Module.t_aggtype.AGGTYPE_ANY, [column]]);
      }
      columns.delete();
    }

    let context, type;
    if (config.row_pivot.length > 0 || config.column_pivot.length > 0) {
      if (config.column_pivot.length > 0) {
        type = Module.t_ctx_type.TWO_SIDED_CONTEXT;
        context = Module.make_context_two(schema, config.row_pivot, config.column_pivot, filter_op, filters, aggregates, sort_spec);

        context.set_depth(Module.t_header.HEADER_ROW, config.row_pivot.length);
        context.set_depth(Module.t_header.HEADER_COLUMN, config.column_pivot.length);
      } else {
        type = Module.t_ctx_type.ONE_SIDED_CONTEXT;
        context = Module.make_context_one(schema, config.row_pivot, filter_op, filters, aggregates, sort_spec);

        context.set_depth(config.row_pivot.length);
      }
    } else {
      type = Module.t_ctx_type.ZERO_SIDED_CONTEXT;
      context = Module.make_context_zero(schema, filter_op, filters, aggregates.map(agg => agg[2][0]), sort_spec);
    }

    return { context: context, context_type: type };
  }

  export
    function generateFlatSnapshot(context: any) {
    let start_row = 0;
    let start_col = 0;
    let schema: any = {};
    let header: Array<string> = [];
    let end_row, end_col, stride;

    let sides = context.sidedness();

    let columns = context.unity_get_column_names();
    let dtype_idx = (sides === 0) ? 0 : 1;

    for (let i = 0, end = columns.size(); i < end; ++i) {
      let column = columns.get(i);
      header.push(column);
      let type = context.get_column_dtype(i + dtype_idx).value;
      switch (type) {
        case 1:
        case 2:
          schema[column] = 'integer';
          break;
        case 19:
          schema[column] = 'string';
          break;
        case 9:
        case 10:
        case 17:
          schema[column] = 'float';
          break;
        case 11:
          schema[column] = 'boolean';
          break;
        case 12:
          schema[column] = 'datetime';
          break;
        case 13:
          schema[column] = 'date';
          break;
      }
    }

    columns.delete();

    let data: any[] = [];
    let row_spans: any[] = [];
    let col_spans: any[] = [];

    if (sides === 0) {
      end_col = context.unity_get_column_count();
      stride = end_col - start_col;

      end_row = context.unity_get_row_count();

      let slice = context.get_data(start_row, end_row, start_col, end_col);

      for (let r = start_row; r < end_row; r++) {
        let row = [];
        for (let c = start_col; c < end_col; c++) {
          row.push(Module.scalar_vec_to_val(slice, (r * stride) + c));
        }
        data.push(row);
      }
      slice.delete();
    } else {
      let row_depth = 0;
      let column_depth = 0;
      stride = 0;

      let slice;
      if (sides === 1) {
        row_depth = context.get_depth() + 1;
        end_col = context.unity_get_column_count() + row_depth;
        stride = end_col - start_col;
        end_row = context.get_leaf_count();
        slice = context.get_leaf_data(start_row, end_row, start_col, end_col);
      } else if (sides === 2) {
        row_depth = context.get_depth(Module.t_header.HEADER_ROW) + 1;
        end_col = context.unity_get_column_count() + row_depth;
        stride = end_col - start_col;
        column_depth = context.get_depth(Module.t_header.HEADER_COLUMN) + 1;
        end_row = context.get_leaf_count(Module.t_header.HEADER_ROW);
        slice = context.get_leaf_data(start_row, end_row, start_col, end_col);
      }

      let start = 0;
      if (column_depth > 0) {
        let unit_sep = String.fromCharCode(31);
        for (let d = 0; d < column_depth; d++) {
          col_spans.push([]);
        }
        for (let c = row_depth; c < stride; c++) {
          let val = Module.scalar_vec_to_val(slice, c);
          let span_reset = false;
          val = val.split(unit_sep);
          for (let i = 0; i < val.length; i++) {
            let span = col_spans[i];
            let curr = span[span.length - 1];
            if (span_reset || !curr || curr[0] !== val[i]) {
              curr = [val[i], 0];
              span.push(curr);
              span_reset = true;
            }
            curr[1] += 1;
          }
        }
        start += 1;
      }

      if (row_depth > 0) {
        for (let d = 0; d < row_depth; d++) {
          row_spans.push([]);
        }

        for (let r = start; r < end_row + start; r++) {
          let row = [];
          let header = [];
          for (let c = 0; c < stride; c++) {
            let val = Module.scalar_vec_to_val(slice, r * stride + c);

            if (c < row_depth - 1) {
              header.push(val);
            } else if (c === row_depth - 1) {
              header.push(val);

              let span_reset = false;
              for (let i = 0; i < header.length; i++) {
                let span = row_spans[i];
                let curr = span[span.length - 1];
                if (span_reset || !curr || curr[0] !== header[i]) {
                  curr = [header[i], 0];
                  span.push(curr);
                  span_reset = true;
                }
                curr[1] += 1;
              }
            } else {
              row.push(val);
            }
          }
          data.push(row);
        }

        let visit_layer = (depth: number, start: number, total: number): number => {
          let spans = row_spans[depth];
          let child_start = 0;
          let i = 0;
          for (let count = 0; (count < total) && (start + i < spans.length); ++i) {
            let span = spans[start + i];
            let [, c] = span;
            // If we aren't at the penultimate level, count our children
            if (depth < row_depth - 2) {
              let cc = visit_layer(depth + 1, child_start, c);
              child_start += cc;
              span.push(cc);
            } else {
              span.push(1);
            }
            count += c;
          }
          return i;
        }

        if (row_depth > 1) {
          visit_layer(0, 0, Number.POSITIVE_INFINITY);
        }
      }
      slice.delete();
    }

    return { header: header, row_spans: row_spans, col_spans: col_spans, data: data, schema: schema };
  }
}

namespace Arrow {
  /**
   * Converts arrow data into a canonical representation for
   * interfacing with perspective.
   *
   * @private
   * @param {object} data Array buffer
   * @returns An object with 3 properties:
   **/
  export
  function loadArrowBuffer(data: ArrayBuffer)
    : {nrecords: number, names: Array<any>, types: Array<any>, cdata: Array<Array<any>>} {
    // TODO Need to validate that the names/types passed in match those in the buffer
    let arrow = Table.from([new Uint8Array(data)]);
    let loader = arrow.schema.fields.reduce((loader: any, field: any, colIdx: any) => {
      return loader.loadColumn(field, arrow.getColumnAt(colIdx));
    }, new ArrowColumnLoader());

    return {
      nrecords: arrow.length,
      names: loader.names,
      types: loader.types,
      cdata: loader.cdata
    };

    /*if (typeof loader.cdata[0].values === "undefined") {
      let nchunks = loader.cdata[0].data.chunkVectors.length;
      let chunks = [];
      for (let x = 0; x < nchunks; x++) {
        chunks.push({
          row_count: loader.cdata[0].data.chunkVectors[x].length,
          is_arrow: true,
          names: loader.names,
          types: loader.types,
          cdata: loader.cdata.map(y => y.data.chunkVectors[x])
        });
      }
      return chunks;
    } else {
      return [
        {
          row_count: arrow.length,
          is_arrow: true,
          names: loader.names,
          types: loader.types,
          cdata: loader.cdata
        }
      ];
    }*/
  }

  class ArrowColumnLoader extends visitor.TypeVisitor {

    cdata: Array<any>;
    names: Array<any>;
    types: Array<any>;

    constructor() {
      super();
      this.cdata = [];
      this.names = [];
      this.types = [];
    }

    loadColumn(field: type.Field, column: Vector) {
      if (this.visit(field.type)) {
        this.cdata.push(column);
        this.names.push(field.name);
      }
      return this;
    }
    visitNull(type: type.Null) {}
    visitBool(type: type.Bool) {
      this.types.push(Module.t_dtype.DTYPE_BOOL);
      return true;
    }
    visitInt(type: type.Int) {
      const bitWidth = type.bitWidth;
      if (bitWidth === 64) {
        this.types.push(Module.t_dtype.DTYPE_INT64);
      } else if (bitWidth === 32) {
        this.types.push(Module.t_dtype.DTYPE_INT32);
      } else if (bitWidth === 16) {
        this.types.push(Module.t_dtype.DTYPE_INT16);
      } else if (bitWidth === 8) {
        this.types.push(Module.t_dtype.DTYPE_INT8);
      }
      return true;
    }
    visitFloat(type: type.Float) {
      const precision = type.precision;
      if (precision === enum_.Precision.DOUBLE) {
        this.types.push(Module.t_dtype.DTYPE_FLOAT64);
      } else if (precision === enum_.Precision.SINGLE) {
        this.types.push(Module.t_dtype.DTYPE_FLOAT32);
      }
      // todo?
      // else if (type.precision === Arrow.enum_.Precision.HALF) {
      //     this.types.push(Module.t_dtype.DTYPE_FLOAT16);
      // }
      return true;
    }
    visitUtf8(type: type.Utf8) {
      this.types.push(Module.t_dtype.DTYPE_STR);
      return true;
    }
    visitBinary(type: type.Binary) {
      this.types.push(Module.t_dtype.DTYPE_STR);
      return true;
    }
    visitFixedSizeBinary(type: type.FixedSizeBinary) {}
    visitDate(type: type.Date_) {
      this.types.push(Module.t_dtype.DTYPE_DATE);
      return true;
    }
    visitTimestamp(type: type.Timestamp) {
      this.types.push(Module.t_dtype.DTYPE_TIME);
      return true;
    }
    visitTime(type: type.Time) {}
    visitDecimal(type: type.Decimal) {}
    visitList(type: type.List) {}
    visitStruct(type: type.Struct) {}
    visitUnion(type: type.Union<any>) {}
    visitDictionary(type : type.Dictionary) {
      return this.visit(type.dictionary);
    }
    visitInterval(type: type.Interval) {}
    visitFixedSizeList(type: type.FixedSizeList) {}
    visitMap(type: type.Map_) {}
  }
}
