// typings/custom.d.ts
declare module "worker-loader?name=psp-worker.js!*" {
    class WebpackWorker extends Worker {
      constructor();
    }
  
    export = WebpackWorker;
  }