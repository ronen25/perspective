import WebpackWorker = require('worker-loader?name=psp-worker.js!./worker');
import {
  UUID
} from '@phosphor/coreutils';

export type TypeName = 'string' | 'float' | 'integer' | 'boolean' | 'date' | 'datetime';
export type Subscription = (data: FlatResult) => void;

export
const enum TypeNames {
  STRING   = 'string',
  FLOAT    = 'float',
  INTEGER  = 'integer',
  BOOLEAN  = 'boolean',
  DATE     = 'date',
  DATETIME = 'datetime'
};

export
type Schema = {
  [ key: string ]: TypeName;
}

export
type SortOrder = 'asc' | 'asc abs' | 'desc' | 'desc abs' | 'none';

export
const enum SortOrders {
  ASC = 'asc',
  ASC_ABS = 'asc abs',
  DESC = 'desc',
  DESC_ABS = 'desc abs',
  NONE = 'none',
}

export
type FlatResult = {
  header: Array<any>;
  row_spans: Array<Array<any>>;
  col_spans: Array<Array<any>>;
  data: Array<Array<any>>;
  schema: Schema;
}

export
type AggregateConfig = {
  /**
   * The list of table columns to use for this aggregate.
   */
  column: string | Array<string>;

  /**
   * The aggregate operation to perform.
   */
  op: string;

  /**
   * The optional name to give the aggregate. If not given this is determined
   * from the aggregate operation and list of dependent columns.
   */
  name?: string;
}

export
interface ViewConfig {
  /**
   * The list of columns for row pivoting.
   */
  row_pivot?: Array<string>;

  /**
   * The list of columns for column pivoting.
   */
  column_pivot?: Array<string>;

  /**
   * The list of aggregates.
   */
  aggregate?: Array<AggregateConfig>;

  /**
   * The sort specification.
   */
  sort?: Array<[number, SortOrders]>;

  /**
   * The filter specification.
   */
  filter?: Array<[string, string, any]>;
}

export
class Table {
  constructor(schema: Schema, options: {index: string}, engine: Engine) {
    this._name = UUID.uuid4();
    this._engine = engine;
    this._schema = schema;

    // generate expanded schema
    let names = Object.keys(schema);
    let types: Array<TypeName> = names.map( (name: string) => schema[name] );

    this._engine.postMessage({
      cmd: "create-table",
      data: {
        name: this._name,
        names: names,
        types: types,
        index: options.index,
      }
    });
  }

  delete() : void {
    this._engine.postMessage({
      cmd: 'delete-table',
      data: {
        name: this._name,
      }
    });
  }

  schema(): Promise<Schema> {
    return new Promise((resolve, reject) : void => {
      resolve(this._schema);
    });
  }

  update(records: any[] | ArrayBuffer) {
    let data = {
      name: this._name,
      records: records
    };

    if (this._engine.transferable && records instanceof ArrayBuffer) {
      this._engine.postMessage({
        cmd: "update",
        data: data
      }, [records]);
    } else {
      this._engine.postMessage({
        cmd: "update",
        data: data
      });
    }
  }

  view(config: ViewConfig) : View {
    return new View(this._name, config, this._engine);
  }

  add_computed(computed: any): Table {
    return this;
  }

  private _name: Private.TableName;
  private _engine: Engine;
  private _schema: Schema
}


export
class View {
  constructor(tableName: Private.TableName, config: ViewConfig, engine: Engine) {
    this._name = UUID.uuid4();
    this._tableName = tableName;
    this._engine = engine;

    this._engine.postMessage({
      cmd: "create-view",
      data: {
        name: this._name,
        table_name: tableName,
        ...config
      }
    });
  }

  subscribe(callback: Subscription) : void {
    this._engine.addViewSubscription(this._name, callback);
  }

  set_depth(depth: number, isColumn: boolean) : void {
    this._engine.postMessage({
      cmd: 'set-view-depth',
      data: {
        name: this._name,
        depth: depth,
        isColumn: isColumn
      }
    });
  }

  delete() : void {
    this._engine.postMessage({
      cmd: 'delete-view',
      data: {
        name: this._name,
        table_name: this._tableName
      }
    });
    this._engine.removeViewSubscriptions(this._name);
  }

  to_flat() : Promise<FlatResult> {
    return new Promise( (resolve, reject) : void => {
      //resolve();
    });
  }

  private _name: Private.ViewName;
  private _tableName: Private.TableName;
  private _engine: Engine;
}


export
class Engine {

  private _worker: Worker;
  private _subscriptions: Map<Private.ViewName, Array<Subscription>>;

  readonly transferable: boolean;

  constructor() {

    let worker = (this._worker = new WebpackWorker());
    worker.addEventListener('message', (event: MessageEvent) => { this._onWorkerMessage(event); });


    // Detect whether we support transferables
    let ab = new ArrayBuffer(1);
    worker.postMessage(ab, [ab]);
    this.transferable = ab.byteLength === 0;

    if (!this.transferable) {
      console.warn("Transferable support not detected");
    } else {
      console.log("Transferable support detected");
    }

    this._subscriptions = new Map();
  }

  postMessage(msg: any, transfer?: Array<ArrayBuffer>) {
    this._worker.postMessage(msg, transfer);
  }

  table(schema: Schema, options: {index: string}): Table {
    return new Table(schema, options, this);
  }

  addViewSubscription(name: Private.ViewName, subscription: Subscription) : void {
    if (!this._subscriptions.has(name)) {
      this._subscriptions.set(name, new Array());
    }

    let subscriptions = this._subscriptions.get(name);
    subscriptions!.push(subscription);
    this.postMessage({
      cmd: 'view-snapshot',
      data: {
        name: name
      }
    });
  }

  removeViewSubscriptions(name: Private.ViewName): void {
    this._subscriptions.delete(name);
  }

  private _onWorkerMessage(event: MessageEvent) {
    let msg = event.data;

    let name = msg.name;
    let subscriptions = this._subscriptions.get(name);
    if (subscriptions) {
      for (let callback of subscriptions) {
        callback(msg.snapshot);
      }
    }
  }
}

namespace Private {
  export type TableName = string;
  export type ViewName = string;
}
