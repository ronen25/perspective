
const {resolve} = require('path');
let webpack = require('webpack');

module.exports = {
  entry: {
    perspective: './bindings/typescript/perspective.ts'
  },
  output: {
    filename: '[name].js',
    path: resolve(__dirname, 'dist/js'),
    library: '[name]',
    libraryTarget: 'umd',
    publicPath: './dist/js/'
  },
  resolve: {
    modules: ['bindings/typescript', 'node_modules'],
    extensions: ['.ts', '.js'],
  },
  plugins: [],
  module: {
    rules: [
      {test: /\.tsx?$/, use: {loader: 'ts-loader'}}
    ]
  },
  devtool: 'inline-source-map'
};